#pragma once
#include <stdbool.h>
#include <SDL.h>
#include <stdint.h>
#include "defs.h"

bool InitializeWindow(void);
void DestroyWindow(void);
void ClearColorBuffer(color_t color);
void RenderColorBuffer(void);
void DrawPixel(int x, int y, color_t color);
void DrawRect(int x, int y, int width, int height, color_t color);
void DrawLine(int x0, int y0, int x1, int y1, color_t color);
void ChangeColorIntensity(color_t* color, float factor);