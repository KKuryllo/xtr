#include "stdafx.h"
#include "ShapeLibrary.h"
#include <cmath>
#include "ShapeLink.h"
#include "glm/gtx/projection.hpp"
using namespace xtr;
using vec2 = glm::vec2;
namespace{
constexpr float percision = 1.0f / float(pattern_size * 2);
constexpr float PI = float(std::_Pi);
constexpr float bezier_max_radius = float(pattern_size) / 2.0f;
constexpr float inverse_pat_size = 1.0f / float(pattern_size);
constexpr float inverse_pat_width = 1.0f / float(pattern_size - 1);
constexpr float bezier_circle_area = PI * (bezier_max_radius * bezier_max_radius);
constexpr float bezier_circle_perimeter = 2.0f * PI * bezier_max_radius;
// get the distance between two points
template<typename pointType>
auto pointDistance(const pointType point_1, const pointType point_2){
	auto&& x_diff = (point_1.x - point_2.x);
	auto&& y_diff = (point_1.y - point_2.y);
	return std::sqrt(x_diff * x_diff + y_diff * y_diff);
}
template<typename pointType>
auto getLength(const pointType point){
	auto&& x = point.x * point.x;
	auto&& y = point.y * point.y;
	return std::sqrt(x + y);
}
auto getAngle(const vec2 point){
	return std::atan2(point.y, point.x);
}
auto convertToCenteredProportion(const vec2 point){
	auto&& x = (point.x - bezier_max_radius) * inverse_pat_size;
	auto&& y = (point.y - bezier_max_radius) * inverse_pat_size;
	return vec2(x, y);
}
// Gets the mid point at a certain percentage
vec2 midPoint(const vec2 p1, const vec2 p2, const float percentage){
	auto&& difference = p2 - p1;
	return p1 + (difference * percentage);
}
void drawThickenedPoint(shape_pattern_2D& pattern, const vec2 point, const float thickness){
	for(float i = point.x - thickness; i <= point.x + thickness; ++i)
		for(float j = point.y - thickness; j <= point.y + thickness; ++j){
			auto&& x_diff_squared = (point.x - i) * (point.x - i);
			auto&& y_diff_squared = (point.y - j) * (point.y - j);
			auto&& thickness_squared = thickness * thickness;
			if(x_diff_squared + y_diff_squared <= thickness_squared) pattern[size_t(i)][size_t(j)] = 1;
		}
}
float calculateNormalAngle2D(const vec2 point_1, const vec2 point_2){
	auto&& vec = point_2 - point_1;
	return std::atan2(vec.y, vec.x) - PI / 2;
}
float bezierCurve2D1Handle(shape_pattern_2D& pattern, const vec2 point_1, const vec2 handle, const vec2 point_2, const float thickness){
	float length = 0;
	pattern[size_t(point_1.x)][size_t(point_1.y)] = 1;
	vec2 last_point = point_1;
	for(float i = percision; i < 1.0f; i += percision){
		//		Get mid point of first lines
		auto&& mid_point_1 = midPoint(point_1, handle, i);
		auto&& mid_point_2 = midPoint(handle, point_2, i);
		//		Get actual point on curve
		auto&& curve_point = midPoint(mid_point_1, mid_point_2, i);
		//		placepoint on pattern
		drawThickenedPoint(pattern, curve_point, thickness);
		length += pointDistance(last_point, curve_point);
		last_point = curve_point;
	}
	return length;
}
float bezierCurve2D2Handle(shape_pattern_2D& pattern, const vec2 point_1, const vec2 handle_1, const vec2 point_2, const vec2 handle_2, const float thickness){
	float length = 0;
	pattern[size_t(point_1.x)][size_t(point_1.y)] = 1;
	vec2 last_point = point_1;
	for(float i = percision; i < 1.0f; i += percision){
		//		Get mid point of first lines
		auto&& mid_point_1 = midPoint(point_1, handle_1, i);
		auto&& mid_handle = midPoint(handle_1, handle_2, i);
		auto&& mid_point_2 = midPoint(handle_2, point_2, i);
		//		Get mid point of the second lines
		auto&& line_point_1 = midPoint(mid_point_1, mid_handle, i);
		auto&& line_point_2 = midPoint(mid_handle, mid_point_2, i);
		//		Get point on curve
		auto&& curve_point = midPoint(line_point_1, line_point_2, i);
		//		placepoint on pattern
		drawThickenedPoint(pattern, curve_point, thickness);
		length += pointDistance(last_point, curve_point);
		last_point = curve_point;
	}
	return length;
}
// draw a quadratic bezier curver and find mins + maxs
// TODO separate
void bezierCurve2D2Handle2(std::vector<vec2>& points, const vec2 point_1, const vec2 handle_1, const vec2 point_2, const vec2 handle_2, vec2& mins, vec2& maxs){
	vec2 last_point;
	auto&& process_point = [&](vec2 point){
		// adjust mins and maxs
		if(point.x < mins.x) mins.x = point.x;
		else if(point.x > maxs.x) maxs.x = point.x;
		if(point.y < mins.y) mins.y = point.y;
		else if(point.y > maxs.y) maxs.y = point.y;
		last_point = point;
		points.push_back(point);
	};

	// process first point
	process_point(point_1);

	// generate rest of points
	for(float i = percision; i < 1.0f; i += percision){
		//		Get mid point of first lines
		auto&& mid_point_1 = midPoint(point_1, handle_1, i);
		auto&& mid_handle = midPoint(handle_1, handle_2, i);
		auto&& mid_point_2 = midPoint(handle_2, point_2, i);
		//		Get mid point of the second lines
		auto&& line_point_1 = midPoint(mid_point_1, mid_handle, i);
		auto&& line_point_2 = midPoint(mid_handle, mid_point_2, i);
		//		Get point on curve
		auto&& curve_point = midPoint(line_point_1, line_point_2, i);
		// process
		process_point(curve_point);
	}
}

float bezierFindNormalAtLength(float length_pos, const vec2 point_1, const vec2 handle_1, const vec2 point_2, const vec2 handle_2, std::pair<vec2, float>& result){
	float length = 0.0f;
	vec2 last_point = point_1;
	for(float i = percision; i < 1.0f; i += percision){
		//		Get mid point of first lines
		auto&& mid_point_1 = midPoint(point_1, handle_1, i);
		auto&& mid_handle = midPoint(handle_1, handle_2, i);
		auto&& mid_point_2 = midPoint(handle_2, point_2, i);
		//		Get mid point of the second lines
		auto&& line_point_1 = midPoint(mid_point_1, mid_handle, i);
		auto&& line_point_2 = midPoint(mid_handle, mid_point_2, i);
		//		Get point on curve
		auto&& curve_point = midPoint(line_point_1, line_point_2, i);
		//		placepoint on pattern
		length += pointDistance(last_point, curve_point);
		if(length >= length_pos){
			result.first = last_point + (curve_point - last_point) / 2.0f;
			result.second = calculateNormalAngle2D(last_point, curve_point);
      return length;
		}
		last_point = curve_point;
	}
	return length;
}
// not complete?  should output result?
bool rayLineIntersection2D(const vec2 ray, const vec2 point_1, const vec2 point_2, vec2& result){
	auto&& line_slope = point_1.x / point_2.y;
	auto&& line_b = point_1.y - point_1.x * line_slope;
	auto&& ray_slope = ray.x / ray.y;
	if(line_slope == ray_slope) return false;
	auto&& x_intersect = line_b / (ray_slope / line_slope);
	if(point_1.x <= point_2.x){
		return (point_1.x <= x_intersect || x_intersect < point_2.x);
	} else{
		return (point_2.x < x_intersect || x_intersect < point_1.x);
	}
}
bool bezierIntersect2D2Handle(const vec2 ray, const vec2 point_1, const vec2 handle_1, const vec2 point_2, const vec2 handle_2, std::pair<vec2, float>& result){
	vec2 test_1 = point_1;		// initial test will use first point
	vec2 test_2;
	for(float i = percision; i < 1.0f; i += percision){
		//		Get mid point of first lines
		auto&& mid_point_1 = midPoint(point_1, handle_1, i);
		auto&& mid_handle = midPoint(handle_1, handle_2, i);
		auto&& mid_point_2 = midPoint(handle_2, point_2, i);
		//		Get mid point of the second lines
		auto&& line_point_1 = midPoint(mid_point_1, mid_handle, i);
		auto&& line_point_2 = midPoint(mid_handle, mid_point_2, i);
		//		Get point on curve
		test_2 = midPoint(line_point_1, line_point_2, i);
		// test for intersection on internal points
		if(rayLineIntersection2D(ray, test_1, test_2, result.first)){
			result.second = calculateNormalAngle2D(test_1, test_2);
			return true;
		}
		// shift points
		test_1 = test_2;
	}
	// test for intersection at end points
	if(rayLineIntersection2D(ray, test_1, point_2, result.first)){
		result.second = calculateNormalAngle2D(test_1, test_2);
		return true;
	}
	return false;
}

template<typename pointType>
pointType rotatePoint(const pointType point, const float angle){
	auto&& x = point.x * std::cos(angle) - point.y * std::sin(angle);
	auto&& y = point.x * std::sin(angle) + point.y * std::cos(angle);
	return pointType(x, y);
}
vec2 makePoint(const float distance, const float angle){
	auto&& x = distance * std::cos(angle);
	auto&& y = distance * std::sin(angle);
	return{x , y};
}
void fillShape(shape_pattern_2D& fill, const int start_x, const int start_y){
	// otherwise fill the colour of the pixel
	fill[start_x][start_y] = 1;
	int w = start_x;
	int e = start_x;

	// find limit to the west
	while(w > 0){
		if(fill[w - 1][start_y] == 0){
			fill[w - 1][start_y] = 1;
			--w;
		} else break;
	}

	// find limit to the east
	while(e < pattern_size - 1){
		if(fill[e + 1][start_y] == 0){
			fill[e + 1][start_y] = 1;
			++e;
		} else break;
	}

	// check pixel to the north
	if(start_y < pattern_size - 1)
		for(int i = w; i < e + 1; ++i){
			if(fill[i][start_y + 1] == 0)
				fillShape(fill, i, start_y + 1);
		}

	// check pixel to the south
	if(start_y > 0)
		for(int i = w; i < e + 1; ++i){
			if(fill[i][start_y - 1] == 0)
				fillShape(fill, i, start_y - 1);
		}
}

float countArea(shape_pattern_2D& fill){
	auto&& count = 0.0f;
	for(auto&& row : fill)
		count += float(row.count());
	return count;
}
}

