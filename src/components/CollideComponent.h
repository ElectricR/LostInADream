#pragma once
#include "../Component.h"
#include "RealComponent.h"

class CollideComponent:
    public Component
{
public:
    CollideComponent(Entity *e, int width, int height):
        Component(e, ComponentTypeId::COLLIDE_COMPONENT),
        width(width),
        height(height)
    {
        this->update();
        this->establish();
    }

    void update() {
        start_x = std::static_pointer_cast<RealComponent>(entity->get_component(ComponentTypeId::REAL_COMPONENT))->x;
        start_y = std::static_pointer_cast<RealComponent>(entity->get_component(ComponentTypeId::REAL_COMPONENT))->y;
    }

    void establish() {
        old_start_x = start_x;
        old_start_y = start_y;
    }

    double start_x;
    double start_y;
    int width;
    int height;
    double old_start_x;
    double old_start_y;
};
