#include "SpiderSlime.h"
#include "DefaultWorld.h"
#include "Events.h"

ActionStatus SpiderSlime::move_update(Events::Event **event) {
	this->add_vspeed(default_world_->gravity);
	this->shift_y(this->get_vspeed());
	return ActionStatus::OK;
}