ShapeLibrary::ShapeLibrary(){
	shapes.add(Shape(ShapeTypes::NUM_TYPES, 0));			// Ensure ID zero can't be used
	// Uncomment to allow for shape testing to console
	/*

	auto&& id = drawSymeticalPolygon2D(13, 0.7f, 1.0f, 1.0f);
	auto&& bez_id = getShape(id.shapeID()).type_index;
	auto&& traced_normals = findSurfaceTracedNormals(0.5f, bez_id, 13);
	for (auto&& line : shape_patterns_2D[bez_id].pattern) {
		for (size_t i = 0; i < line.size(); ++i) {
			if (line[i]) std::cout << '_';
			else std::cout << '?';
		}

		std::cout << std::endl;
	}
	*/
}
ShapeLibrary::~ShapeLibrary(){}

// NOTE THIS IS NOT THREAD SAFE YOU MUST MAKE IT SO IF USING MULTITHREADING when initializing.  (i.e. initialize before doing any multithreading...)
ShapeLibrary& ShapeLibrary::getInstance(){
	static ShapeLibrary instance;			// Guaranteed to be destroyed.
	return instance;							// Instantiated on first use.	
}

unsigned int ShapeLibrary::createBezierCurve2D(const vec2 point_1, const vec2 handle, const vec2 point_2, const float thickness){
	// create a new pattern
	Bezier2D bezier;
	auto&& pattern = bezier.pattern;
	bezier.length = bezierCurve2D1Handle(pattern, point_1, handle, point_2, thickness);
	auto&& points = bezier.points_n_handles;
	points.push_back({vec2(), point_1, handle});
	points.push_back({handle, point_2, vec2()});
	return registerBezier2D(bezier);
}

