#pragma once
#include <cstddef>

enum class ComponentTypeId : size_t {
    VISIBLE_COMPONENT,
    REAL_COMPONENT,
    CAMERA_COMPONENT,
    ROTATABLE_COMPONENT,
    MOVEABLE_COMPONENT,
    MASS_COMPONENT,
    COLLIDE_COMPONENT,
    PLAYER_COMPONENT
};

