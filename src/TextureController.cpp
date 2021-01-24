#include "TextureController.h"
#include <iostream>
#include <SDL2/SDL_image.h>
#include "Globals.h"

SDL_Texture* TextureController::load_texture(const std::string& img_name, int scale) {

	if (!texture_map_.count(img_name)) {
		SDL_Surface *img_pre = IMG_Load((global::path_to_res_directory + img_name + ".png").c_str());
		SDL_Surface *srf = SDL_CreateRGBSurface(img_pre->flags, 
				img_pre->w * scale, 
				img_pre->h * scale, 
				img_pre->format->BitsPerPixel, 
				img_pre->format->Rmask, 
				img_pre->format->Gmask, 
				img_pre->format->Bmask, 
				img_pre->format->Amask);
		
		SDL_Rect src_rect = SDL_Rect({ 0, 0, img_pre->w, img_pre->h }); 
		SDL_Rect dest_rect = SDL_Rect({ 0, 0, img_pre->w * scale, img_pre->h * scale });
		
		SDL_BlitScaled(img_pre, &src_rect, srf,	&dest_rect);

		SDL_Texture *img = SDL_CreateTextureFromSurface(renderer_, srf);

		SDL_FreeSurface(img_pre);
		SDL_FreeSurface(srf);
		texture_map_[img_name] = img;
		
		return img;
	}
	else {
		return texture_map_[img_name];
	}
}