unsigned int ShapeLibrary::createBezierCurve2D(const vec2 point_1, const vec2 handle_1, const vec2 point_2, const vec2 handle_2, const float thickness){
	// create a new pattern
	Bezier2D bezier;
	auto&& pattern = bezier.pattern;
	bezier.length = bezierCurve2D2Handle(pattern, point_1, handle_1, point_2, handle_2, thickness);
	auto&& points = bezier.points_n_handles;
	points.push_back({vec2(), point_1, handle_1});
	points.push_back({handle_2, point_2, vec2()});
	return registerBezier2D(bezier);
}

unsigned int ShapeLibrary::drawBezierPolygon2D(const std::vector<PointNHandles>& points_n_handles, const float thickness){
	// create a new pattern
	Bezier2D bezier;
	auto&& pattern = bezier.pattern;

	// draw all internal curves
	for(auto&& iter = points_n_handles.begin(); iter < points_n_handles.end() - 1; ++iter){
		auto&& point_1 = iter->point;
		auto&& handle_1 = iter->handle_2;
		auto&& point_2 = (iter + 1)->point;
		auto&& handle_2 = (iter + 1)->handle_1;
		bezier.length += bezierCurve2D2Handle(pattern, point_1, handle_1, point_2, handle_2, thickness);
	}
	// link end points
	auto&& point_1 = (points_n_handles.end() - 1)->point;
	auto&& handle_1 = (points_n_handles.end() - 1)->handle_2;
	auto&& point_2 = points_n_handles.begin()->point;
	auto&& handle_2 = points_n_handles.begin()->handle_1;
	bezier.length += bezierCurve2D2Handle(pattern, point_1, handle_1, point_2, handle_2, thickness);
	// register and return result
	bezier.points_n_handles = points_n_handles;
	return registerBezier2D(bezier);
}

