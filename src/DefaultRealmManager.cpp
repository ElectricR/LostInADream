#include "DefaultRealmManager.h"
#include "DefaultActionHandler.h"
#include "DefaultInputController.h"
#include "DefaultConstructor.h"
#include "Globals.h"
#include "Action.h"

#include "Camera.h"
#include "Platform.h"
#include "SpiderSlime.h"
#include "Player.h"
#include "StreetSign.h"
#include "GameAutomaton.h"
#include "SlapamoleGameAutomaton.h"
#include "DefaultWorld.h"

#include <fstream>
#include <string>
#include <sstream>


DefaultRealmManager::DefaultRealmManager(std::shared_ptr<Renderer> renderer, 
		std::shared_ptr<TextureController> texture_controller, 
		const double &render_time_ratio,
		const global::GameState *current_game_state, 
		const uint8_t *&current_key_states, 
		const uint32_t &current_mouse_states, 
		std::shared_ptr<Camera> &camera):
	BaseRealmManager(renderer, global::Realm::DEFAULT),
	default_input_controller_(std::make_unique<DefaultInputController>(current_game_state, 
				current_key_states,
				current_mouse_states)),
	default_action_handler_(std::make_shared<DefaultActionHandler>()),
	default_world_(std::make_shared<DefaultWorld>()),
	default_constructor_(std::make_unique<DefaultConstructor>(texture_controller, 
				default_world_,
				default_grid_)),
	camera_(camera)
{
	create_realm();
}

void DefaultRealmManager::create_realm() {
	std::ifstream InputFile(global::path_to_res_directory + "world/world");
	std::string line;
	while (getline(InputFile, line)) {
		std::istringstream iss(line);
		unsigned int id;
		int x;
		int y;
		iss >> id >> x >> y;
		switch (static_cast<EntityType>(id)) {
		case EntityType::BACKGROUND: {
			std::string img_name; 
			iss >> img_name;
			std::shared_ptr<Entity> e(default_constructor_->make_entity("background", x, y));
			subordinate_entities_.insert(e);
			renderer_->add(realm_, e);
			break;
		}
		case EntityType::PLAYER: {
			Player *t = default_constructor_->make_default_active<Player>("player", x, y);
			std::shared_ptr<Player> p(t); 
			subordinate_entities_.insert(p);
			renderer_->add(realm_, p);
			camera_->set_target(p);
			default_grid_->register_entity(p);
			p->imprint_entity_to_grid();
			default_action_handler_->emplace(p, 
					&Player::move_update, 
					ActionType::PLAYER_MOVE);
			default_action_handler_->emplace(p, 
					&Player::collide_update, 
					ActionType::PLAYER_COLLIDE);
			default_action_handler_->emplace(std::static_pointer_cast<Active>(p), 
					&Active::update_grid_position, 
					ActionType::PLAYER_UPDATE_GRID_POSITION);
			default_input_controller_->set_target(p);
			break; 
		}
		case EntityType::PLATFORM: {
			Platform *t = default_constructor_->make<Platform>("platform", x, y);
			std::shared_ptr<Active> e(t);
			subordinate_entities_.insert(e);
			renderer_->add(realm_, e);
			default_grid_->register_entity(e);
			e->imprint_entity_to_grid();
			break; 
		}
		case EntityType::SPIDERSLIME: {
			SpiderSlime *t = default_constructor_->make_default_active<SpiderSlime>(
					"spider_slime", x, y);
			std::shared_ptr<Entity> e(t);
			subordinate_entities_.insert(e);
			renderer_->add(realm_, e);
			break;
		}
		case EntityType::CHAIR: {
			std::shared_ptr<Entity> e(default_constructor_->make_entity("chair", x, y));
			subordinate_entities_.insert(e);
			renderer_->add(realm_, e);
			break;
		}
		case EntityType::UNITAZ: {
			std::shared_ptr<Entity> e(default_constructor_->make_entity("unitaz", x, y));
			subordinate_entities_.insert(e);
			renderer_->add(realm_, e);
			break;
		}
		case EntityType::SURGICAL_TABLE: {
			std::shared_ptr<Entity> e(default_constructor_->make_entity("surgical_table", x, y));
			subordinate_entities_.insert(e);
			renderer_->add(realm_, e);
			break;
		}
		case EntityType::METEOROID_GAME_AUTOMATON: {
			GameAutomaton *t = default_constructor_->make<GameAutomaton>(
					"meteoroid_game_automaton", x, y);
			std::shared_ptr<Entity> e(t);
			subordinate_entities_.insert(e);
			renderer_->add(realm_, e);
			break;
		}
		case EntityType::SLAPAMOLE_GAME_AUTOMATON: {
			SlapamoleGameAutomaton *t = 
				default_constructor_->make<SlapamoleGameAutomaton>(
						"slapamole_game_automaton", 
						x, 
						y);
			std::shared_ptr<Entity> e(t);
			subordinate_entities_.insert(e);
			renderer_->add(realm_, e);
			break;
		}
		default: {
			break;
		}
		}
	}
}


void DefaultRealmManager::manage_realm() {
	default_input_controller_->control();

	Events::Event *event = nullptr;
	ActionStatus ret_val;
	while (static_cast<bool>(ret_val = default_action_handler_->handle_actions(&event))) {
		switch(ret_val) {
		case ActionStatus::END_OF_THE_LINE: {
			subordinate_entities_.erase(
					std::shared_ptr<Entity>(
						static_cast<Events::EndOfTheLine*>(event)->entity,
						[](Entity*){}));
			break;
		}	    
		default: {
			break;
		}
		}	
		delete event;
	}
	camera_->update_position();
}
