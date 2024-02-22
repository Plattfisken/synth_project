#include "./constants.h"
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef float f32;
typedef double f64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#define ARRAY_LENGTH(array) (sizeof((array)) / sizeof((array)[0]))

struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  u32 pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
  bool quit;
} state;
enum note { 
  none,
  C = 262, 
  CSharp = 277, 
  D = 294, 
  DSharp = 311,
  E = 330, 
  F = 349, 
  FSharp = 370, 
  G = 392, 
  GSharp = 415, 
  A = 440, 
  ASharp = 466, 
  B = 494
} noteToPlay;
i32 _octave = 0;
int last_frame_time = 0;
int initialize_window(void) {

  if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_AUDIO) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  state.window =
      SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
  if (!state.window) {
    fprintf(stderr, "Error creating SDL Window\n");
    return false;
  }

  state.renderer =
      SDL_CreateRenderer(state.window, -1, SDL_RENDERER_PRESENTVSYNC);
  if (!state.renderer) {
    fprintf(stderr, "Error creating SDL renderer\n");
    return false;
  }

  state.texture = SDL_CreateTexture(state.renderer, SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH,
                                    SCREEN_HEIGHT);
  if (!state.texture) {
    fprintf(stderr, "Error creating SDL texture\n");
    return false;
  }
  return true;
}

void process_input() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT:
      state.quit = true;
      break;
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
          state.quit = true;
          break; 
        case SDLK_z:
          noteToPlay = C;
          break;
        case SDLK_s:
          noteToPlay = CSharp;
          break;
        case SDLK_x:
          noteToPlay = D;
          break;
        case SDLK_d:
          noteToPlay = DSharp;
          break;
        case SDLK_c:
          noteToPlay = E;
          break;
        case SDLK_v:
          noteToPlay = F;
          break;
        case SDLK_g:
          noteToPlay = FSharp;
          break;
        case SDLK_b:
          noteToPlay = G;
          break;
        case SDLK_h:
          noteToPlay = GSharp;
          break;
        case SDLK_n:
          noteToPlay = A;
          break;
        case SDLK_j:
          noteToPlay = ASharp;
          break;
        case SDLK_m:
          noteToPlay = B;
          break;
        case SDLK_COMMA:
          noteToPlay = C * 2;
          break;
        case SDLK_w:
          ++_octave;
          break;
        case SDLK_q:
          --_octave;
          break;
      }
      break;
    case SDL_KEYUP:
      noteToPlay = none;
      break;
  }
}
// void SDLAudioCallback(void *userData, u8 *audioData, int length) {
//   memset(audioData, 0, length);
// }
SDL_AudioDeviceID SDLInitAudio(i32 bufferSize, i32 samplesPerSecond) {
  SDL_AudioSpec audioSettings = {0};
  SDL_AudioSpec obtained;
  audioSettings.freq = samplesPerSecond; 
  audioSettings.format = AUDIO_S16LSB;
  audioSettings.channels = 2;
  audioSettings.samples = bufferSize;
  audioSettings.callback = NULL; //&SDLAudioCallback;

  SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &audioSettings, &obtained, SDL_AUDIO_ALLOW_ANY_CHANGE);

  if(obtained.format != AUDIO_S16LSB) {
    fprintf(stderr, "Could not get S16LSB as sample format\n");
    SDL_CloseAudio();
    return 0;
  }
  return deviceId;
}
void setup() {
}

void update() {
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }
  // Get a delta time factor converted to seconds to be used to update my
  // objects
  float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

  last_frame_time = SDL_GetTicks();
  // memset(state.pixels, 0xFFFF00FF, sizeof(state.pixels));
  
  for (int i = 0; i < ARRAY_LENGTH(state.pixels); ++i) {
    f32 procent = (f32)i / (f32)ARRAY_LENGTH(state.pixels);  
    u32 red = (u32)(procent * 0xFF) << 24u;
    u32 green = (u32)(procent * 0xFF) << 16u;
    u32 blue = (u32)(procent * 0xFF) << 8u;
    u32 color = 0x000000FF | red | green | blue;
    state.pixels[i] = color;
  }
  SDL_UpdateTexture(state.texture, NULL, state.pixels, SCREEN_WIDTH * 4);
}

void render() {
  SDL_RenderClear(state.renderer);
  SDL_RenderCopy(state.renderer, state.texture, NULL, NULL);
  SDL_RenderPresent(state.renderer);
}

void destroy_window() {
  SDL_DestroyTexture(state.texture);
  SDL_DestroyRenderer(state.renderer);
  SDL_DestroyWindow(state.window);

  SDL_CloseAudio(); //Not strictly necessary, SDL_Quit should already do this
  SDL_Quit();
}
void playTone(SDL_AudioDeviceID deviceId, i32 samplesPerSecond, i32 hz) {
  i32 toneHz = hz;
  i16 toneVolume = 3000;
  static u32 runningSampleIndex = 0;
  i32 squareWavePeriod = samplesPerSecond / toneHz;
  i32 halfSquareWavePeriod = squareWavePeriod / 2;
  i32 bytesPerSample = sizeof(i16) * 2;
  bool soundIsPlaying = false;
    i32 targetQueueBytes = 4800 * bytesPerSample;
    i32 bytesToWrite = targetQueueBytes - SDL_GetQueuedAudioSize(deviceId);
    if(bytesToWrite) {
      void *soundBuffer = malloc(bytesToWrite);
      i16 *sampleOut = (i16 *)soundBuffer;
      i32 sampleCount = bytesToWrite/bytesPerSample;
      for(i32 sampleIndex = 0; sampleIndex < sampleCount; ++sampleIndex) {
        i16 sampleValue = ((runningSampleIndex++ / halfSquareWavePeriod) % 2) ? toneVolume : -toneVolume;
        *sampleOut++ = sampleValue;
        *sampleOut++ = sampleValue;
      }
      if(SDL_QueueAudio(deviceId, soundBuffer, bytesToWrite) != 0) {
        fprintf(stderr,"%s", SDL_GetError());
      }
      free(soundBuffer);
    }
    if(!soundIsPlaying) {
      SDL_PauseAudioDevice(deviceId, 0);
      soundIsPlaying = true;
    }
}
int main() {
  state.quit = !initialize_window();
  setup();
  i32 samplesPerSecond = 48000;
  SDL_AudioDeviceID deviceId = SDLInitAudio(4096, samplesPerSecond);
  if(deviceId == 0) {
    fprintf(stderr, "Error when opening audio device\n");
  }

  while (!state.quit) {
    
    process_input();
    update();
    render();
    if(noteToPlay != none)
      playTone(deviceId, samplesPerSecond, noteToPlay * pow(2, _octave));
    else
      SDL_ClearQueuedAudio(deviceId);
  }
  destroy_window();

  return 0;
}