unsigned int ShapeLibrary::drawBezierPolygon2D2(const std::vector<PointNHandles>& points_n_handles, const float thickness, glm::vec2& pivot){
	// create temporary vector for points
	std::vector<vec2> point_collection;
	// variables to hold max & min x/y of all points
	vec2 mins = points_n_handles.begin()->point;
	vec2 maxs = points_n_handles.begin()->point;
	// draw all internal curves
	for(auto&& iter = points_n_handles.begin(); iter < points_n_handles.end() - 1; ++iter){
		auto&& point_1 = iter->point;
		auto&& handle_1 = iter->handle_2;
		auto&& point_2 = (iter + 1)->point;
		auto&& handle_2 = (iter + 1)->handle_1;
		bezierCurve2D2Handle2(point_collection, point_1, handle_1, point_2, handle_2, mins, maxs);
	}
	// link end points
	auto&& point_1 = (points_n_handles.end() - 1)->point;
	auto&& handle_1 = (points_n_handles.end() - 1)->handle_2;
	auto&& point_2 = points_n_handles.begin()->point;
	auto&& handle_2 = points_n_handles.begin()->handle_1;
	bezierCurve2D2Handle2(point_collection, point_1, handle_1, point_2, handle_2, mins, maxs);
	// Reposition points to fit within pattern area
	auto&& x_dist_max = maxs.x - mins.x;
	auto&& y_dist_max = maxs.y - mins.y;
	float max_dist = x_dist_max > y_dist_max ? x_dist_max : y_dist_max;

	auto&& scale = (float(pattern_size - 1) - 2.0f * (thickness + 0.5f)) / max_dist;
	auto&& x_buff = (float(pattern_size - 1) - x_dist_max * scale) * 0.5f;
	auto&& y_buff = (float(pattern_size - 1) - y_dist_max * scale) * 0.5f;

	// scale and translate points to fit in pattern
	auto&& adjust_vec2 = [&](vec2 point){
		point.x = (point.x - mins.x) * scale + x_buff;
		point.y = (point.y - mins.y) * scale + y_buff;
		return point;
	};

	// Scale points to pattern center
	for(auto&& point : point_collection){
		point = adjust_vec2(point);
		assert(point.x - thickness >= 0);
		assert(point.y - thickness >= 0);
		assert(point.x + thickness <= float(pattern_size - 1));
		assert(point.y + thickness <= float(pattern_size - 1));
		//TODO investigate why assertations fail
	}
	// resize points_n_handles for later map lookup when creating new shape
	Bezier2D bezier;
	bezier.points_n_handles.resize(points_n_handles.size());

	// Scale original points and handles as well as save in bezier
	for(size_t i = 0; i < points_n_handles.size(); i++){
		bezier.points_n_handles[i].handle_1 = adjust_vec2(points_n_handles[i].handle_1);
		bezier.points_n_handles[i].point = adjust_vec2(points_n_handles[i].point);
		bezier.points_n_handles[i].handle_2 = adjust_vec2(points_n_handles[i].handle_2);
	}
	// Scale pivot as well
	pivot = adjust_vec2(pivot);

	//offload and measure perimeter of shape
	auto&& current = point_collection.begin();
	auto&& next = point_collection.begin() + 1;
	while(next != point_collection.end()){
		bezier.length += pointDistance(*current, *next);
		drawThickenedPoint(bezier.pattern, *current, thickness);
		current = next;
		next++;
	}
	next = point_collection.begin();
	// once more to connect end to beginning
	bezier.length += pointDistance(*current, *next);
	drawThickenedPoint(bezier.pattern, *current, thickness);

	// register and return result
	return registerBezier2D(bezier);
}

