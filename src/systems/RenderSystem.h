#pragma once
#include <unordered_set>
#include <SDL2/SDL.h>
#include "../components/VisibleComponent.h"
#include "../components/CameraComponent.h"
#include "../System.h"

class RenderSystem:
    public System
{
public:
    
    RenderSystem(SDL_Renderer *sdl_renderer, const std::shared_ptr<CameraComponent>& c):
        sdl_renderer(sdl_renderer),
        camera(c)
    {}

    void update() override;

    void add(std::shared_ptr<VisibleComponent>&& v) {
        components.insert(std::move(v));
    }
    
private:
    std::unordered_set<std::shared_ptr<VisibleComponent>> components;
    SDL_Renderer *sdl_renderer;
    std::shared_ptr<CameraComponent> camera;
};
