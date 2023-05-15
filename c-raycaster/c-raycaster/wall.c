#include "wall.h"

void RenderWallProjection(void) {
	for (int x = 0; x < NUM_RAYS; x++) {
		// Calculate perpendicular distance to correct fish-eye distortion
		float perpDistance = rays[x].distance * cos(rays[x].rayAngle - player.rotation);

		// Calculate projected wall height
		float wallHeight = (TILE_SIZE / perpDistance) * DIST_PROJ_PLANE;

		// Find wall top Y value
		int wallTopY = (WINDOW_HEIGHT / 2) - (wallHeight / 2);
		wallTopY = wallTopY < 0 ? 0 : wallTopY;

		// Find wall bottom Y value
		int wallBottomY = (WINDOW_HEIGHT / 2) + (wallHeight / 2);
		wallBottomY = wallBottomY > WINDOW_HEIGHT ? WINDOW_HEIGHT : wallBottomY;

		// Draw Ceiling
		for (int y = 0; y < wallTopY; y++) {
			DrawPixel(x, y, 0xFF444444);
		}

		// Calculate texture offset X
		int textureOffsetX, textureOffsetY;
		if (rays[x].wasHitVertical) {
			textureOffsetX = (int)rays[x].wallHitY % TILE_SIZE;
		}
		else {
			textureOffsetX = (int)rays[x].wallHitX % TILE_SIZE;
		}

		// Get correct texture ID from map content
		int texNum = rays[x].texture - 1;

		// Query texture width and height from upng
		int texture_width = upng_get_width(textures[texNum]);
		int texture_height = upng_get_height(textures[texNum]);

		// Render the wall from topY to bottomY
		for (int y = wallTopY; y < wallBottomY; y++) {
			// Calculate texture offset y
			int topDistance = y + (wallHeight / 2) - (WINDOW_HEIGHT / 2);
			textureOffsetY = topDistance * ((float)texture_height / (float)wallHeight);

			// Set color of wall based on texture color
			color_t* wallTextureBuffer = (color_t*)upng_get_buffer(textures[texNum]);
			uint32_t texelColor = wallTextureBuffer[texture_width * textureOffsetY + textureOffsetX];

			// Make pixel color darker if hit was vertical
			if (rays[x].wasHitVertical) {
				ChangeColorIntensity(&texelColor, 0.7);
			}

			DrawPixel(x, y, texelColor);
		}

		// Draw Floor
		for (int y = wallBottomY; y < WINDOW_HEIGHT; y++) {
			DrawPixel(x, y, 0xFF888888);
		}
	}
}