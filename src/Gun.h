#pragma once
#include <memory>
#include "Entity.h"
#include "Events.h"
#include "Action.h"

class Character;

class Gun:
	public Entity
{
public:
	Gun(SDL_Texture *img, 
			EntityType id, 
			int layer, 
			int x, 
			int y, 
			int scale,
		       	std::weak_ptr<Character> parent,
			const std::shared_ptr<Camera>& camera,
			const int &mouse_x,
			const int &mouse_y,
			const uint32_t &current_time):
		Entity(img, id, layer, x, y, scale),
		parent_(parent),
		camera_(camera),
		mouse_x_(mouse_x),
		mouse_y_(mouse_y),
		current_time_(current_time)
	{}

	ActionStatus move_update(Events::Event **);
	ActionStatus shoot(Events::Event **);
private:
	std::weak_ptr<Character> parent_;
	std::shared_ptr<Camera> camera_;
	const int &mouse_x_;
	const int &mouse_y_;
	const uint32_t &current_time_;
	uint32_t shot_last_time_ = 0;
};

