#pragma once
#include "defs.h"
#include <stdbool.h>

#define MAP_NUM_ROWS 13
#define MAP_NUM_COLS 20

bool MapHasWallAt(float x, float y);
bool IsInsideMap(float x, float y);
void RenderMapGrid(void);
int GetMapAt(int i, int j);