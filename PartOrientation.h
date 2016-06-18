#pragma once
#include <tuple>
namespace xtr {
struct PartOrientation {
	PartOrientation() {}
	PartOrientation(const float ipart_angle, const float iposition_angle, const float idisplacement) : part_angle(ipart_angle), position_angle(iposition_angle), displacement(idisplacement){}	
	bool operator<(const PartOrientation& other) const
	{
		return std::tie(part_angle, position_angle, displacement)
			< std::tie(other.part_angle, other.position_angle, other.displacement);
	}
	float part_angle = 0.0f;
	float position_angle = 0.0f;
	float displacement = 0.0f;
};
}
