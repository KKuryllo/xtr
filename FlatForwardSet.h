#pragma once

// This is incomplete
// The idea is to store contineous data in a single pointer for use with forward and direct access as well
namespace xtr {
template<typename T>
class FlatForwardSet {
	enum class STATUS : char {
		LAST,
		END,
		NEXT
	};
	typedef std::pair<T, STATUS> iterator;
	template<typename value_type>
	T& operator[](const value_type i)
	{
		return _data[i]
	}
	~FlatForwardSet()
	{
		delete _data;
		_data = nullptr;
	}
	void add(T&& t)
	{
		if (_data == nullptr) {
			_data = new T[1];
			_data[0].second = STATUS::END;
			_data[0].first = t;
			return;
		}
	}
public:
private:
	iterator* _data = nullptr;
};
}
