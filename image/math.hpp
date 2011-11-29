#ifndef _IMAGE_MATH_
#define _IMAGE_MATH_

#include "container.hpp"
#include <cmath>

namespace std
{
	Image::container pow(const Image::container& a, double b)
	{
		return a.apply( [b](container::value_type a) { return pow(a, b); } );
	}
}

#endif
