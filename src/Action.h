#pragma once
#include <memory>
#include "Events.h"
#include "ActionStatus.h"
#include "ActionList.h"

class Action{
public:
	Action(ActionType prio):
		priority_(prio)
	{}
	
	virtual ~Action() = default;

	virtual ActionStatus handle(Events::Event **) = 0;

	ActionType get_priority() {
		return priority_;
	}

private:
	ActionType priority_;
};

template<class T>
class DefaultAction:
	public Action
{
public:
	DefaultAction(const std::weak_ptr<T>& obj_ptr, 
			ActionStatus(T::*m_ptr)(Events::Event **), 
			ActionType prio):
		Action(prio),
		obj_ptr_(obj_ptr),
		m_ptr_(m_ptr)
	{}

	ActionStatus handle(Events::Event **event) override {
		std::shared_ptr<T> ptr = obj_ptr_.lock();
		if (!ptr) {
			return ActionStatus::ACTION_EXPIRED;
		}
		else {
			return (ptr.get()->*m_ptr_)(event);
		}
	}
private:
	std::weak_ptr<T> obj_ptr_;
	ActionStatus(T::*m_ptr_)(Events::Event **);
};

template<class T>
std::unique_ptr<Action> make_action(const std::weak_ptr<T>& ptr, 
		ActionStatus(T::*m_ptr)(Events::Event **), 
		ActionType priority)
{
	DefaultAction<T> DAH(ptr, m_ptr, priority);
	return std::unique_ptr<Action>(&DAH);
}

class ActionComparator {
public:
	bool operator()(const std::unique_ptr<Action>& a, const std::unique_ptr<Action>& b) const {
		return a->get_priority() < b->get_priority();
	}
};
