#pragma once
#include "Active.h"
#include "Default.h"
#include "Events.h"

class SpiderSlime:
	public Active, Default
{
public:
	SpiderSlime(SDL_Texture *img, 
			EntityType id, 
			int layer, 
			int x, 
			int y, 
			int scale, 
			std::shared_ptr<RSS_Grid<Active>> grid, 
			const std::shared_ptr<DefaultWorld>& default_world):
		Active(img, id, layer, x, y, scale, grid),
		Default(default_world)
	{}

	ActionStatus move_update(Events::Event **);
};

