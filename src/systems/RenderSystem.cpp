#include "RenderSystem.h"
#include "../components/RealComponent.h"
#include "../components/RotatableComponent.h"


void RenderSystem::update() {
    SDL_RenderClear(sdl_renderer);

    for (auto& component : components) {
        SDL_Rect frame_rect{ static_cast<int>(component->frame) * component->image_w, 
            0, component->image_w, component->image_h };
        SDL_Rect dest_rect{ static_cast<int>(std::static_pointer_cast<RealComponent>(component->entity->get_component(ComponentTypeId::VISIBLE_COMPONENT))->x - camera->x),
                static_cast<int>(std::static_pointer_cast<RealComponent>(component->entity->get_component(ComponentTypeId::VISIBLE_COMPONENT))->y - camera->y),
                component->image_w,
                component->image_h };
        if (component->entity->check_component(ComponentTypeId::ROTATABLE_COMPONENT)) {
            auto rotatable_component = std::static_pointer_cast<RotatableComponent>(component->entity->get_component(ComponentTypeId::ROTATABLE_COMPONENT));
            SDL_RenderCopyEx(sdl_renderer, component->tex, &frame_rect, &dest_rect, rotatable_component->get_deg_angle(), nullptr, SDL_FLIP_NONE);
        }
        else {
            SDL_RenderCopy(sdl_renderer, component->tex, &frame_rect, &dest_rect);
        }
    }
    SDL_RenderPresent(sdl_renderer);
}
