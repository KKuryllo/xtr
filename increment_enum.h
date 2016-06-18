#pragma once
#include "to_underlying.h"
namespace xtr {
template<typename E>
constexpr E increment_enum(E val){
	return E(xtr::to_underlying(val) + 1);
}
}
