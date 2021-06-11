#pragma once
#include "../Component.h"

class MassComponent:
    public Component
{
public:
    MassComponent(Entity *e):
        Component(e, ComponentTypeId::MASS_COMPONENT)
    {}
};
