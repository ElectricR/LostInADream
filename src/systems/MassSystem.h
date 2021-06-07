#pragma once
#include <unordered_set>
#include "../System.h"
#include "../components/MassComponent.h"

class MassSystem:
    public System
{
public:
    void update() override;

    void add(std::shared_ptr<MassComponent> &&v) {
        components.insert(std::move(v));
    }

private:
    std::unordered_set<std::shared_ptr<MassComponent>> components;
};

