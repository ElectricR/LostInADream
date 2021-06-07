#pragma once
#include <string>
#include <unordered_map>
#include <SDL2/SDL.h>

class TextureController {
public:
	TextureController(SDL_Renderer *sdl_renderer):
		sdl_renderer_(sdl_renderer)
	{}

	SDL_Texture *load_texture(const std::string& img_name);

private:
	SDL_Renderer *sdl_renderer_;
	std::unordered_map<std::string, SDL_Texture*> texture_map_;
};

