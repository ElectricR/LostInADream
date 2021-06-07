#include "Entity.h"
#include "Component.h"


void Entity::add_component(const std::shared_ptr<Component>& c) {
    components.insert({c->id, c});
}