ShapeLink ShapeLibrary::drawSymeticalPolygon2D(const int symmetry, const float indentation, const float sharpness, const float thickness){
	ShapeLink link;
	// First check if it already exists
	// Checking if it exists twice (also at the end) isn't a good way to do this.  Solve this problem
	// Maybe what you should do is reserve position? Will need to prevent use of position till it is ready.  Maybe a spin lock on a boolean?
	{
		ScopedLock a_lock(shape_mutex);
		auto&& result = symmetrical_polygon_2d_map.find(std::tie(symmetry, indentation, sharpness, thickness));
		if(result != symmetrical_polygon_2d_map.end()){
			link.assign_unsafe(result->second);
			++shapes[result->second].links;
			return link;
		}
	}

	// create a new pattern
	// Generate points and handles
	std::vector<PointNHandles> points_n_handles;
	// Set necessary variables
	points_n_handles.resize(symmetry * 2);
	float&& angle_step = (2.0f * PI) / float(points_n_handles.size());
	float&& current_angle = 0.0f;
	auto&& max_radius = (float(pattern_size) - 1.0f) / 2.0f - thickness;
	auto&& min_radius = max_radius * indentation;
	auto&& origin = vec2(max_radius + thickness, max_radius + thickness);
	// now generate points
	// Note that as the shape is symmetrical we save calculations by making the handle on one side the mirror image of the one on the other
	for(size_t&& i = 0; i < points_n_handles.size(); i += 2){
		auto&& point_1 = origin + makePoint(max_radius, current_angle);
		auto&& point_2 = origin + makePoint(min_radius, current_angle + angle_step);
		points_n_handles[i].point = point_1;
		points_n_handles[i + 1].point = point_2;
		// hangle length
		auto&& handle_length_1 = max_radius * 4 / 3 * std::tan(PI / (2 * symmetry * 2)) * sharpness;
		auto&& handle_length_2 = min_radius * 4 / 3 * std::tan(PI / (2 * symmetry * 2)) * sharpness;
		// generate handles between these two points.
		auto&& point_1_handle_disp = makePoint(handle_length_1, current_angle - PI * 0.5f);
		auto&& point_2_handle_disp = makePoint(handle_length_2, current_angle + angle_step + PI * 0.5f);
		points_n_handles[i].handle_1 = point_1 + point_1_handle_disp;
		points_n_handles[i].handle_2 = point_1 - point_1_handle_disp;
		points_n_handles[i + 1].handle_1 = point_2 - point_2_handle_disp;
		points_n_handles[i + 1].handle_2 = point_2 + point_2_handle_disp;
		current_angle += angle_step * 2.0f;
	}
	auto&& id = drawBezierPolygon2D2(points_n_handles, thickness, origin);
	// Now try and add it to the library
	{
		ScopedLock a_lock(shape_mutex);
		auto&& result = symmetrical_polygon_2d_map.find(std::tie(symmetry, indentation, sharpness, thickness));		// Do a second check to see if it hasn't already been created
		if(result != symmetrical_polygon_2d_map.end()){
			// created in meantime.  remove newly generated shape
			delinkShape(id);
			link.assign_unsafe(result->second);
			++shapes[result->second].links;
			return link;
		} else{	// truely a new one add it
			symmetrical_polygon_2d_map[std::tie(symmetry, indentation, sharpness, thickness)] = id;
			link.assign_unsafe(id);
			shapes[id].type = ShapeTypes::SYMMETRICAL_POLYGON_2D;
			determinePolygonAreaVolume(id);
			return link;
		}
	}
}

