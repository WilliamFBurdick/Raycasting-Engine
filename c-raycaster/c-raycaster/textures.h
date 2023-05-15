#ifndef TEXTURES_H
#define TEXTURES_H
#include "upng.h"
#include "defs.h"
#include <stdint.h>

#define NUM_TEXTURES 14

upng_t* textures[NUM_TEXTURES];

void LoadTextures();
void FreeTextures();
#endif