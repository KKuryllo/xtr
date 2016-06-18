#pragma once
#include "ShapeLibrary.h"
namespace xtr {
class ShapeLink {
public:
	ShapeLink() {}
	ShapeLink(unsigned int shape_id)
	{
		link(shape_id);
	}
	ShapeLink(ShapeLink&& other) noexcept		// Move constructor
	{
		shape = other.shape;
		other.shape = 0;  // set this to zero to prevent a delink when others destructor is called
	}
	ShapeLink(const ShapeLink& other) noexcept	// Copy constructor
	{
		link(other.shape);
	}
	~ShapeLink()
	{
		if (shape != 0) shape_library.delinkShape(shape);
	}
	bool operator<(const ShapeLink& other) const
	{
		return shape < other.shape;
	}
	ShapeLink& operator=(ShapeLink&& other) noexcept
	{
		delink();
		shape = other.shape;
		other.shape = 0;
		return *this;
	}
	ShapeLink& operator=(const ShapeLink& other) noexcept
	{
		delink();
		link(other.shape);
		return *this;
	}
	void assign_unsafe(const unsigned int shape_id)
	{
		shape = shape_id;
	}
	unsigned int shapeID() const
	{
		return shape;
	}
	bool linked() const
	{
		return shape != 0;
	}
private:
	void link(const unsigned int shape_id)
	{
		if (shape_id != 0) {
			shape_library.linkShape(shape_id);
			shape = shape_id;
		}
	}
	void delink()
	{
		if (shape != 0) {
			shape_library.delinkShape(shape);
			shape = 0;
		}
	}
	unsigned int shape = 0;
	static xtr::ShapeLibrary& shape_library;
};
}