ShapeLink ShapeLibrary::drawPolygon2D(const ShapeSpecification&& specification){
	ShapeLink link;
	// First check if it already exists
	{
		ScopedLock a_lock(shape_mutex);
		auto&& result = polygon_2d_map.find(specification);
		if(result != polygon_2d_map.end()){
			link.assign_unsafe(result->second);
			++shapes[result->second].links;
			return link;
		}
	}
	// Now Draw the shape
	auto pivot = specification.pivot;
	auto&& points = specification.points;
	auto&& thickness = specification.thickness;

	// create a new pattern
	// points and handles to generate pattern
	std::vector<PointNHandles> points_n_handles;

	// Generate points
	// Store value temporarily in the x coordinate of the first handle

	auto&& simple_symmetry = [&](){
		for(auto&& point : points){
			xtr::PointNHandles p_n_h;
			p_n_h.point = {point.coordinate.x, point.coordinate.y};
			p_n_h.handle_2.x = point.curvature; // temporary storage of curvature in handle_2.x for performance
			points_n_handles.push_back(p_n_h);
		}
		pivot.y = -pivot.y;  // Venu look at this
	};

	auto&& rotation_symmetry = [&](){
		float scaling = 1.0f / float(specification.symmetry);
		float angle_displacement = 2.0f * PI / float(specification.symmetry);
		for(int i = 0; i < specification.symmetry; i++)
			for(auto&& point : points){
				auto&& angle = getAngle(point.coordinate) * scaling + float(i) * angle_displacement;;
				auto&& mag = getLength(point.coordinate);
				//	auto&& new_angle = angle * scaling + float(i) * angle_displacement;
				auto&& new_point = makePoint(mag, angle);
				xtr::PointNHandles p_n_h;
				p_n_h.point = {new_point.x, float(pattern_size) - 1.0f - new_point.y};
				p_n_h.handle_2.x = point.curvature;
				points_n_handles.push_back(p_n_h);
			}
	};

	auto&& mirror_rotational_symmetry = [&](){
		float angle_scaling = 1.0f / float(specification.symmetry);
		float angle_displacement = 2.0f * PI / float(specification.symmetry);
		for(int i = 0; i < specification.symmetry; i++){
			size_t start;
			size_t end;
			size_t change;
			if(i % 2 == 0){
				start = 0;
				end = points.size();
				change = 1;
			} else{
				start = points.size() - 1;
				end = -1;
				change = -1;
			}
			for(auto&& index = start; index != end; index += change){
				auto&& point = points[index];
				auto&& angle = getAngle(point.coordinate);
				auto&& mag = getLength(point.coordinate);
				auto&& new_angle = (change == -1 ? angle_displacement - angle * angle_scaling : angle * angle_scaling) + float(i) * angle_displacement;
				auto&& new_point = makePoint(mag, new_angle);
				xtr::PointNHandles p_n_h;
				p_n_h.point = {new_point.x, float(pattern_size) - 1.0f - new_point.y};
				p_n_h.handle_2.x = point.curvature;
				points_n_handles.push_back(p_n_h);
			}
		}
	};

	auto&& mirror_linear_symmetry = [&](){

	};

	if(specification.symmetry > 1){	// Use point replicating functions for higher level symmetry if necessary
		switch(specification.sym_type){
		case SymmetryType::ROTATIONAL:
			rotation_symmetry();
			break;
		case SymmetryType::MIRROR_ROTATIONAL:
			mirror_rotational_symmetry();
			break;
		case SymmetryType::MIRROR_LINEAR:
			mirror_linear_symmetry();
			break;
		}
	} else simple_symmetry();      // else just transfer points over


		// Generate Handles	
	auto&& previous = points_n_handles.end() - 1;
	auto&& current = points_n_handles.begin();
	auto&& next = points_n_handles.begin() + 1;

	// Lambda for generating handles
	auto&& gen_handles = [&](){
		// The handles are parallel to a line from the previous point to the next point and the length of the handles can
		// be calculated py projection onto this line of the current point
		// first set the previous point as the origin
		auto&& c = current->point - previous->point;
		auto&& n = next->point - previous->point;
		// now calculate the negative handle 1 vector by projection of c onto n
		auto&& h1 = glm::proj(c, n);
		// calculate positive handle 2 vector by differnce between n vector and h1
		auto&& h2 = n - h1;
		// now calculate handle points
		current->handle_1 = current->point - h1 * current->handle_2.x;  // NOTE: curvature stored in x value of second handle
		current->handle_2 = current->point + h2 * current->handle_2.x;
	};
	// generate handles for all points
	while(next != points_n_handles.end()){
		gen_handles();
		previous = current;
		current = next;
		++next;
	}
	next = points_n_handles.begin();
	gen_handles();

	auto&& id = drawBezierPolygon2D2(points_n_handles, thickness, pivot);

	// Now try and add it to the library
	{
		ScopedLock a_lock(shape_mutex);
		auto&& result = polygon_2d_map.find(specification);		// Do a second check to see if it hasn't already been created
		if(result != polygon_2d_map.end()){
			// created in meantime.  remove newly generated shape
			delinkShape(id);
			link.assign_unsafe(result->second);
			++shapes[result->second].links;
			return link;
		} else{	// truely a new one add it
			polygon_2d_map[std::move(specification)] = id;
			link.assign_unsafe(id);
			shapes[id].type = ShapeTypes::POLYGON_2D;
			shapes[id].pivot = {pivot.x * inverse_pat_width, (1.0f - pivot.y * inverse_pat_width)};
			determinePolygonAreaVolume(id);
			return link;
		}
	}
}

