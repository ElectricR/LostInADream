#pragma once
#include "Character.h"
#include "Events.h"


class Player :
	public Character
{
public:
	using Character::Character;

	ActionStatus move_update(Events::Event **);
	ActionStatus shoot_update(Events::Event **);
	ActionStatus use_update(Events::Event **);
	ActionStatus collide_update(Events::Event **);
};

