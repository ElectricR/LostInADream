#pragma once
#include <unordered_set>
#include "../System.h"
#include "../components/CollideComponent.h"
#include "../RSS_Grid.h"

class CollideSystem:
    public System
{
public:
    void update() override;

    void add(std::shared_ptr<CollideComponent> &&v) {
        grid.track(v);
        components.insert(std::move(v));
    }

private:
    std::unordered_set<std::shared_ptr<CollideComponent>> components;
    RSS_Grid<CollideComponent> grid {32};
};

