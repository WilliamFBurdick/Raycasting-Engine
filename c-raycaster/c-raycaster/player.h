#pragma once

typedef struct {
	float x, y;
	float width, height;
	int turnDirection;	// -1 for left, +1 for right
	int moveDirection;	// -1 for backwards, +1 for forwards
	float rotation;
	float moveSpeed;
	float turnSpeed;
} player_t;

extern player_t player;

void MovePlayer(float deltaTime);
void RenderMapPlayer(void);