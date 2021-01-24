#include "DefaultInputController.h"
#include "Globals.h"
#include "Player.h"
#include "DefaultIntentionFlagCollection.h"
#include <memory>

void DefaultInputController::set_target(const std::weak_ptr<Entity> &entity) {
	entity_ = entity;
	entity_type_ = entity.lock()->get_type_id();
	DIFC_ = std::make_shared<DefaultIntentionFlagCollection>();
	entity_.lock()->set_default_intention_flag_collection(DIFC_);
}

void DefaultInputController::player_control() {
	switch (*game_state_) {
	case global::GameState::DEFAULT: {
		if (current_key_states_[SDL_SCANCODE_D]) {
			DIFC_->move_right = true;
		}
		else {
			DIFC_->move_right = false;
		}

		if (current_key_states_[SDL_SCANCODE_A]) {
			DIFC_->move_left = true;
		}
		else {
			DIFC_->move_left = false;
		}

		if (current_key_states_[SDL_SCANCODE_SPACE]) {
			DIFC_->jump = true;
		}
		else {
			DIFC_->jump = false;
		}

		if (current_key_states_[SDL_SCANCODE_E]) {
			DIFC_->use = true;
		}
		else{
			DIFC_->use = false;
		}

		if (current_mouse_states_ & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			DIFC_->shoot = true;
		}
		else{
			DIFC_->shoot = false;
		}

		break;
	}
	}

}

void DefaultInputController::control() {
	switch (entity_type_)
	{
	case EntityType::PLAYER: {
		player_control();
		break;
	}
	default: {
		break;
	}
	}
}
