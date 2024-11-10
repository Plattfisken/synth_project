#pragma once
#include "app.h"
#include "root.h"

global Note randomNote = {none, 0};
global Note notes[13];

global i32 selectedOctave = 0;
global u32 volume = 3000;
global int last_frame_time = 0;
global bool toggledRandomizer = false;
global u32 addedHarmonics = 1; //Zero or one?!?!??
global WaveType waveToGenerate;

internal f32 getHertz(Note note) {
    const f32 TwelvthRootOfTwoApprox = 1.059463f;
    i32 A = 440;
    i32 distanceFromA = note.Name;
    f32 result = A * pow(TwelvthRootOfTwoApprox, distanceFromA) * pow(2, note.Octave);
    return result;
}

internal i16 getSampleValue(u32 sampleIndex, i32 samplesPerSecond, i16 toneVolume) {
    f32 result = 0;
    for (i32 i = 0; i < ARRAY_LENGTH(notes); ++i) {
        if (notes[i].Name != none) {
            f32 hz = getHertz(notes[i]);
            result += (sin(2 * M_PI * sampleIndex / (samplesPerSecond / hz))) * toneVolume;
            for (i32 u = 0; u < addedHarmonics; ++u) {
                switch (waveToGenerate) {
                    case sine: {
                        result += (sin(2 * M_PI * sampleIndex / (samplesPerSecond / (hz * (u + 1))))) * (toneVolume / (u * 2));
                    } break;
                    case sawtooth: {
                        result += (sin(2 * M_PI * sampleIndex / (samplesPerSecond / (hz * (u + 1))))) * (toneVolume / (u * 2));
                        
                    } break;
                    case square: {
                        if (!(u % 2)) {
                            result += (sin(2 * M_PI * sampleIndex / (samplesPerSecond / (hz * (u + 1))))) * (toneVolume / u);
                        }
                    } break;
                    case triangle: {
                        if (u % 2) {
                            result += (sin(2 * M_PI * sampleIndex / (samplesPerSecond / (hz * (u + 1))))) * (toneVolume / u);
                        } 
                    } break;
                }
            }
        }
    }

    if (randomNote.Name != none) {
        i32 hz = getHertz(randomNote);
        result += (sin(2 * M_PI * sampleIndex / (samplesPerSecond / hz))) * toneVolume;
            for (i32 u = 0; u < addedHarmonics; ++u) {
                switch (waveToGenerate) {
                    case sine: {
                        result += (sin(2 * M_PI * sampleIndex / (samplesPerSecond / (hz * (u + 1))))) * (toneVolume / (u * 2));
                    } break;
                    case sawtooth: {
                        result += (sin(2 * M_PI * sampleIndex / (samplesPerSecond / (hz * (u + 1))))) * (toneVolume / (u * 2));
                        
                    } break;
                    case square: {
                        if (!(u % 2)) {
                            result += (sin(2 * M_PI * sampleIndex / (samplesPerSecond / (hz * (u + 1))))) * (toneVolume / u);
                        }
                    } break;
                    case triangle: {
                        if (u % 2) {
                            result += (sin(2 * M_PI * sampleIndex / (samplesPerSecond / (hz * (u + 1))))) * (toneVolume / u);
                        } 
                    } break;
                }
            }
    }
    return result;
}

internal u32 roundFloat(f32 f) {
  u32 result = (u32)(f + 0.5f);
  return result;
}

internal i32 getRandomNoteName(void) {
  i32 randNr = (rand() % 13) + none;
  return randNr;
}

internal void setup(void) {
    for (i32 i = 0; i < ARRAY_LENGTH(notes); ++i) {
        notes[i].Name = none;
    }
    srand(time(NULL));
}

internal void appUpdateAndRender(u32 *pixels, size_t size) {
    persistent f32 redOffset;
    if (redOffset >= 1.0f)
        redOffset = 0.0f;
    redOffset += 0.001f; 

    persistent f32 blueOffset;
    if (blueOffset >= 1.0f)
        blueOffset = 0.0f;
    blueOffset += 0.002f; 

    persistent f32 greenOffset;
    if (greenOffset >= 1.0f)
        greenOffset = 0.0f;
    greenOffset += 0.003f; 

    for (int i = 0; i < size; ++i) {
        f32 procent = (f32)i / (f32)size;
        u32 red = (u32)(800.0 * procent * redOffset * 0xFF) << 24u;
        u32 green = (u32)(979.0 * procent * greenOffset * 0xFF) << 16u;
        u32 blue = (u32)(530.0 * procent * blueOffset * 0xFF) << 8u;
        u32 color = 0x000000FF | red | green | blue;
        pixels[i] = color;
    }
}
