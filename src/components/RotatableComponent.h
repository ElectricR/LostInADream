#pragma once
#include "../Component.h"

class RotatableComponent:
    public Component
{
public:
    double get_rad_angle() const {
        return angle_rad_;
    }

    double get_deg_angle() const {
        return angle_deg_;
    }

private:
	double angle_deg_ = 0;
	double angle_rad_ = 0;
};
