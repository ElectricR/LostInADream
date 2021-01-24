#pragma once
#include <utility>

namespace utils {

std::pair<double, double> normalized_2d_vector(const std::pair<double, double> &pre_vector);
std::pair<double, double> rotate_2d_vector(const std::pair<double, double> &pre_vector, 
		const std::pair<double, double> &point, 
		double angle_r);
}
