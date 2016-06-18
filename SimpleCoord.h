#pragma once
#include <glm/glm.hpp>
namespace xtr {
	struct SimpleCoord{
		SimpleCoord() : position(0.0f, 0.0f, 0.0f), scale(1.0f) {}
		SimpleCoord(glm::vec3 the_position, float the_scale) : position(the_position), scale(the_scale) {}
		glm::vec3 position;
		float scale;
	};
}
