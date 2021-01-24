#include "Gun.h"
#include "Character.h"
#include "Globals.h"
#include "Camera.h"
#include <utility>
#include <cmath>
#include "2d_vector_math.h"
#include "Events.h"


ActionStatus Gun::move_update(Events::Event **) {
	std::shared_ptr<Character> parent = parent_.lock();
	if (!parent){
		return ActionStatus::END_OF_THE_LINE;
	}

	double angle_r = std::atan2(mouse_y_ - 
			static_cast<int>(parent->get_center_y()) + 
			camera_->y, 
			mouse_x_ - 
			static_cast<int>(parent->get_center_x()) + 
			camera_->x);
	this->set_angle(angle_r * 180 / global::PI);
	this->set_x(parent->get_center_x() + 100 * std::cos(angle_r));
	this->set_y(parent->get_center_y() + 100 * std::sin(angle_r));
	return ActionStatus::OK;
}

ActionStatus Gun::shoot(Events::Event **event) {
	if (current_time_ > shot_last_time_ + 200) {
		std::pair<double, double> bullet_velocity = 
			utils::normalized_2d_vector(std::pair<double, double>(
						camera_->x + mouse_x_ - this->get_x(), 
						 camera_->y + mouse_y_ - this->get_y()));
		*event = new Events::BulletFired(this->get_x(), 
				this->get_y(), 
				this->get_angle(), 
				bullet_velocity.first, 
				bullet_velocity.second);
		return ActionStatus::BULLET_FIRED;
	}
	else {
		return ActionStatus::OK;
	}	
}
