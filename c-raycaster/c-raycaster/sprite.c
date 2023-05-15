#include "sprite.h"

#define NUM_SPRITES 2

static sprite_t sprites[NUM_SPRITES] = {
	{.x = 640, .y = 630, .texture = 9},
	{.x = 660, .y = 690, .texture = 9}
};

void RenderMapSprites(void) {
	for (int i = 0; i < NUM_SPRITES; i++) {
		DrawRect(sprites[i].x * MINIMAP_SCALE_FACTOR, sprites[i].y * MINIMAP_SCALE_FACTOR, 2 * MINIMAP_SCALE_FACTOR, 2 * MINIMAP_SCALE_FACTOR, sprites[i].visible ? 0xFF00FFFF : 0xFF444444);
	}
}

void RenderSpriteProjection(void) {
	sprite_t visibleSprites[NUM_SPRITES];
	int numVisibleSprites = 0;

	// Find sprites visible inside FOV
	for (int i = 0; i < NUM_SPRITES; i++) {
		float angleSpritePlayer = player.rotation - atan2(sprites[i].y - player.y, sprites[i].x - player.x);

		// Make sure angle is always between 0 and 180 degrees
		if (angleSpritePlayer > PI) {
			angleSpritePlayer -= TWO_PI;
		}
		if (angleSpritePlayer < -PI) {
			angleSpritePlayer += TWO_PI;
		}
		angleSpritePlayer = fabs(angleSpritePlayer);

		// If sprite angle is less than half of FOV plus small error margin
		const float EPSILON = 0.2;
		if (angleSpritePlayer < (FOV_ANGLE / 2) + EPSILON) {
			sprites[i].visible = true;
			sprites[i].angle = angleSpritePlayer;
			sprites[i].distance = DistanceBetweenPoints(sprites[i].x, sprites[i].y, player.x, player.y);
			visibleSprites[numVisibleSprites] = sprites[i];
			numVisibleSprites++;
		}
		else {
			sprites[i].visible = false;
		}
	}
	
	// Sort sprites by distance using bubble-sort to ensure further sprites rendered first
	for (int i = 0; i < numVisibleSprites; i++) {
		for (int j = i + 1; j < numVisibleSprites; j++) {
			if (visibleSprites[i].distance < visibleSprites[j].distance) {
				sprite_t temp = visibleSprites[i];
				visibleSprites[i] = visibleSprites[j];
				visibleSprites[j] = temp;
			}
		}
	}

	// Render all visible sprites
	for (int i = 0; i < numVisibleSprites; i++) {
		// Draw pixels of sprite in correct screen position
		sprite_t sprite = visibleSprites[i];
		
		// Calculate perpendicular distance of sprite to avoid fish-eye
		float perpDistance = sprite.distance * cos(sprite.angle);

		// Calculate the sprite projected height and width
		float spriteHeight = (TILE_SIZE / perpDistance) * DIST_PROJ_PLANE;
		float spriteWidth = spriteHeight;

		// Sprite top Y
		float spriteTopY = (WINDOW_HEIGHT / 2) - (spriteHeight / 2);
		spriteTopY = (spriteTopY < 0) ? 0 : spriteTopY;

		// Sprite bottom Y
		float spriteBottomY = (WINDOW_HEIGHT / 2) + (spriteHeight / 2);
		spriteBottomY = (spriteBottomY > WINDOW_HEIGHT) ? WINDOW_HEIGHT : spriteBottomY;

		// Calculate the sprite X position in projection plane
		float spriteAngle = atan2(sprite.y - player.y, sprite.x - player.x) - player.rotation;
		float spriteScreenPosX = tan(spriteAngle) * DIST_PROJ_PLANE;

		float spriteLeftX = (WINDOW_WIDTH / 2) + spriteScreenPosX - (spriteWidth / 2);
		float spriteRightX = spriteLeftX + spriteWidth;

		// Query width and height of texture
		int textureWidth = upng_get_width(textures[sprite.texture]);
		int textureHeight = upng_get_height(textures[sprite.texture]);

		// Loop all x-values
		for (int x = spriteLeftX; x < spriteRightX; x++) {
			float texelWidth = (textureWidth / spriteWidth);
			int textureOffsetX = (x - spriteLeftX) * texelWidth;

			for (int y = spriteTopY; y < spriteBottomY; y++) {
				if (x > 0 && x < WINDOW_WIDTH && y > 0 && y < WINDOW_WIDTH) {
					int distanceFromTop = y + (spriteHeight / 2) - (WINDOW_HEIGHT / 2);
					int textureOffsetY = distanceFromTop * (textureHeight / spriteHeight);

					// Paint the pixel with correct texture color
					color_t* spriteTextureBuffer = (color_t*)upng_get_buffer(textures[sprite.texture]);
					color_t texelColor = spriteTextureBuffer[(textureWidth * textureOffsetY) + textureOffsetX];

					if (sprite.distance < rays[x].distance && texelColor != 0xFFFF00FF) {
						DrawPixel(x, y, texelColor);
					}

				}
			}
		}
	}
}