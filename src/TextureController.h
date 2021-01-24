#pragma once
#include <string>
#include <unordered_map>
#include <SDL2/SDL.h>

class TextureController {
public:
	TextureController(SDL_Renderer *renderer):
		renderer_(renderer)
	{}

	SDL_Texture *load_texture(const std::string& img_name, int scale);

private:
	SDL_Renderer *renderer_;
	std::unordered_map<std::string, SDL_Texture*> texture_map_;
};

