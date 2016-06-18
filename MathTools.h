#pragma once
#include <glm/glm.hpp>
#include <cmath>

namespace xtr {
	// This template clamps a numerical value within a specified low and high range
	template<typename numericalType>
	numericalType valClamp(const numericalType low, const numericalType high, const numericalType test){
		return test < low ? low : test > high ? high : test;
	}
	inline glm::vec3 vec3MagnitudeClamp(glm::vec3 vector, float max_magnitude) {
		float magnitude = glm::length(vector);
		if (magnitude > max_magnitude)
			return vector / magnitude * max_magnitude;
		else
			return vector;
	}
	template<typename valueType>
	valueType interpolateBiLin(const valueType& c_1, const valueType& c_2, const valueType& c_3, const valueType& c_4, const float x, const float y){
		return c_1*(1.0f - x)*(1.0f - y) + c_2*x*(1.0f - y) + c_3 * (1.0f - x)*y + c_4 *x*y;
	}
	inline glm::vec3 axisPlaneIntersection(const glm::vec3 line_point, const glm::vec3 normal, const glm::vec3 plane_point) {
		auto P2_P0 = plane_point - line_point;
		glm::vec3 P1_P0 = { 0, 0, -1000.0f };
		return line_point + (glm::dot(normal, P2_P0) / glm::dot(normal, P1_P0)) * P1_P0;
	}
	inline glm::vec2 makeVEC2(const float angle, const float amplitute) {
		return glm::vec2(amplitute* std::cos(angle), amplitute* std::sin(angle));
	}
}