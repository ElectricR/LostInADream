#pragma once

class Entity;

namespace Events {

enum class EventType {
	NONE,
	END_OF_THE_LINE,
	BULLET_FIRED,
};
	
class Event {
public:
	virtual ~Event() = default;
	EventType type = EventType::NONE;
};

class EndOfTheLine:
	public Event
{
public:
	EndOfTheLine(Entity *e):
		entity(e)
	{
		type = EventType::END_OF_THE_LINE;
	}

	Entity *entity;
};
	


class BulletFired:
	public Event
{
public:
	BulletFired(double x, double y, double angle, double hspeed, double vspeed):
		x(x),
		y(y),
		angle(angle),
		hspeed(hspeed),
		vspeed(vspeed)
	{
		type = EventType::BULLET_FIRED;
	}

	double x;
	double y;
	double angle;
	double hspeed;
	double vspeed;
};

}
