#ifndef _IMAGE_UTILS_
#define _IMAGE_UTILS_

#include <utility>
#include "container.hpp"

namespace Image
{
	/**
	 * 予測画像の作成
	 *
	 * @param premap 元画像
	 * @param vec 動きベクトルコンテナ
	 * @param macro_block_size マクロブロックのサイズ
	 */
	template <typename T, typename E>
	container<T> prediction (
		const container<T> &premap,
		const container<std::pair<E, E>> &vec,
		const unsigned int macro_block_size )
	{
		container<T> mcmap(premap.width(), premap.height());

		//各マクロブロックごとに処理
		for (int cy = 0; cy < vec.height(); ++cy) {
			for (int cx = 0; cx < vec.width(); ++cx) {
				E dx = vec(cx, cy).first;
				E dy = vec(cx, cy).second;
				int x = cx * macro_block_size;
				int y = cy * macro_block_size;

				std::copy (
					premap, x+dx, y+dy,
					macro_block_size, macro_block_size,
					mcmap, x, y );
			}
		}

		return mcmap;
	}

}

#endif
/* vim: set ts=2 sw=2 sts=2 noexpandtab ff=unix ft=cpp fenc=utf-8 : */
