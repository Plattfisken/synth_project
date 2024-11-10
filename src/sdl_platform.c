#include "root.h"

struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    u32 pixels[SCREEN_WIDTH * SCREEN_HEIGHT];
    bool quit;
} state;

internal bool SDLInitializeWindow(void) {

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

internal void playAudioData(SDL_AudioDeviceID deviceId, i32 samplesPerSecond, i16 toneVolume) {
    persistent u32 runningSampleIndex = 0;
    i32 bytesPerSample = sizeof(i16) * 2;
    i32 targetQueueBytes = 4800 * bytesPerSample;
    i32 bytesToWrite = targetQueueBytes - SDL_GetQueuedAudioSize(deviceId);
    if (bytesToWrite) {
        void *soundBuffer = malloc(bytesToWrite);
        i16 *sampleOut = (i16 *)soundBuffer;
        i32 sampleCount = bytesToWrite / bytesPerSample;
        for (i32 sampleIndex = 0; sampleIndex < sampleCount; ++sampleIndex) {
            i16 sampleValue = getSampleValue(runningSampleIndex, samplesPerSecond, toneVolume);
            ++runningSampleIndex;
            *sampleOut++ = sampleValue;
            *sampleOut++ = sampleValue;
        }
        if (SDL_QueueAudio(deviceId, soundBuffer, bytesToWrite) != 0) {
            fprintf(stderr, "%s", SDL_GetError());
        }
        free(soundBuffer);
        SDL_PauseAudioDevice(deviceId, 0);
    }
}

internal void process_input(void) {
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
      notes[0].Name = C;
      notes[0].Octave = selectedOctave;
      break;
    case SDLK_s:
      notes[1].Name = CSharp;
      notes[1].Octave = selectedOctave;
      break;
    case SDLK_x:
      notes[2].Name = D;
      notes[2].Octave = selectedOctave;
      break;
    case SDLK_d:
      notes[3].Name = DSharp;
      notes[3].Octave = selectedOctave;
      break;
    case SDLK_c:
      notes[4].Name = E;
      notes[4].Octave = selectedOctave;
      break;
    case SDLK_v:
      notes[5].Name = F;
      notes[5].Octave = selectedOctave;
      break;
    case SDLK_g:
      notes[6].Name = FSharp;
      notes[6].Octave = selectedOctave;
      break;
    case SDLK_b:
      notes[7].Name = G;
      notes[7].Octave = selectedOctave;
      break;
    case SDLK_h:
      notes[8].Name = GSharp;
      notes[8].Octave = selectedOctave;
      break;
    case SDLK_n:
      notes[9].Name = A;
      notes[9].Octave = selectedOctave;
      break;
    case SDLK_j:
      notes[10].Name = ASharp;
      notes[10].Octave = selectedOctave;
      break;
    case SDLK_m:
      notes[11].Name = B;
      notes[11].Octave = selectedOctave;
      break;
    case SDLK_COMMA:
      notes[12].Name = C;
      notes[12].Octave = selectedOctave + 1;
      break;
    case SDLK_w:
      ++selectedOctave;
      break;
    case SDLK_q:
      --selectedOctave;
      break;
    case SDLK_i:
      if (volume >= 20)
        volume -= 20;
      else
        volume = 0;
      break;
    case SDLK_o:
      volume += 20;
      break;
    case SDLK_r:
      toggledRandomizer = toggledRandomizer == false ? true : false;
      break;
    case SDLK_y:
      ++addedHarmonics;
      break;
    case SDLK_t:
      if (addedHarmonics > 1)
        --addedHarmonics;
      break;
    case SDLK_1:
      waveToGenerate = sine;
      addedHarmonics = 0;
      break;
    case SDLK_2:
      waveToGenerate = sawtooth;
      addedHarmonics = 10;
      break;
    case SDLK_3:
      waveToGenerate = square;
      addedHarmonics = 10;
      break;
    case SDLK_4:
      waveToGenerate = triangle;
      addedHarmonics = 10;
      break;
    }
    break;
  case SDL_KEYUP:
    switch (event.key.keysym.sym) {
    case SDLK_z:
      notes[0].Name = none;
      break;
    case SDLK_s:
      notes[1].Name = none;
      break;
    case SDLK_x:
      notes[2].Name = none;
      break;
    case SDLK_d:
      notes[3].Name = none;
      break;
    case SDLK_c:
      notes[4].Name = none;
      break;
    case SDLK_v:
      notes[5].Name = none;
      break;
    case SDLK_g:
      notes[6].Name = none;
      break;
    case SDLK_b:
      notes[7].Name = none;
      break;
    case SDLK_h:
      notes[8].Name = none;
      break;
    case SDLK_n:
      notes[9].Name = none;
      break;
    case SDLK_j:
      notes[10].Name = none;
      break;
    case SDLK_m:
      notes[11].Name = none;
      break;
    case SDLK_COMMA:
      notes[12].Name = none;
      break;
    }
    break;
  }
}

// internal void SDLQuitApp(void) {
//     SDL_DestroyTexture(state.texture);
//     SDL_DestroyRenderer(state.renderer);
//     SDL_DestroyWindow(state.window);
//
//     SDL_CloseAudio(); // Not strictly necessary, SDL_Quit should already do this
//     SDL_Quit();
// }

internal void render(void) {
    SDL_RenderClear(state.renderer);
    SDL_RenderCopy(state.renderer, state.texture, NULL, NULL);
    SDL_RenderPresent(state.renderer);
}


int main(int argc, char **argv) {
    state.quit = !SDLInitializeWindow();
    setup();

    i32 samplesPerSecond = 48000;
    SDL_AudioDeviceID deviceId = SDLInitAudio(1024, samplesPerSecond);
    if (deviceId == 0) {
        fprintf(stderr, "Error when opening audio device\n");
    }

    while (!state.quit) {

        process_input();

        int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

        if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
            SDL_Delay(time_to_wait);
        }
        // Get a delta time factor converted to seconds to be used to update my
        // objects
        float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

        last_frame_time = SDL_GetTicks();

        //Fixa detta, kanske skriva om hur texturerna ritas upp på skärmen sdl_back_buffer?
        appUpdateAndRender(state.pixels, ARRAY_LENGTH(state.pixels));
        SDL_UpdateTexture(state.texture, NULL, state.pixels , SCREEN_WIDTH * 4);

        render();
        if (toggledRandomizer) {
            randomNote.Name = getRandomNoteName();
            randomNote.Octave = selectedOctave;
        } 
        else {
            randomNote.Name = none;
        }
        bool playSound = false;
        if (randomNote.Name != none) {
            playSound = true;
        }
        for (int i = 0; i < ARRAY_LENGTH(notes); ++i) {
            if (notes[i].Name != none) {
                playSound = true;
                break;
            }
        }
        if (playSound)
            playAudioData(deviceId, samplesPerSecond, volume);
        else
            SDL_ClearQueuedAudio(deviceId);
    }
    SDL_Quit();
    return 0;
}
