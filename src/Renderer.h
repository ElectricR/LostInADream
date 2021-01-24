#pragma once
#include <SDL2/SDL.h>
#include <set>
#include <map>
#include "utils.h"
#include "Globals.h"
#include "Entity.h"

class Camera;

class Renderer {
public:
	Renderer(SDL_Renderer *sdl_renderer, 
			const std::shared_ptr<Camera>& camera, 
			const double& render_time_ratio):
		sdl_renderer_(sdl_renderer),
		camera_(camera),
		render_time_ratio_(render_time_ratio)
	{}

	void render();

	void register_new_realm(global::Realm realm) {
		rend_list_[realm] = std::multiset<std::weak_ptr<Entity>, EntityLayerComparator>();
	}

	void add(global::Realm realm, std::weak_ptr<Entity> entity) {
		rend_list_[realm].insert(entity);
	}

private:
	std::map<global::Realm, 
		std::multiset<std::weak_ptr<Entity>, EntityLayerComparator>> rend_list_;
	SDL_Renderer *sdl_renderer_;
	std::shared_ptr<Camera> camera_;
	const double& render_time_ratio_;
};

