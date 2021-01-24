#pragma once
#include "Entity.h"
#include <SDL2/SDL.h>
#include <memory>

class Camera
{
public:
	Camera(const double& render_time_ratio):
		render_time_ratio_(render_time_ratio)
	{}

	SDL_Rect get_rect();

	void update_position();

	void set_target(std::weak_ptr<Entity> target){
		target_ = target;
	}

	int x = 0;
	int y = 0;
	int w = 1920;
	int h = 1080;
private:
	std::weak_ptr<Entity> target_;
	const double& render_time_ratio_;
};

