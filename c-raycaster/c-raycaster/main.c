#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "defs.h"
#include "textures.h"
#include "graphics.h"
#include "map.h"
#include "ray.h"
#include "player.h"
#include "wall.h"
#include "sprite.h"

bool isGameRunning = false;
int ticksLastFrame = 0;

void Setup() {
	LoadTextures();
}

void ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isGameRunning = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				isGameRunning = false;
			}
			if (event.key.keysym.sym == SDLK_UP) {
				player.moveDirection = 1;
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				player.moveDirection = -1;
			}
			if (event.key.keysym.sym == SDLK_LEFT) {
				player.turnDirection = -1;
			}
			if (event.key.keysym.sym == SDLK_RIGHT) {
				player.turnDirection = 1;
			}
			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_UP) {
				player.moveDirection = 0;
			}
			if (event.key.keysym.sym == SDLK_DOWN) {
				player.moveDirection = 0;
			}
			if (event.key.keysym.sym == SDLK_LEFT) {
				player.turnDirection = 0;
			}
			if (event.key.keysym.sym == SDLK_RIGHT) {
				player.turnDirection = 0;
			}
			break;
		}
	}
}

void Update() {
	// sleep until target frame time length
	int waitTime = FRAME_TIME_LENGTH - (SDL_GetTicks() - ticksLastFrame);
	if (waitTime > 0 && waitTime <= FRAME_TIME_LENGTH) {
		SDL_Delay(waitTime);
	}

	float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;
	ticksLastFrame = SDL_GetTicks();

	MovePlayer(deltaTime);
	CastAllRays();
}

void Render() {
	ClearColorBuffer(0xFF000000); 
	RenderWallProjection();
	RenderSpriteProjection();

	// Render minimap
	RenderMapGrid();
	RenderMapRays();
	RenderMapSprites();
	RenderMapPlayer();

	
	RenderColorBuffer();
};

void ReleaseResources(void) {
	FreeTextures();
	DestroyWindow();
}

int main(int argc, char* argv[]) {
	isGameRunning = InitializeWindow();
	Setup();
	while (isGameRunning) {
		ProcessInput();
		Update();
		Render();
	}

	ReleaseResources();

	return 0;
}