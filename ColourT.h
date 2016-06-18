#pragma once
#include <glm/glm.hpp>
namespace xtr {
struct ColourT2
{
	ColourT2() {}
	ColourT2(unsigned char r_in, unsigned char a_in) : r(r_in), a(a_in) {}
	unsigned char r;
	unsigned char a;
};
struct ColourT
{
	ColourT() {}
	ColourT(unsigned char r_in, unsigned char g_in, unsigned char b_in, unsigned char a_in) : r(r_in), g(g_in), b(b_in), a(a_in) {}
	unsigned char& operator[](int position)
	{
		return components[position];
	}
	union 
	{
		unsigned char components[4];
		struct {
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		};
	};
};
}