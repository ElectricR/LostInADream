#pragma once
#include "Entity.h"
#include "Events.h"
#include <memory>
#include "RSS_Grid.h"
#include "EntityTypes.h"
#include "ActionStatus.h"

class Active:
	public Entity
{
public:
	Active(SDL_Texture *img, 
			EntityType id, 
			int layer, 
			int x, 
			int y, 
			int scale, 
			const std::shared_ptr<RSS_Grid<Active>>& grid):
		Entity(img, id, layer, x, y, scale),
		default_grid_(grid)
	{}

	void imprint_entity_to_grid();
	void remove_entity_from_grid();
	ActionStatus update_grid_position(Events::Event **);
	std::pair<std::pair<int, int>, std::pair<int, int>>  get_grid_cells();
	void establish_position();

	unsigned int get_jump_charges() {
		return jump_charges_;
	}

	void set_jump_charges(unsigned int value) {
		jump_charges_ = value;
	}

	void decrease_jump_charges() {
		--jump_charges_;
	}

	void restore_jump_charges() {
		jump_charges_ = max_jump_charges_;
	}

	void set_max_jump_charges(unsigned int value) {
		max_jump_charges_ = value;
	}

	unsigned int get_max_jump_charges() {
		return max_jump_charges_;
	}

	SDL_Rect get_collision_rect() {
		return SDL_Rect({ static_cast<int>(this->get_x()) + mask_rect_x_offset_,
				static_cast<int>(this->get_y()) + mask_rect_y_offset_,
				mask_rect_w_,
				mask_rect_h_ });
	}

	int get_mask_rect_x_offset_() {
		return mask_rect_x_offset_;
	}

	void set_mask_rect_x_offset_(int value) {
		mask_rect_x_offset_ = value;
	}

	int get_mask_rect_y_offset_() {
		return mask_rect_y_offset_;
	}

	void set_mask_rect_y_offset_(int value) {
		mask_rect_y_offset_ = value;
	}

	int get_mask_rect_w_() {
		return mask_rect_w_;
	}

	void set_mask_rect_w_(int value) {
		mask_rect_w_ = value;
	}

	int get_mask_rect_h_() {
		return mask_rect_h_;
	}

	void set_mask_rect_h_(int value) {
		mask_rect_h_ = value;
	}

	bool get_can_jump() {
		return can_jump_;
	}

	void set_can_jump(bool value) {
		can_jump_ = value;
	}

	std::weak_ptr<RSS_Grid<Active>> get_default_grid() {
		return default_grid_;
	}

private:
	double old_x_ = 0;
	double old_y_ = 0;
	unsigned int jump_charges_ = 0;
	unsigned int max_jump_charges_ = 1;
	bool can_jump_ = true;
	std::weak_ptr<RSS_Grid<Active>> default_grid_;
	int mask_rect_x_offset_ = 0;
	int mask_rect_y_offset_ = 0;
	int mask_rect_w_ = this->get_image_w();
	int mask_rect_h_ = this->get_image_h();
};
