#pragma once

#include <stdbool.h>
#include "defs.h"
#include "player.h"
#include "utils.h"

typedef struct {
	float rayAngle;
	float wallHitX, wallHitY;
	float distance;
	int wasHitVertical;
	int wallHitContent;
} ray_t;

extern ray_t rays[NUM_RAYS];

void CastRay(float rayAngle, int stripID);
void CastAllRays(void);
void RenderMapRays(void);

bool IsRayFacingUp(float angle);
bool IsRayFacingDown(float angle);
bool IsRayFacingLeft(float angle);
bool IsRayFacingRight(float angle);