#pragma once 
#include "Entity.h"
#include "ComponentList.h"

class Component {
public:
    Component(Entity *e, ComponentTypeId id):
        entity(e),
        id(id)
    {}

    Entity *entity;
    ComponentTypeId id;
};
