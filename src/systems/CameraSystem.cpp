#include "CameraSystem.h"

void CameraSystem::update() {
    for (auto& component : components) {
        component.x = static_cast<int>(component.target->x) - component.w / 2; 
        component.y = static_cast<int>(component.target->y) - component.h / 2; 
    }
}
