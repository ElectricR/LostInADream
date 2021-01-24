#pragma once
#include "Globals.h"
#include "EntityTypes.h"
#include <memory>

class Entity;
struct DefaultIntentionFlagCollection;

class DefaultInputController {
public:
	DefaultInputController(
			const global::GameState *game_state, 
			const uint8_t *&current_key_states, 
			const uint32_t &current_mouse_states): 
		game_state_(game_state),
		current_key_states_(current_key_states),
		current_mouse_states_(current_mouse_states)
	{}

	void control();
	
	void set_target(const std::weak_ptr<Entity> &entity);

private:
	void player_control();

	std::weak_ptr<Entity> entity_;
	EntityType entity_type_ = EntityType::NONE;
	const global::GameState *game_state_;
	const uint8_t *&current_key_states_;
	const uint32_t &current_mouse_states_;
	std::shared_ptr<DefaultIntentionFlagCollection> DIFC_;
};

