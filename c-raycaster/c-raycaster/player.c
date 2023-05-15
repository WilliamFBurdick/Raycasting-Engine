#include "player.h"
#include "defs.h"
#include "map.h"
#include "graphics.h"
#include "utils.h"

player_t player = {
	.x = WINDOW_WIDTH / 2,
	.y = WINDOW_HEIGHT / 2,
	.width = 5,
	.height = 5,
	.turnDirection = 0,
	.moveDirection = 0,
	.rotation = PI / 2,
	.moveSpeed = 100,
	.turnSpeed = 45 * (PI / 180)
};

void MovePlayer(float deltaTime) {
	player.rotation += player.turnDirection * player.turnSpeed * deltaTime;
	NormalizeAngle(&player.rotation);

	float moveStep = player.moveDirection * player.moveSpeed * deltaTime;

	float newX = player.x + cos(player.rotation) * moveStep;
	float newY = player.y + sin(player.rotation) * moveStep;

	if (!MapHasWallAt(newX, newY)) {
		player.x = newX;
		player.y = newY;
	}
}



void RenderMapPlayer() {
	DrawRect((player.x - (player.width / 2)) * MINIMAP_SCALE_FACTOR, (player.y - (player.height / 2))* MINIMAP_SCALE_FACTOR, player.width* MINIMAP_SCALE_FACTOR, player.height* MINIMAP_SCALE_FACTOR, 0xFF0000FF);
}