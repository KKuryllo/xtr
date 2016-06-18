#pragma once
/* Adapted from code segment by user R. Martinho Fernandes (http://stackoverflow.com/users/46642/r-martinho-fernandes) on StackOverflow.com */
#include  <type_traits>
namespace xtr {
template <typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e)
{
	return static_cast<typename std::underlying_type<E>::type>(e);
};
}