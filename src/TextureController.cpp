#include "TextureController.h"
#include <iostream>
#include <SDL2/SDL_image.h>

SDL_Texture* TextureController::load_texture(const std::string& img_name) {
    //BEGIN TODO
    int scale = 1;
    //END TODO

	if (!texture_map_.count(img_name)) {
		SDL_Surface *img_pre = IMG_Load(("res/" + img_name + ".png").c_str());
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

		SDL_Texture *tex = SDL_CreateTextureFromSurface(sdl_renderer_, srf);

		SDL_FreeSurface(img_pre);
		SDL_FreeSurface(srf);
		texture_map_[img_name] = tex;
		
		return tex;
	}
	else {
		return texture_map_[img_name];
	}
}