void ShapeLibrary::determinePolygonAreaVolume(const unsigned int shape_id){
	auto&& shape = shapes[shape_id];
	auto&& poly_data = shape_patterns_2D[shape.type_index];
	auto pattern = poly_data.pattern;
	fillShape(pattern, pattern_size / 2, pattern_size / 2);
	shape.volume = countArea(pattern) / bezier_circle_area;
	shape.area = poly_data.length / bezier_circle_perimeter;
}

// Get back a shapes pattern
const shape_pattern_2D ShapeLibrary::getPattern_2D(unsigned int id){
	ScopedLock a_lock(shape_mutex);
	return shape_patterns_2D[id].pattern;
}

// Register an undefined shape so that it may be linked elsewhere
ShapeLink ShapeLibrary::getIDForUndefined(const std::string& shape_tag){
	ShapeLink link;
	// First see if tag exists
	ScopedLock alock(shape_mutex);
	auto&& result = undefined_map.find(shape_tag);
	if(result != undefined_map.end()){
		link.assign_unsafe(result->second);
		++shapes[result->second].links;
		return link;
	}
	// If not register it
	auto&& id = (unsigned int)shapes.add(Shape(ShapeTypes::UNDEFINED, 0));
	undefined_map[shape_tag] = id;
	link.assign_unsafe(id);
	return link;
}

std::string ShapeLibrary::getNameForUndefined(const unsigned int shape_id){
	ScopedLock alock(shape_mutex);
	for(auto&& pair : undefined_map)
		if(pair.second == shape_id) return pair.first;
	return "KEY NOT FOUND";
}

// Retrieve a shape by it's id
const Shape ShapeLibrary::getShape(const unsigned int shape_id){
	ScopedLock alock(shape_mutex);
	assert(!shapes.isFree(shape_id));
	return shapes[shape_id];
}

const Shape ShapeLibrary::getShapeActivate(const unsigned int shape_id){
	ScopedLock alock(shape_mutex);
	assert(!shapes.isFree(shape_id));
	auto shape = shapes[shape_id];
	shapes[shape_id].status = Shape_Status::ACTIVE;
	return shape;
}

bool ShapeLibrary::findNormalAtAngle2D(const float angle, const unsigned int bezier_id, std::pair<vec2, float>& result){
	std::vector<PointNHandles> points_n_handles;
	{
		ScopedLock alock(shape_mutex);
		points_n_handles = shape_patterns_2D[bezier_id].points_n_handles;
	}

	auto&& ray = makePoint(1.0f, angle);
	// find for internal curves
	for(auto&& iter = points_n_handles.begin(); iter < points_n_handles.end() - 1; ++iter){
		auto&& point_1 = iter->point;
		auto&& handle_1 = iter->handle_2;
		auto&& point_2 = (iter + 1)->point;
		auto&& handle_2 = (iter + 1)->handle_1;
		if(bezierIntersect2D2Handle(ray, point_1, handle_1, point_2, handle_2, result))
			return true;
	}
	// find for end points
	auto&& point_1 = (points_n_handles.end() - 1)->point;
	auto&& handle_1 = (points_n_handles.end() - 1)->handle_2;
	auto&& point_2 = points_n_handles.begin()->point;
	auto&& handle_2 = points_n_handles.begin()->handle_1;
	return bezierIntersect2D2Handle(ray, point_1, handle_1, point_2, handle_2, result);
}

