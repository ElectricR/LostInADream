#pragma once
#include "../Component.h"
#include "RealComponent.h"

class CameraComponent:
    public Component
{
public:
    CameraComponent():
        Component(nullptr, ComponentTypeId::CAMERA_COMPONENT)
    {}
    /* TODO
    CameraComponent(int &w, int &h):
        w(w),
        h(h)
    {}
    */
    
    double x = 0;
    double y = 0;
    int w = 1920;
    int h = 1080;
    RealComponent *target;
};
