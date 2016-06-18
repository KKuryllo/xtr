#pragma once
namespace xtr {
	template<typename pairType>
	struct BasicPair{
		BasicPair() noexcept: x(0), y(0) {}
		BasicPair(pairType x_in, pairType y_in) noexcept : x(x_in), y(y_in) {}
		bool operator==(const BasicPair<pairType>& other_pair) const{
			return this->x == other_pair.x && this->y == other_pair.y;
		}
		bool operator!=(const BasicPair<pairType>& other_pair) const{
			return this->x != other_pair.x || this->y != other_pair.y;
		}
		bool operator<(const BasicPair<pairType>& other_pair) const{
			if (this->x > other_pair.x) return true;
			if (this->x == other_pair.x && this->y > other_pair.y) return true;
			else return false;
		}
		bool operator>(const BasicPair<pairType>& other_pair) const{
			if (this->x < other_pair.x) return true;
			if (this->x == other_pair.x && this->y < other_pair.y) return true;
			else return false;
		}
		BasicPair<pairType> operator+(const BasicPair<pairType>& second) const
		{
			return BasicPair<pairType>(this->x + second.x, this->y + second.y);
		}
		BasicPair<pairType> operator-(const BasicPair<pairType>& second) const
		{
			return BasicPair<pairType>(this->x - second.x, this->y - second.y);
		}
		BasicPair<pairType> operator*(const float second) const
		{
			return BasicPair<pairType>(this->x * second, this->y * second);
		}
		pairType x;
		pairType y;
	};

	typedef BasicPair<int> IntPair;
	typedef BasicPair<unsigned int> UintPair;
}
