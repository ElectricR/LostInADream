#pragma once
#include "BaseRealmManager.h"
#include <memory>
#include "RSS_Grid.h"
#include <unordered_set>
#include "DefaultInputController.h"
#include "DefaultConstructor.h"

class GlobalExecutiveCollection;
class DefaultActionHandler;
class TextureController;
class Camera;
class DefaultWorld;
class Entity;

class DefaultRealmManager :
	public BaseRealmManager
{
public:

	DefaultRealmManager(std::shared_ptr<Renderer> renderer, 
			std::shared_ptr<TextureController> texture_controller, 
			const double &render_time_ratio,
			const global::GameState *current_game_state, 
			const uint8_t *&current_key_states, 
			const uint32_t &current_mouse_states,
			std::shared_ptr<Camera> &camera);

	void manage_realm() override;

	void create_realm() override;

	void destroy_realm() override {}
private:
	std::unordered_set<std::shared_ptr<Entity>> subordinate_entities_;
	std::unique_ptr<DefaultInputController> default_input_controller_ = nullptr;
	std::shared_ptr<DefaultActionHandler> default_action_handler_; 
	std::shared_ptr<DefaultWorld> default_world_;
	std::shared_ptr<RSS_Grid<Active>> default_grid_ = std::make_shared<RSS_Grid<Active>>(30, RSS_Grid<Active>::RSS_GridType::DEFAULT);
	std::unique_ptr<DefaultConstructor> default_constructor_;
	std::shared_ptr<Camera> camera_;
};

