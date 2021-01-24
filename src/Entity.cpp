#include "Entity.h"
#include "Camera.h"

Entity::Entity(SDL_Texture* img, EntityType id, int layer, int x, int y, int scale):
	img_(img),
	id_(id),
	x_(x),
	y_(y),
	layer_(layer),
	scale_(scale)
{
	SDL_QueryTexture(img_, NULL, NULL, &image_w_, &image_h_);
}	

void Entity::render(SDL_Renderer *renderer, std::shared_ptr<Camera> c, double render_time_ratio) {
	SDL_Rect frame_rect{ static_cast<int>(frame_) * image_w_, 0, image_w_, image_h_ };
	SDL_Rect dest_rect{
			static_cast<int>(x_ + hspeed_ * render_time_ratio) - c->x,
			static_cast<int>(y_ + vspeed_ * render_time_ratio) - c->y,
			image_w_,
			image_h_ };
	if (angle_ == 0.) {
		SDL_RenderCopy(renderer, img_, &frame_rect, &dest_rect);
	}
	else {
		SDL_RenderCopyEx(renderer, img_, &frame_rect, &dest_rect, angle_, NULL, SDL_FLIP_NONE);
	}
}

void Entity::render(SDL_Renderer *renderer) {
	SDL_Rect frame_rect{ static_cast<int>(frame_) * image_w_, 0, image_w_, image_h_ };
	SDL_Rect dest_rect{ static_cast<int>(x_), static_cast<int>(y_), image_w_, image_h_ };
	if (angle_ == 0.) {
		SDL_RenderCopy(renderer, img_, &frame_rect, &dest_rect);
	}
	else {
		SDL_RenderCopyEx(renderer, img_, &frame_rect, &dest_rect, angle_, NULL, SDL_FLIP_NONE);
	}
}
