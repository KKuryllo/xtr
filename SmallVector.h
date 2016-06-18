#pragma once
#include <boost/container/small_vector.hpp>
namespace xtr {
template<typename type, size_t N>
using SmallVector = boost::container::small_vector<type, N>;
}