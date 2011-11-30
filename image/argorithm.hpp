#ifndef _IMAGE_ARGORITHM_
#define _IMAGE_ARGORITHM_

#include <limits>
#include <utility>
#include <cmath>
#include "container.hpp"

namespace Image
{
	template <typename T>
	container<std::pair<int, int>> full_search (
		const container<T> &premap,
		const container<T> &crtmap,
		unsigned int macro_brock_size,
		unsigned int search_size
	) {
		container<std::pair<int, int>> ve(
			premap.width()/macro_brock_size,
			premap.height()/macro_brock_size
		);

		//探索開始点
		for (int my=0; my*macro_brock_size < premap.height(); ++my) {
			for (int mx=0; mx*macro_brock_size < premap.width(); ++mx) {
				//探索
				double sad = std::numeric_limits<double>::max();
				int x = mx * macro_brock_size;
				int y = my * macro_brock_size;

				// -search_size 〜 search_size の範囲で探索
				int search = static_cast<int>(search_size);
				for (int dy = -search; dy <= search; ++dy) {
					for (int dx = -search; dx <= search; ++dx) {
						//画像端は処理対象外
						if (y+dy < 0 || x+dx < 0 ||
							y+dy+macro_brock_size > premap.height() ||
							x+dx+macro_brock_size > premap.width()
						) {
							continue;
						}

						//誤差計算
						double sum = 0;
						for (int iy=0; iy<macro_brock_size; ++iy) {
							for (int ix=0; ix<macro_brock_size; ++ix) {
								sum += abs(crtmap(x+ix, y+iy) - premap(x+ix+dx, y+iy+dy));
							}
						}

						//ベクトル保存
						if (sad > sum) {
							sad = sum;
							ve(mx, my).first  = dx;
							ve(mx, my).second = dy;
						}
					}
				}
			}
		}

		return ve;
	}
}

#endif

