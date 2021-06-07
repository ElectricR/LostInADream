#pragma once
#include "../Component.h"
#include <unordered_map>
#include <cstdint>

class InputComponent:
    public Component
{
public:
    std::unordered_map<size_t, void(InputComponent::*)()> handlers;
};
