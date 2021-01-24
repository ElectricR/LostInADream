#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include <set>
#include "Camera.h"
#include "Globals.h"
#include "DefaultRealmManager.h"
#include "TextureController.h"
#include "Renderer.h"


int main(int argc, char* args[])
{
	SDL_Init(SDL_INIT_VIDEO);
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}

	SDL_Window* window = SDL_CreateWindow("123", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1920, 1080, SDL_WINDOW_SHOWN);
	SDL_Renderer* sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(sdl_renderer, 0x0, 0x0, 0x0, 0xFF);
	SDL_Event e;
	bool quitCalled = 0;
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

	int mouse_x = 0;
	int mouse_y = 0;
	const uint8_t *current_key_states = 0;
	uint32_t current_mouse_states = 0;

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
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quitCalled = true;
				}
				else if (e.type == SDL_KEYDOWN) {
					if (e.key.keysym.sym == SDLK_ESCAPE) {
						quitCalled = true;
					}
				}
			}

			/*Make sure to run check key state AFTER calling SDL_PollEvent*/
			current_key_states = SDL_GetKeyboardState(NULL);
			current_mouse_states = SDL_GetMouseState(&mouse_x, &mouse_y);

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

		if (quitCalled) {
			break;
		}
		
	end_time = SDL_GetTicks();
		
	if (end_time < planned_tick_time) {
			SDL_Delay(planned_tick_time - end_time);
		}
		
	planned_tick_time += frame_duration;
		
	}

	return 0;
}
