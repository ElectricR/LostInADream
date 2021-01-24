#pragma once
#include <memory>
#include <string>
#include "RSS_Grid.h"
#include <SDL2/SDL.h>
#include "TextureController.h"
#include "EntityTypes.h"

class Entity;
class DefaultWorld;
class Active;

class DefaultConstructor {
public:
	DefaultConstructor(const std::shared_ptr<TextureController>& tex_ptr, 
			const std::shared_ptr<DefaultWorld>& default_world,
			const std::shared_ptr<RSS_Grid<Active>> &default_grid):
		tex_ctrl_(tex_ptr),
		default_world_(default_world),
		default_grid_(default_grid)
	{}


	template<class T>
	T *make(const std::string &name, int x, int y) {
		load_attrs(name);
		SDL_Texture *img = tex_ctrl_->load_texture(name, scale_); 
		return new T(img, id_, layer_, x, y, scale_, default_grid_);
	}
	
	template<class T>
	T *make_default_active (const std::string &name, int x, int y) {
		load_attrs(name);
		SDL_Texture *img = tex_ctrl_->load_texture(name, scale_); 
		return new T(img, id_, layer_, x, y, scale_, default_grid_, default_world_);
	}

	Entity *make_entity(const std::string &, int x, int y);

private:
	void load_attrs(const std::string &name); 

	std::shared_ptr<TextureController> tex_ctrl_;
	std::shared_ptr<DefaultWorld> default_world_;
	std::shared_ptr<RSS_Grid<Active>> default_grid_;

	EntityType id_ = EntityType::NONE;
	int layer_ = 0;
	int scale_ = 0;
};
