#include "ray.h"
#include <float.h>
#include <stdbool.h>
#include "map.h"
#include "graphics.h"

ray_t rays[NUM_RAYS];

bool IsRayFacingDown(float angle) {
	return (angle > 0 && angle < PI);
}

bool IsRayFacingUp(float angle) {
	return !IsRayFacingDown(angle);
}

bool IsRayFacingRight(float angle) {
	return (angle < 0.5 * PI || angle > 1.5 * PI);
}

bool IsRayFacingLeft(float angle) {
	return !IsRayFacingRight(angle);
}

void CastRay(float rayAngle, int stripID) {
	NormalizeAngle(&rayAngle);

	float xintercept, yintercept;
	float xstep, ystep;
	/////////////////////////////////////
	// HORIZONTAL RAY-GRID INTERSECTION//
	/////////////////////////////////////
	bool foundHorizWallHit = false;
	float horizWallHitX = 0;
	float horizWallHitY = 0;
	int horizWallContent = 0;

	// find y-coordinate of closest horizontal grid intersection
	yintercept = floor(player.y / TILE_SIZE) * TILE_SIZE;
	yintercept += (IsRayFacingDown(rayAngle)) ? TILE_SIZE : 0;
	// find x-coordinate of closest horizontal grid intersection
	xintercept = player.x + (yintercept - player.y) / tan(rayAngle);

	// Calculate increment for xstep and ystep
	ystep = TILE_SIZE;
	ystep *= (IsRayFacingUp(rayAngle)) ? -1 : 1;

	xstep = ystep / tan(rayAngle);
	xstep *= (IsRayFacingLeft(rayAngle) && xstep > 0) ? -1 : 1;
	xstep *= (IsRayFacingRight(rayAngle) && xstep < 0) ? -1 : 1;

	float nextHorizTouchX = xintercept;
	float nextHorizTouchY = yintercept;


	// Increment xstep and ystep until we find a wall
	while (IsInsideMap(nextHorizTouchX, nextHorizTouchY)) {
		float xToCheck = nextHorizTouchX;
		float yToCheck = nextHorizTouchY + (IsRayFacingUp(rayAngle) ? -1 : 0);

		if (MapHasWallAt(xToCheck, yToCheck)) {
			horizWallHitX = nextHorizTouchX;
			horizWallHitY = nextHorizTouchY;
			horizWallContent = GetMapAt((int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE));
			foundHorizWallHit = true;
			break;
		}
		else {
			nextHorizTouchX += xstep;
			nextHorizTouchY += ystep;
		}
	}

	/////////////////////////////////////
	// VERTICAL RAY-GRID INTERSECTION//
	/////////////////////////////////////
	bool foundVertWallHit = false;
	float vertWallHitX = 0;
	float vertWallHitY = 0;
	int vertWallContent = 0;

	// find x-coordinate of closest vertical grid intersection
	xintercept = floor(player.x / TILE_SIZE) * TILE_SIZE;
	xintercept += (IsRayFacingRight(rayAngle)) ? TILE_SIZE : 0;
	// find y-coordinate of closest horizontal grid intersection
	yintercept = player.y + (xintercept - player.x) * tan(rayAngle);

	// Calculate increment for xstep and ystep
	xstep = TILE_SIZE;
	xstep *= (IsRayFacingLeft(rayAngle)) ? -1 : 1;

	ystep = TILE_SIZE * tan(rayAngle);
	ystep *= (IsRayFacingUp(rayAngle) && ystep > 0) ? -1 : 1;
	ystep *= (IsRayFacingDown(rayAngle) && ystep < 0) ? -1 : 1;

	float nextVertTouchX = xintercept;
	float nextVertTouchY = yintercept;

	// Increment xstep and ystep until we find a wall
	while (IsInsideMap(nextVertTouchX, nextVertTouchY)) {
		float xToCheck = nextVertTouchX + (IsRayFacingLeft(rayAngle) ? -1 : 0);
		float yToCheck = nextVertTouchY;

		if (MapHasWallAt(xToCheck, yToCheck)) {
			vertWallHitX = nextVertTouchX;
			vertWallHitY = nextVertTouchY;
			vertWallContent = GetMapAt((int)floor(yToCheck / TILE_SIZE), (int)floor(xToCheck / TILE_SIZE));
			foundVertWallHit = true;
			break;
		}
		else {
			nextVertTouchX += xstep;
			nextVertTouchY += ystep;
		}
	}

	// Calculate distances. Choose smallest.
	float horizHitDistance = foundHorizWallHit ?
		DistanceBetweenPoints(player.x, player.y, horizWallHitX, horizWallHitY)
		: FLT_MAX;
	float vertHitDistance = foundVertWallHit ?
		DistanceBetweenPoints(player.x, player.y, vertWallHitX, vertWallHitY)
		: FLT_MAX;

	if (vertHitDistance < horizHitDistance) {
		rays[stripID].wallHitX = vertWallHitX;
		rays[stripID].wallHitY = vertWallHitY;
		rays[stripID].distance = vertHitDistance;
		rays[stripID].wasHitVertical = true;
		rays[stripID].wallHitContent = vertWallContent;
		rays[stripID].rayAngle = rayAngle;
	}
	else {
		rays[stripID].wallHitX = horizWallHitX;
		rays[stripID].wallHitY = horizWallHitY;
		rays[stripID].distance = horizHitDistance;
		rays[stripID].wasHitVertical = false;
		rays[stripID].wallHitContent = horizWallContent;
		rays[stripID].rayAngle = rayAngle;
	}

}

void CastAllRays(void) {
	for (int col = 0; col < NUM_RAYS; col++) {

		float rayAngle = player.rotation + atan((col - NUM_RAYS / 2) / DIST_PROJ_PLANE);
		CastRay(rayAngle, col);
	}
}

void RenderMapRays(void) {
	for (int i = 0; i < NUM_RAYS; i++) {
		DrawLine(
			player.x * MINIMAP_SCALE_FACTOR, player.y * MINIMAP_SCALE_FACTOR, rays[i].wallHitX * MINIMAP_SCALE_FACTOR, rays[i].wallHitY * MINIMAP_SCALE_FACTOR, 0xFF0000FF);
	}
}