std::vector<std::pair<vec2, float>> ShapeLibrary::findSurfaceTracedNormals(const float proportion, const unsigned int bezier_id, const int symmetry){
	std::vector<std::pair<vec2, float>> result;
	Bezier2D bezier;
	{
		ScopedLock alock(shape_mutex);
		bezier = shape_patterns_2D[bezier_id];
	}
	auto&& points_n_handles = bezier.points_n_handles;
	std::pair<vec2, float> normal;

	const float divided_length = bezier.length / symmetry;
	const float length_stop = (divided_length * proportion);
	float length_pos = length_stop;
	bool looking = true;

	// find for internal curves
	for(size_t&& i = 0; i < points_n_handles.size() - 1; ++i){
		if(i % (points_n_handles.size() / symmetry) == 0) looking = true;		// always start looking at beginning.
		if(looking){
			auto&& point_1 = points_n_handles[i].point;
			auto&& handle_1 = points_n_handles[i].handle_2;
			auto&& point_2 = points_n_handles[i + 1].point;
			auto&& handle_2 = points_n_handles[i + 1].handle_1;
			auto&& end = bezierFindNormalAtLength(length_pos, point_1, handle_1, point_2, handle_2, normal);
			if(end >= length_pos){
				result.push_back(normal);
				length_pos = length_stop;
				looking = false;
			} else
				length_pos -= end;
		}
	}
	// find for end points
	if(looking){
		auto&& point_1 = (points_n_handles.end() - 1)->point;
		auto&& handle_1 = (points_n_handles.end() - 1)->handle_2;
		auto&& point_2 = points_n_handles.begin()->point;
		auto&& handle_2 = points_n_handles.begin()->handle_1;
		auto&& end = bezierFindNormalAtLength(length_pos, point_1, handle_1, point_2, handle_2, normal);
		if(end >= length_pos){
			result.push_back(normal);
			length_pos = length_stop;
		}
	}
	return result;
}

PartOrientation ShapeLibrary::findFirstSymmetricNormal(const float proportion, const ShapeLink& shape, const int symmetry){
	std::pair<vec2, float> normal;
	Bezier2D bezier;
	{
		ScopedLock alock(shape_mutex);
		bezier = shape_patterns_2D[shapes[shape.shapeID()].type_index];
	}
	auto&& points_n_handles = bezier.points_n_handles;

	const float divided_length = bezier.length / symmetry;
	float length_pos = (divided_length * proportion);

	// find for internal curves
	for(size_t&& i = 0; i < points_n_handles.size() - 1; ++i){
		auto&& point_1 = points_n_handles[i].point;
		auto&& handle_1 = points_n_handles[i].handle_2;
		auto&& point_2 = points_n_handles[i + 1].point;
		auto&& handle_2 = points_n_handles[i + 1].handle_1;
		auto&& end = bezierFindNormalAtLength(length_pos, point_1, handle_1, point_2, handle_2, normal);
		if(end >= length_pos){
			auto&& point = convertToCenteredProportion(normal.first);
			auto&& position_angle = getAngle(point);
			auto&& displacement = getLength(point);
			return PartOrientation(normal.second, position_angle, displacement);
		} else
			length_pos -= end;
	}
	// if here point was not found on internal curve section and must be between end and begining point
	auto&& point_1 = (points_n_handles.end() - 1)->point;
	auto&& handle_1 = (points_n_handles.end() - 1)->handle_2;
	auto&& point_2 = points_n_handles.begin()->point;
	auto&& handle_2 = points_n_handles.begin()->handle_1;
#ifndef NDEBUG
  auto length =
#endif
	bezierFindNormalAtLength(length_pos, point_1, handle_1, point_2, handle_2, normal);
  assert(length >= length_pos);
	auto&& point = convertToCenteredProportion(normal.first);
	return PartOrientation(normal.second, getAngle(point), getLength(point));
}

// Private Memeber functions
unsigned int ShapeLibrary::registerBezier2D(const Bezier2D& bezier){
	// register pattern
	unsigned int&& pattern_id = 0;
	ScopedLock alock(shape_mutex);
	pattern_id = (unsigned int)shape_patterns_2D.add(bezier);
	return (unsigned int)shapes.add(Shape(ShapeTypes::BEZIER_2D, pattern_id));
}