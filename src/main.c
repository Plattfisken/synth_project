#include "./constants.h"
#include <SDL2/SDL.h>
#include <math.h>
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
enum NoteName {
  none = 100, // Any value that can never be reached in another way
  C = -9,
  CSharp = -8,
  D = -7,
  DSharp = -6,
  E = -5,
  F = -4,
  FSharp = -3,
  G = -2,
  GSharp = -1,
  A = 0,
  ASharp = 1,
  B = 2
};
typedef struct Note {
  enum NoteName Name;
  i32 Octave;
} Note; 
Note noteToPlay = {none, 0};

i32 selectedOctave = 0;
u32 volume = 3000;
int last_frame_time = 0;

int initialize_window(void) {

  if (SDL_Init(SDL_INIT_EVERYTHING | SDL_INIT_AUDIO) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }

  state.window = SDL_CreateWindow(
      "test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
      SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
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
          noteToPlay.Name = C;
          noteToPlay.Octave = selectedOctave;
          break;
        case SDLK_s:
          noteToPlay.Name = CSharp;
          noteToPlay.Octave = selectedOctave;
          break;
        case SDLK_x:
          noteToPlay.Name = D;
          noteToPlay.Octave = selectedOctave;
          break;
        case SDLK_d:
          noteToPlay.Name = DSharp;
          noteToPlay.Octave = selectedOctave;
          break;
        case SDLK_c:
          noteToPlay.Name = E;
          noteToPlay.Octave = selectedOctave;
          break;
        case SDLK_v:
          noteToPlay.Name = F;
          noteToPlay.Octave = selectedOctave;
          break;
        case SDLK_g:
          noteToPlay.Name = FSharp;
          noteToPlay.Octave = selectedOctave;
          break;
        case SDLK_b:
          noteToPlay.Name = G;
          noteToPlay.Octave = selectedOctave;
          break;
        case SDLK_h:
          noteToPlay.Name = GSharp;
          noteToPlay.Octave = selectedOctave;
          break;
        case SDLK_n:
          noteToPlay.Name = A;
          noteToPlay.Octave = selectedOctave;
          break;
        case SDLK_j:
          noteToPlay.Name = ASharp;
          noteToPlay.Octave = selectedOctave;
          break;
        case SDLK_m:
          noteToPlay.Name = B;
          noteToPlay.Octave = selectedOctave;
          break;
        case SDLK_COMMA:
          noteToPlay.Name = C;
          noteToPlay.Octave = selectedOctave + 1;
          break;
        case SDLK_w:
          ++selectedOctave;
          break;
        case SDLK_q:
          --selectedOctave;
          break;
        case SDLK_i:
          if(volume >= 100)
            volume -= 100;
          else
            volume = 0;
          break;
        case SDLK_o:
          volume +=100;
          break;
      }
      break;
    case SDL_KEYUP:
      noteToPlay.Name = none;
      break;
  }
}
void setup() {}
void update() {
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }
  // Get a delta time factor converted to seconds to be used to update my
  // objects
  float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

  last_frame_time = SDL_GetTicks();

  static f32 redOffset;
  if (redOffset >= 1.0f)
    redOffset = 0.0f;
  redOffset += 0.001f;

  static f32 blueOffset;
  if (blueOffset >= 1.0f)
    blueOffset = 0.0f;
  blueOffset += 0.002f;

  static f32 greenOffset;
  if (greenOffset >= 1.0f)
    greenOffset = 0.0f;
  greenOffset += 0.003f;

  for (int i = 0; i < ARRAY_LENGTH(state.pixels); ++i) {
    f32 procent = (f32)i / (f32)ARRAY_LENGTH(state.pixels);
    u32 red = (u32)(procent * redOffset * 0xFF) << 24u;
    u32 green = (u32)(procent * greenOffset * 0xFF) << 16u;
    u32 blue = (u32)(procent * blueOffset * 0xFF) << 8u;
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

  SDL_CloseAudio(); // Not strictly necessary, SDL_Quit should already do this
  SDL_Quit();
}
SDL_AudioDeviceID SDLInitAudio(i32 bufferSize, i32 samplesPerSecond) {
  SDL_AudioSpec audioSettings = {0};
  SDL_AudioSpec obtained;
  audioSettings.freq = samplesPerSecond;
  audioSettings.format = AUDIO_S16LSB;
  audioSettings.channels = 2;
  audioSettings.samples = bufferSize;
  audioSettings.callback = NULL;

  SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(
      NULL, 0, &audioSettings, &obtained, SDL_AUDIO_ALLOW_ANY_CHANGE);

  if (obtained.format != AUDIO_S16LSB) {
    fprintf(stderr, "Could not get S16LSB as sample format\n");
    SDL_CloseAudio();
    return 0;
  }
  return deviceId;
}
f32 getHertz(Note note) {
  const f32 TwelvthSqrtOfTwoApprox = 1.059463f;
  i32 A = 440;
  i32 distanceFromA = note.Name;
  f32 result =
      A * pow(TwelvthSqrtOfTwoApprox, distanceFromA) * pow(2, note.Octave);
  return result;
}
u32 roundFloat(f32 f) {
  u32 result = (u32)(f + 0.5f);
  return result;
}
void playSoundWave(SDL_AudioDeviceID deviceId, i32 samplesPerSecond, f32 hz, i16 toneVolume) {
  static u32 runningSampleIndex = 0;
  f32 squareWavePeriod = samplesPerSecond / hz;
  i32 bytesPerSample = sizeof(i16) * 2;
  i32 targetQueueBytes = 4800 * bytesPerSample;
  i32 bytesToWrite = targetQueueBytes - SDL_GetQueuedAudioSize(deviceId);
  if (bytesToWrite) {
    void *soundBuffer = malloc(bytesToWrite);
    i16 *sampleOut = (i16 *)soundBuffer;
    i32 sampleCount = bytesToWrite / bytesPerSample;
    for (i32 sampleIndex = 0; sampleIndex < sampleCount; ++sampleIndex) {
      i16 sampleValue = sin(2 * M_PI * runningSampleIndex++ / squareWavePeriod) * toneVolume;  
      *sampleOut++ = sampleValue;
      *sampleOut++ = sampleValue;
    }
       if (SDL_QueueAudio(deviceId, soundBuffer, bytesToWrite) != 0) {
      fprintf(stderr, "%s", SDL_GetError());
    }
    free(soundBuffer);
  }
  SDL_PauseAudioDevice(deviceId, 0);
}
int main() {
  state.quit = !initialize_window();
  setup();
  i32 samplesPerSecond = 48000;
  SDL_AudioDeviceID deviceId = SDLInitAudio(4096, samplesPerSecond);
  if (deviceId == 0) {
    fprintf(stderr, "Error when opening audio device\n");
  }

  while (!state.quit) {

    process_input();
    update();
    render();
    if (noteToPlay.Name != none)
      playSoundWave(deviceId, samplesPerSecond, getHertz(noteToPlay), volume);
    else
      SDL_ClearQueuedAudio(deviceId);
  }
  destroy_window();

  return 0;
}
