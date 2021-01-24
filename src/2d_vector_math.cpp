#include "2d_vector_math.h"
#include <cmath>

std::pair<double, double> utils::normalized_2d_vector(const std::pair<double, double> &pre_vector) {
	double length = std::sqrt(pre_vector.first * pre_vector.first + 
			pre_vector.second * pre_vector.second);
	return std::pair<double, double>(pre_vector.first / length, pre_vector.second / length);
}

std::pair<double, double> utils::rotate_2d_vector(const std::pair<double, double> &pre_vector, 
		const std::pair<double, double> &point,
		double angle_r) {
	double cos_a = std::cos(angle_r);
	double sin_a = std::sin(angle_r);
	return std::pair<double, double>((pre_vector.first - point.first) * cos_a - 
			(pre_vector.second - point.second) * sin_a, 
			(pre_vector.first - point.first) * sin_a + 
			(pre_vector.second - point.second) * cos_a);
}
