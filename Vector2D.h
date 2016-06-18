#pragma once
#include <vector>
namespace xtr {
template <typename T>
// A simple 2D vector designed to emulate function from a 2D array
class Vector2D {
public:
	Vector2D() {}
	Vector2D(const size_t x, const size_t y)
	{
		resize(x, y);
	}
	Vector2D(const size_t x, const size_t y, T initial_value)
	{
		resize(x, y, initial_value);
	}
	void resize(const size_t x, const size_t y)
	{
		data.resize(x);
		for (auto& row : data)
			row.resize(y);
	}
	void resize(const size_t x, const size_t y, T initial_value)
	{
		data.resize(x);
		for (auto& row : data) {
			row.resize(y);
			for (auto&& val : row)
				val = initial_value;
		}
	}
	void fill(T value)
	{
		for (auto& row : data) {
			row.fill(row.begin(), row.end(), value);
		}
	}
	inline std::vector<T>& operator[](const size_t x)
	{
		return data[x];
	}
	inline const std::vector<T>& operator[](const size_t x) const
	{
		return data[x];
	}
	inline std::pair<size_t, size_t>  size()
	{
		return{ data.size(), data.size() > 0 ? data[0].size() : 0 };
	}
	inline void swap(Vector2D<T>& other)
	{
		data.swap(other.data);
	}
	std::vector<std::vector<T>> data;
};
}