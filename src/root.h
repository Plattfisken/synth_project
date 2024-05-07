#pragma once 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <SDL2/SDL.h>

#include "constants.h"
#include "types.h"

#define global static
#define persistent static
#define internal static

#define ARRAY_LENGTH(array) (sizeof((array)) / sizeof((array)[0]))

#include "app.c"
#include "app.h"
