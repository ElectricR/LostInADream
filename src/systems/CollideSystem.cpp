#include "CollideSystem.h"
#include "../components/RealComponent.h"

#include <iostream>
void CollideSystem::update() {
    for (auto& component : components) {
        grid.update(component);
    }

    for (auto& component : components) {
        if (component->entity->check_component(ComponentTypeId::PLAYER_COMPONENT)) {
            for (auto& [target, collision_rect] : grid.get_collisions(component)) {
                std::static_pointer_cast<RealComponent>(component->entity->get_component(ComponentTypeId::REAL_COMPONENT))->y -= 1;
            }
        }
    }
}

        
