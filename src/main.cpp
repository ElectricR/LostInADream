#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "System.h"
#include <iostream>

//TODO
#include <vector>
#include "TextureController.h"
#include "systems/RenderSystem.h"
#include "systems/MassSystem.h"
#include "components/CameraComponent.h"


int main() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Error in SDL initialization: " << SDL_GetError() << std::endl;
    }

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "Error in IMG initialization: " << SDL_GetError() << std::endl;
    }

	SDL_Window* window = SDL_CreateWindow("123", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "Error in window creation: " << SDL_GetError() << std::endl;
    }

	SDL_Renderer* sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(sdl_renderer, 0x0, 0x0, 0x0, 0xFF);
	SDL_Event sdl_event;

	bool quit_called = false;

    // BEGIN TODO
    std::vector<System*> systems; 

    auto camera = std::make_shared<CameraComponent>();
    auto player = std::make_shared<Entity>();
    player->add_component(std::make_shared<RealComponent>(player.get()));
    auto texturer = new TextureController(sdl_renderer);
    auto player_tex = texturer->load_texture("player");
    auto pl_vis = std::make_shared<VisibleComponent>(player.get(), player_tex);
    auto pl_mas = std::make_shared<MassComponent>(player.get());
    player->add_component(pl_vis);
    player->add_component(pl_mas);
    auto rs = new RenderSystem(sdl_renderer, camera);
    rs->add(std::move(pl_vis));
    auto ms = new MassSystem();
    ms->add(std::move(pl_mas));
    systems.push_back(rs);
    systems.push_back(ms);

    SDL_RenderClear(sdl_renderer);
    // END TODO

    while (true) {

        //Brute-force quit. TODO
        while (SDL_PollEvent(&sdl_event) != 0) {
            if (sdl_event.type == SDL_QUIT) {
                quit_called = true;
            }
            else if (sdl_event.type == SDL_KEYDOWN) {
                if (sdl_event.key.keysym.sym == SDLK_ESCAPE) {
                    quit_called = true;
                }
            }
        }

        for (auto &system : systems) {
            system->update();
        }

		if (quit_called) {
			break;
		}
		
    }
    /*
	SDL_RenderClear(sdl_renderer);////////////////////////////////////?

	int max_fps = 240;
	int update_rate = 240;
	double frame_duration = 1. / max_fps * 1000;
	double update_duration = 1. / update_rate * 1000;
	double planned_tick_time = 0.0;
	double lag = 0.0;
	uint32_t start_time = SDL_GetTicks();
	uint32_t end_time = SDL_GetTicks();
	double render_time_ratio = 0;
	uint32_t framePerSecondStartTime = SDL_GetTicks();
	int frameCount = 0;

	global::GameState current_game_state = global::GameState::DEFAULT;

	std::shared_ptr<Camera> camera = std::make_shared<Camera>(render_time_ratio);
	std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>(
			sdl_renderer, 
			camera, 
			render_time_ratio);

	std::shared_ptr<TextureController> texture_controller = 
		std::make_shared<TextureController>(sdl_renderer);

	std::set<std::shared_ptr<BaseRealmManager>> managers({ 
			std::make_shared<DefaultRealmManager>(
				renderer,
				texture_controller,
				render_time_ratio,
				&current_game_state,
				current_key_states,
				current_mouse_states,
				camera) });

	while (true) {
		uint32_t elapsed = end_time - start_time;
		start_time = end_time;
		lag += elapsed;
		while (lag>= update_duration) {

			for (std::set<std::shared_ptr<BaseRealmManager>>::iterator 
					it = managers.begin(); 
					it != managers.end(); 
					++it){
				(*it)->manage_realm();
			}	

			lag -= update_duration;
		}

		render_time_ratio = lag/update_duration;

		if (max_fps <= update_rate) {
			render_time_ratio = 0;
		}

		renderer->render();

		++frameCount;

		if (SDL_GetTicks() - framePerSecondStartTime > 1000) {
			// std::cout <<"FPS: "<< frameCount << std::endl;
			frameCount = 0;
			framePerSecondStartTime = SDL_GetTicks();
		}

	end_time = SDL_GetTicks();
		
	if (end_time < planned_tick_time) {
			SDL_Delay(planned_tick_time - end_time);
		}
		
	planned_tick_time += frame_duration;
		
	}

	return 0;
    */
}
