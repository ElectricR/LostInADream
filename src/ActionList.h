#pragma once

enum class ActionType: long{
	PLAYER_MOVE,
	PLAYER_UPDATE_GRID_POSITION,
	PLAYER_COLLIDE,
	PLAYER_SHOOT,
	PLAYER_USE,
};