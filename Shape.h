#pragma once
namespace xtr {
enum class ShapeTypes :char{
	BEZIER_2D,
	SYMMETRICAL_POLYGON_2D,
	POLYGON_2D,
	BEZIER_3D,
	VOLUME_3D,
	UNDEFINED,
	NUM_TYPES
};
enum class Shape_Status : char{
	EMPTY,
	NEW,
	REPLACED,
	REMOVED,
	ACTIVE
};
struct Shape {
	Shape(ShapeTypes itype, unsigned int itype_index): type(itype), type_index(itype_index){}
	ShapeTypes type;
	Shape_Status status = Shape_Status::NEW;
	unsigned int type_index;
	float area = 1.0f;
	float volume = 1.0f;
	float sharpness = 1.0f;
	glm::vec2 pivot;
	unsigned int links = 1;
};
}
