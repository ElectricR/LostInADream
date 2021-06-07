#include "MassSystem.h"
#include "../components/RealComponent.h"

void MassSystem::update() {
    for (auto& component : components) {
        std::static_pointer_cast<RealComponent>(component->entity->get_component(ComponentTypeId::REAL_COMPONENT))->y += 1;
    }
}

        
