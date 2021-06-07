#pragma once
#include <unordered_set>
#include "../System.h"
#include "../components/CameraComponent.h"

class CameraSystem:
    public System
{
public:
    void update() override;

    void add_camera(CameraComponent &&v) {
        components.insert(std::move(v));
    }

private:
    std::unordered_set<CameraComponent> components;
};

