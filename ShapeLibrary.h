#pragma once
#include <boost/container/flat_map.hpp>
#include "BasicPair.h"
#include "IDStack.h"
#include "MultithreadingDefines.h"
#include "PartOrientation.h"
#include "Shape.h"
#include "SymmetryType.h"
#include "glm/glm.hpp"

namespace xtr{
const int pattern_size = 256;
typedef std::array<std::bitset<pattern_size>, pattern_size> shape_pattern_2D;
class ShapeLink;
struct PointNHandles{
	glm::vec2 handle_1;
	glm::vec2 point;
	glm::vec2 handle_2;
};
struct Point{
  Point() {}
	Point(const float x, const float y, const float c = 0.5f) : coordinate(x, y), curvature(c){}
	bool operator<(const Point& other) const{
		return std::tie(coordinate.x, coordinate.y, curvature)
			< std::tie(other.coordinate.x, other.coordinate.y, other.curvature);
	}
	glm::vec2 coordinate;
	float curvature;
};

struct ShapeSpecification{
	std::vector<Point> points;
	glm::vec2 pivot = {0.0f, 0.0f};
	glm::vec2 orientation = {1.0f, 1.0f};
	float thickness = 4.0f;
	int symmetry = 1;
	SymmetryType sym_type = SymmetryType::ROTATIONAL;
	bool operator<(const ShapeSpecification& other) const{
		return std::tie(points, pivot.x, pivot.y, orientation.x, orientation.y, thickness, symmetry, sym_type)
			< std::tie(other.points, other.pivot.x, pivot.y, other.orientation.x, other.orientation.y, other.thickness, other.symmetry, other.sym_type);
	}
	inline void addPoint(const float x, const float y, const float c = 0.5f){ points.push_back({x, y, c}); }
  void resizePoints(const size_t n) { points.resize(n); }
	inline void setSymmetry(const SymmetryType type){ sym_type = type; }
	inline void setThickness(const float t){ thickness = t; }
	inline void setSymmetry(const int s){ symmetry = s; }
};
struct Bezier2D{
	shape_pattern_2D pattern;
	float thickness = 1.0f;
	float length = 0.0f;
	unsigned int texture_id = 0;
	std::vector<PointNHandles> points_n_handles;
};
class ShapeLibrary{
public:
	static ShapeLibrary& getInstance();
	ShapeLink drawSymeticalPolygon2D(const int symmetry, const float extension, const float sharpness, const float thickness = 1.0f);
	ShapeLink drawPolygon2D(const ShapeSpecification&& specification);
	const shape_pattern_2D getPattern_2D(unsigned int id);
	ShapeLink getIDForUndefined(const std::string& shape_tag);
	std::string getNameForUndefined(const unsigned int shape_id);
	const Shape getShape(const unsigned int shape_id);
	const Shape getShapeActivate(const unsigned int shape_id);
	std::vector<std::pair<glm::vec2, float>> findSurfaceTracedNormals(const float proportion, const unsigned int bezier_id, const int symmetry = 1);
	PartOrientation findFirstSymmetricNormal(const float proportion, const ShapeLink& shape, const int symmetry = 1);
	void delinkShape(const unsigned int shape_id){
		ScopedLock a_lock(shape_mutex);
		assert(!shapes.isFree(shape_id));
		auto&& shape = shapes[shape_id];
		if(shape.links <= 1){
			switch(shape.type){
			case ShapeTypes::BEZIER_2D:
				shape_patterns_2D.remove(shape.type_index);
				break;
			case ShapeTypes::SYMMETRICAL_POLYGON_2D:
				shape_patterns_2D.remove(shape.type_index);
				for(auto&& iter = symmetrical_polygon_2d_map.begin(); iter != symmetrical_polygon_2d_map.end(); ++iter){
					if(iter->second == shape_id){
						symmetrical_polygon_2d_map.erase(iter);
						break;
					}
				}
				break;
			case ShapeTypes::POLYGON_2D:
				shape_patterns_2D.remove(shape.type_index);
				for(auto&& iter = symmetrical_polygon_2d_map.begin(); iter != symmetrical_polygon_2d_map.end(); ++iter){
					if(iter->second == shape_id){
						symmetrical_polygon_2d_map.erase(iter);
						break;
					}
				}
				break;
			case ShapeTypes::UNDEFINED:
				for(auto&& iter = undefined_map.begin(); iter != undefined_map.end(); ++iter){
					if(iter->second == shape_id){
						undefined_map.erase(iter);
						break;
					}
				}
				break;
			case ShapeTypes::BEZIER_3D:
			case ShapeTypes::VOLUME_3D:
			case ShapeTypes::NUM_TYPES:
				break;
			}
			shapes.remove(shape_id);
		} else --shape.links;
	}
	void linkShape(const unsigned int shape_id){
		ScopedLock a_lock(shape_mutex);
		assert(!shapes.isFree(shape_id));
		++shapes[shape_id].links;
	}
	void setStatus(const unsigned int shape_id, const Shape_Status status){
		ScopedLock a_lock(shape_mutex);
		shapes[shape_id].status = status;
	}
private:
	ShapeLibrary();
	~ShapeLibrary();
	ShapeLibrary(ShapeLibrary const&); // Don't Implement
	void operator=(ShapeLibrary const&); // Don't implement
	bool findNormalAtAngle2D(const float angle, const unsigned int bezier_id, std::pair<glm::vec2, float>& result);
	unsigned int createBezierCurve2D(const glm::vec2 point_1, const glm::vec2 handle, const glm::vec2 point_2, const float thickness);
	unsigned int createBezierCurve2D(const glm::vec2 point_1, const glm::vec2 handle_1, const glm::vec2 point_2, const glm::vec2 handle_2, const float thickness);
	unsigned int drawBezierPolygon2D(const std::vector<PointNHandles>& point_n_handle, const float thickness);
	unsigned int drawBezierPolygon2D2(const std::vector<PointNHandles>& point_n_handle, const float thickness, glm::vec2& pivot);
	void determinePolygonAreaVolume(const unsigned int shape_id);
	unsigned int registerBezier2D(const Bezier2D& bezier);
	SingleMutex shape_mutex;
	IDStack<Shape> shapes;
	IDStack<Bezier2D> shape_patterns_2D;
	boost::container::flat_map<std::string, unsigned int> undefined_map;
	boost::container::flat_map<std::tuple<int, float, float, float>, unsigned int> symmetrical_polygon_2d_map;
	boost::container::flat_map<ShapeSpecification, unsigned int> polygon_2d_map;
};
}
