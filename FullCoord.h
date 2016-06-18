#pragma once
#include "SimpleCoord.h"
namespace xtr {
	struct FullCoord : public SimpleCoord{
		FullCoord() : rotation(0.0f, 0.0f, 0.0f) {}
		FullCoord(const SimpleCoord& ancentors) : SimpleCoord(ancentors) {}
		FullCoord(const FullCoord& sibling) : SimpleCoord(sibling), rotation(sibling.rotation) {}
		FullCoord(glm::vec3 the_position, float the_scale, glm::vec3 the_rotation) : SimpleCoord(the_position, the_scale), rotation(the_rotation) {}
		glm::vec3 rotation;
		glm::mat4 getTransformationMatrix(){
			// code for calculating model Matrix
			auto&& modelMatrix = glm::translate(glm::mat4(1.0f), position);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(scale)); // uniform scale will not effect the normal matrix
			modelMatrix = glm::rotate(modelMatrix, rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));
			return modelMatrix;
		}
		void fillTransformationMatrix(glm::mat4& modelMatrix){
			// code for filling a model Matrix
			modelMatrix = glm::translate(glm::mat4(1.0f), position);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(scale)); // uniform scale will not effect the normal matrix
			modelMatrix = glm::rotate(modelMatrix, rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));
		}
	};
}
