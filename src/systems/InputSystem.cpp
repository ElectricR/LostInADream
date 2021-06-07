#include "InputSystem.h"
#include <SDL2/SDL.h>

void InputSystem::update() {
    current_key_states_ = SDL_GetKeyboardState(nullptr);
    current_mouse_states_ = SDL_GetMouseState(&mouse_x_, &mouse_y_);

    for (auto& component : components) {
        for (auto& [key, handler] : component.handlers) {
            if (current_key_states_[key]) {
                component.*handler();
            }
        }
    }
}

