#include "defs.h"
#include "graphics.h"
#include <stdio.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

static color_t* colorBuffer = NULL;
static SDL_Texture* colorBufferTexture;

bool InitializeWindow() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	int fullscreenWidth = display_mode.w;
	int fullscreenHeight = display_mode.h;
	window = SDL_CreateWindow("C - Raycaster", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, fullscreenWidth, fullscreenHeight, SDL_WINDOW_BORDERLESS);
	if (window == NULL) {
		fprintf(stderr, "Error creating SDL Window.\n");
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		fprintf(stderr, "Error creating SDL Renderer.\n");
		return false;
	}
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// Allocate bytes in memory for color buffer
	colorBuffer = (color_t*)malloc(sizeof(color_t) * (color_t)WINDOW_WIDTH * (color_t)WINDOW_HEIGHT);

	colorBufferTexture = SDL_CreateTexture(
		renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT
	);

	return true;
}

void DestroyWindow() {
	free(colorBuffer);
	SDL_DestroyTexture(colorBufferTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void ClearColorBuffer(color_t color) {
	for (int i = 0; i < WINDOW_WIDTH * WINDOW_HEIGHT; i++) {
		colorBuffer[i] = color;
	}
}

void RenderColorBuffer() {
	SDL_UpdateTexture(colorBufferTexture, NULL, colorBuffer, (int)((color_t)WINDOW_WIDTH * sizeof(color_t)));
	SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void DrawPixel(int x, int y, color_t color) {
	colorBuffer[(WINDOW_WIDTH * y) + x] = color;
}

void DrawRect(int x, int y, int width, int height, color_t color) {
	for (int i = x; i <= (x + width); i++) {
		for (int j = y; j <= (y + height); j++) {
			DrawPixel(i, j, color);
		}
	}
}

void DrawLine(int x0, int y0, int x1, int y1, color_t color) {
	int delta_x = (x1 - x0);
	int delta_y = (y1 - y0);

	int side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

	float x_inc = delta_x / (float)side_length;
	float y_inc = delta_y / (float)side_length;

	float current_x = x0;
	float current_y = y0;

	for (int i = 0; i <= side_length; i++) {
		DrawPixel(round(current_x), round(current_y), color);
		current_x += x_inc;
		current_y += y_inc;
	}
}

void ChangeColorIntensity(color_t* color, float factor) {
	color_t a = (*color & 0xFF000000);
	color_t r = (*color & 0x00FF0000) * factor;
	color_t g = (*color & 0x0000FF00) * factor;
	color_t b = (*color & 0x000000FF) * factor;

	*color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
}