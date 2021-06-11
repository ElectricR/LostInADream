#pragma once
#include "../Component.h"

class PlayerComponent:
    public Component
{
public:
    PlayerComponent(Entity *e):
        Component(e, ComponentTypeId::PLAYER_COMPONENT)
    {}
};
