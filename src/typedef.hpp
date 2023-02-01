/*
 * Author: Vardan Grigoryan
 * Date: 02/01/2023
 */

#ifndef __TYPEDEF_HPP__
#define __TYPEDEF_HPP__

#include <functional>

#include "result.hpp"

template <typename R, typename... T>
struct typedefs {
	using cb = std::function<R(T...)>;
};


using rcb = typedefs<void, result<int>>::cb;

#endif // __TYPEDEF_HPP__
