#pragma once
#include "../Component.h"

class RealComponent:
    public Component
{
public:
    RealComponent(Entity *e):
        Component(e, ComponentTypeId::REAL_COMPONENT)
    {}

    double x = 0;
    double y = 0;
private:
};
