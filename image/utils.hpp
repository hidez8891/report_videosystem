#ifndef _IMAGE_UTILS_
#define _IMAGE_UTILS_

#include <utility>
#include "container.hpp"

namespace Image
{
	template <typename T, typename E1, typename E2>
	container<T> prediction(
		const container<T> &premap,
		const container<std::pair<E1,E2>> &vec,
		const unsigned int macro_brock_width,
		const unsigned int macro_brock_height
	) {
		container<T> mcmap(premap.width(), premap.height());

		//各マクロブロックごとに処理
		for (int cy = 0; cy < vec.height(); ++cy) {
			for (int cx = 0; cx < vec.width(); ++cx) {
				E1 dx = vec(cx, cy).first;
				E2 dy = vec(cx, cy).second;
				int x = cx * macro_brock_width;
				int y = cy * macro_brock_height;

				std::copy(
					premap, x+dx, y+dy,
					macro_brock_width, macro_brock_height,
					mcmap, x, y
				);
			}
		}

		return mcmap;
	}
}

#endif

