#include "DefaultActionHandler.h"
#include "Action.h"
#include "Events.h"

ActionStatus DefaultActionHandler::handle_actions(Events::Event **default_event) {
	for (; it_ != action_set_.end();) {
		ActionStatus ret_val = (*it_)->handle(default_event);
		switch(ret_val) {
		case ActionStatus::OK: {
			++it_;
			break;
		}
		case ActionStatus::ACTION_EXPIRED: {
			it_ = action_set_.erase(it_);
			break;
		}
		case ActionStatus::END_OF_THE_LINE: {
			it_ = action_set_.erase(it_);
			return ret_val;
		}
		default: {
			++it_;
			break;
		}
		}
	}
	it_ = action_set_.begin();
	return ActionStatus::OK;
}
