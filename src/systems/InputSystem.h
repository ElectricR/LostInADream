#pragma once
#include <cstdint>
#include <unordered_set>
#include "../System.h"
#include "../components/InputComponent.h"

class InputSystem:
    public System
{
public:
    void update() override;
private:
    std::unordered_set<InputComponent> components;

	int mouse_x_ = 0;
	int mouse_y_ = 0;
	const uint8_t *current_key_states_ = 0;
	uint32_t current_mouse_states_ = 0;
};
