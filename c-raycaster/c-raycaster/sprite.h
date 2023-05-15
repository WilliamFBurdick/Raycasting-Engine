#pragma once
#include "graphics.h"
#include "defs.h"
#include "player.h"
#include "textures.h"
#include <stdbool.h>
#include "utils.h"
#include "upng.h"
#include "ray.h"

typedef struct {
	float x;
	float y;
	float distance;
	float angle;
	bool visible;
	int texture;
} sprite_t;

void RenderSpriteProjection(void);
void RenderMapSprites(void);