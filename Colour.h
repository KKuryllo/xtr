#pragma once
#include <glm/glm.hpp>
#include "ColourT.h"
namespace xtr {
	// This is the colour object used by the engine.
	typedef struct Colour4{
		Colour4() {}
		Colour4(const float red, const float green, const float blue, const float alpha) : r(red), g(green), b(blue), a(alpha) {}
		Colour4(const glm::vec4& glm_colour) : r(glm_colour[0]), g(glm_colour[1]), b(glm_colour[2]), a(glm_colour[3]) {}
		union {
			float components[4];
			struct {
				float r;
				float g;
				float b;
				float a;
			};
		};
		// Assignment operator
		Colour4& operator=(const Colour4& other_colour){
			this->r = other_colour.r;
			this->g = other_colour.g;
			this->b = other_colour.b;
			this->a = other_colour.a;
			return *this;
		}
		// Addition operator
		Colour4 operator+(const Colour4& other_colour)const {
			Colour4 out_colour;
			out_colour.r = this->r + other_colour.r;
			out_colour.g = this->g + other_colour.g;
			out_colour.b = this->b + other_colour.b;
			out_colour.a = this->a + other_colour.a;
			return out_colour;
		}
		// Subtraction operator
		Colour4 operator-(const Colour4& other_colour)const {
			Colour4 out_colour;
			out_colour.r = other_colour.r - this->r;
			out_colour.g = other_colour.g - this->g;
			out_colour.b = other_colour.b - this->b;
			out_colour.a = other_colour.a - this->a;
			return out_colour;
		}
		// negation operator
		Colour4 operator-(){
			Colour4 out_colour;
			out_colour.r = -this->r;
			out_colour.g = -this->g;
			out_colour.b = -this->b;
			out_colour.a = -this->a;
			return out_colour;
		}
		// Addition Assingment operator
		Colour4 operator+=(const Colour4& other_colour){
			this->r += other_colour.r;
			this->g += other_colour.g;
			this->b += other_colour.b;
			this->a += other_colour.a;
			return *this;
		}
		// Division operator
		Colour4 operator/(const float divisor) const {
			Colour4 out_colour;
			out_colour.r = this->r / divisor;
			out_colour.g = this->g / divisor;
			out_colour.b = this->b / divisor;
			out_colour.a = this->a / divisor;
			return out_colour;
		}
		// Multiplication operator
		Colour4 operator*(const float factor) const{
			Colour4 out_colour;
			out_colour.r = this->r * factor;
			out_colour.g = this->g * factor;
			out_colour.b = this->b * factor;
			out_colour.a = this->a * factor;
			return out_colour;
		}
		float& operator[](const int i){
			return components[i];
		}
		glm::vec4 toVec4() const { return glm::vec4(r, g, b, a); }
		ColourT toColourT() const { return ColourT((unsigned char)(255u * r), (unsigned char)(255u * g), (unsigned char)(255u * b), (unsigned char)(255u * a)); }
	} Colour4;
}
