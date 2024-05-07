#pragma once
#include "app.h"
#include "root.h"

u32 pixels[SCREEN_WIDTH * SCREEN_HEIGHT];

internal void appUpdateAndRender() {
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

    for (int i = 0; i < ARRAY_LENGTH(pixels); ++i) {
        f32 procent = (f32)i / (f32)ARRAY_LENGTH(pixels);
        u32 red = (u32)(800.0 * procent * redOffset * 0xFF) << 24u;
        u32 green = (u32)(979.0 * procent * greenOffset * 0xFF) << 16u;
        u32 blue = (u32)(530.0 * procent * blueOffset * 0xFF) << 8u;
        u32 color = 0x000000FF | red | green | blue;
        pixels[i] = color;
    }

}
