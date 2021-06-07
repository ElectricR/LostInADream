#pragma once
#include <unordered_map>
#include <memory>
#include "ComponentList.h"

class Component;

class Entity {
public:
    std::shared_ptr<Component> get_component(ComponentTypeId id) {
        return components[id];
    }

    bool check_component(ComponentTypeId id) {
        return components.contains(id);
    }

    void add_component(const std::shared_ptr<Component>& c);

private:
    std::unordered_map<ComponentTypeId, std::shared_ptr<Component>> components;
};
