#pragma once
#include <array>
namespace xtr {
template<typename T, unsigned int width_, unsigned int height_>
class Array2D {
public:
	using base_array = std::array<T, width_ * height_>;
	inline T& at(const unsigned int x, const unsigned int y)
	{
		return data[x * width_ + y];
	}
	inline const T& at(const unsigned int x, const unsigned int y) const
	{
		return data[x * width_ + y];
	}
	inline T& operator[](const unsigned int x)
	{
		return data[x];
	}
	inline const T& operator[](const unsigned int x) const
	{
		return data[x];
	}
	inline base_array& operator()()
	{
		return data;
	}
	inline const base_array& operator()() const
	{
		return data;
	}
	static constexpr unsigned int width = width_;
	static constexpr unsigned int height = height_;
	base_array data;
};
}
