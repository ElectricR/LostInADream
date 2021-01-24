#pragma once
#include <set>
#include <memory>
#include "Action.h"
#include "Events.h"

class DefaultActionHandler {
public:
	ActionStatus handle_actions(Events::Event **);

	template<class T>
	void emplace(const std::shared_ptr<T>& obj_ptr, 
			ActionStatus(T::*m_ptr)(Events::Event **), 
			ActionType prio) {
		action_set_.insert(std::make_unique<DefaultAction<T>>(obj_ptr, m_ptr, prio));
	}

private:
	std::multiset<std::unique_ptr<Action>, ActionComparator> action_set_;
	std::multiset<std::unique_ptr<Action>, ActionComparator>::iterator it_ = action_set_.begin();
};
