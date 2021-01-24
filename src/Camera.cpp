#include "Camera.h"
#include <iostream>

SDL_Rect Camera::get_rect() {
	return SDL_Rect({ x, y, w, h });
}

void Camera::update_position() {
	std::shared_ptr<Entity> target = target_.lock();
	if (!target) {
		return;
	}
	x = static_cast<int>(target->get_x()) - w / 2 + 
		target->get_image_w() / 2 + 
		static_cast<int>(target->get_hspeed() * render_time_ratio_);
	y = static_cast<int>(target->get_y()) - h / 2 + 
		target->get_image_h() / 2 + 
		static_cast<int>(target->get_vspeed() * render_time_ratio_);
}
