#pragma once
#include "Active.h"
#include "Default.h"
#include <memory>

class Gun;

class Character:
	public Active, 
	public Default
{
public:
	Character(SDL_Texture *img,
		       	EntityType id, 
			int layer, 
			int x,
		       	int y,
		       	int scale,
		       	const std::shared_ptr<RSS_Grid<Active>>& grid,
			const std::shared_ptr<DefaultWorld>& default_world):
		Active(img, id, layer, x, y, scale, grid),
		Default(default_world)
	{}

	std::weak_ptr<Gun> get_gun() {
		return gun_;
	}

	void set_gun(const std::weak_ptr<Gun>& gun) {
		gun_ = gun;
	}
private:
	std::weak_ptr<Gun> gun_;
};

