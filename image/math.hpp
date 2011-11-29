#ifndef _IMAGE_MATH_
#define _IMAGE_MATH_

#include "container.hpp"
#include <cmath>

namespace std
{
	template <typename T>
	Image::container<T> pow(const Image::container<T>& a, double b)
	{
		return a.apply([b](T c) { return pow(c, b); });
	}

	template <typename T>
	Image::container<T> abs(const Image::container<T>& a)
	{
		return a.apply([](T c) { return abs(c); });
	}
}

#endif
