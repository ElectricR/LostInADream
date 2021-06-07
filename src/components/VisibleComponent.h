#pragma once
#include "../Component.h"
#include <SDL2/SDL.h>

class VisibleComponent:
    public Component
{
public:
    VisibleComponent(Entity *e, SDL_Texture *tex):
        Component(e, ComponentTypeId::VISIBLE_COMPONENT),
        tex(tex)
    {
        SDL_QueryTexture(tex, nullptr, nullptr, &image_w, &image_h);
    }

    SDL_Texture *tex = nullptr; 
    unsigned frame = 0;
	int image_w = 0;
	int image_h = 0;
};
