#include "types.h"

#ifndef APP_H
#define APP_H

typedef enum {
  none = -10, // Any value that can never be reached in another way
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
} NoteName;

typedef enum { 
    sine, 
    sawtooth, 
    square, 
    triangle 
} WaveType;

typedef struct {
  NoteName Name;
  i32 Octave;
  // u32 Volume;
} Note;

#endif // !APP_H
