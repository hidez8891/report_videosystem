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

	template <typename T>
	double sum(const Image::container<T>& a)
	{
		double d = 0;

		std::for_each(a.begin(), a.end(), [&d](const float &i) {
			d += i;
		});

		return d;
	}
}

#endif
