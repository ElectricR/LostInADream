#include "Player.h"
#include "Events.h"
#include "EntityTypes.h"
#include "DefaultIntentionFlagCollection.h"
#include "DefaultWorld.h"
#include "Gun.h"
#include <SDL2/SDL.h> //dsa

ActionStatus Player::move_update(Events::Event **event) {
	if (under_control_) {
		if (DIFC_->move_right) {
			this->shift_x(2);
		}

		if (DIFC_->move_left) {
			this->shift_x(-2);
		}

		if (DIFC_->jump) {
			if (this->get_can_jump()) {
				if (this->get_jump_charges() > 0) {
					this->set_vspeed(-30);
					this->decrease_jump_charges();
				}
				this->set_can_jump(false);
			}
		}
		else {
			if (!this->get_can_jump()) {
				this->set_can_jump(true);
			}
		}
	}
	this->add_vspeed(default_world_->gravity);
	this->shift_y(this->get_vspeed());
	return ActionStatus::OK;
}

ActionStatus Player::use_update(Events::Event **event) {
	if (under_control_) {
		if (DIFC_->use) {
			std::pair<std::pair<int, int>, std::pair<int, int>> grid_bounds = this->get_grid_cells();
			for (int i = grid_bounds.first.first; i != grid_bounds.first.second + 1; ++i) {
				for (int j = grid_bounds.second.first; j != grid_bounds.second.second + 1; ++j) {
					std::vector<std::weak_ptr<Active>>& cell_vector = 
						this->get_default_grid().lock()->get_cell(i, j);
					for (std::vector<std::weak_ptr<Active>>::iterator 
							it = cell_vector.begin(); 
							it != cell_vector.end(); 
							++it) {
						EntityType cell_entity_type = (*it).lock()->get_type_id();
						switch (cell_entity_type)
						{
						case EntityType::METEOROID_GAME_AUTOMATON:
						{
							//init_meteoroids(this->get_executive_collection());
							return ActionStatus::OK;
						}
						case EntityType::SLAPAMOLE_GAME_AUTOMATON: {
							//cout << 123;
							//this->is_using = false;
							return ActionStatus::OK;
						}
						default: {
							break;
						}
						}
					}
				}
			}
		}
	}
}

ActionStatus Player::shoot_update(Events::Event **event) {
	if (under_control_) {
		if (DIFC_->shoot) {
			if (!this->get_gun().expired()) {
				return this->get_gun().lock()->shoot(event);
			}
		}
	}
	return ActionStatus::OK;
}

bool inelastic_collision(Active* weak_active, const std::shared_ptr<Active>& strong_active){
	SDL_Rect collision_rect;
	SDL_Rect A = weak_active->get_collision_rect();
	SDL_Rect B = strong_active->get_collision_rect();
	if (SDL_IntersectRect(&A, &B, &collision_rect)) {
		if (collision_rect.w >= collision_rect.h) {
			if (weak_active->get_center_y() < strong_active->get_center_y()) {
				weak_active->shift_y(-collision_rect.h);
				weak_active->restore_jump_charges();
				weak_active->set_vspeed(0);
			}
			else {
				weak_active->shift_y(collision_rect.h);
			}
		}
		else {
			if (weak_active->get_center_x() < strong_active->get_center_x()) {
				weak_active->shift_x(-collision_rect.w);
			}
			else {
				weak_active->shift_x(collision_rect.w);
			}
		}
		return true;
	}
	return false;
}

ActionStatus Player::collide_update(Events::Event **event) {
	std::pair<std::pair<int, int>, std::pair<int, int>> grid_bounds = this->get_grid_cells();
	for (int i = grid_bounds.first.first; i != grid_bounds.first.second + 1; ++i) {
		for (int j = grid_bounds.second.first; j != grid_bounds.second.second + 1; ++j) {
			std::vector<std::weak_ptr<Active>>& cell = 
				this->get_default_grid().lock()->get_cell(i, j);
			for (std::vector<std::weak_ptr<Active>>::iterator it = cell.begin(); 
					it != cell.end(); 
					++it) 
			{
				EntityType entity_id = (*it).lock()->get_type_id();
				switch(entity_id){
				case EntityType::PLATFORM: {
					if (inelastic_collision(this, (*it).lock())) {
						this->update_grid_position(event);
						return ActionStatus::COLLIDED;
					}
				} 
				default: {
					break;
				}
				}
			}

		}
	}
	return ActionStatus::OK;
}
