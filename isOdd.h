#pragma once
namespace xtr {
template<typename numerical>
bool isOdd(numerical x) { 
	return x & 1;
}
}
