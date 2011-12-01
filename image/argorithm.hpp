#ifndef _IMAGE_ARGORITHM_
#define _IMAGE_ARGORITHM_

#include <limits>
#include <utility>
#include <cmath>
#include "container.hpp"

#include <iostream>

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
						if ((y+dy < 0) || (x+dx < 0) ||
							(y+dy+macro_brock_size > premap.height()) ||
							(x+dx+macro_brock_size > premap.width())
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

	template <typename T>
	container<std::pair<int, int>> three_step_search (
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
				int x = mx * macro_brock_size;
				int y = my * macro_brock_size;

				//中心点の誤差計算
				double sad = 0;
				for (int iy=0; iy<macro_brock_size; ++iy) {
					for (int ix=0; ix<macro_brock_size; ++ix) {
						sad += abs(crtmap(x+ix, y+iy) - premap(x+ix, y+iy));
					}
				}

				// n = 4, 2, 1 で近傍探索
				for (int n=4; n > 0; n >>= 1) {
					int px = ve(mx, my).first;
					int py = ve(mx, my).second;

					//n-step 8近傍セルと比較
					for (int dy = -n; dy <= n; dy += n) {
						for (int dx = -n; dx <= n; dx += n) {
							//画像端と画像中央は処理対象外
							if ((y+py+dy < 0) || (x+px+dx < 0) ||
								(y+py+dy+macro_brock_size > premap.height()) ||
								(x+px+dx+macro_brock_size > premap.width()) ||
								(dy == 0 && dx == 0)
							) {
								continue;
							}

							//誤差計算
							double sum = 0;
							for (int iy=0; iy<macro_brock_size; ++iy) {
								for (int ix=0; ix<macro_brock_size; ++ix) {
									sum += abs(crtmap(x+ix, y+iy) - premap(x+ix+px+dx, y+iy+py+dy));
								}
							}

							//ベクトル保存
							if (sad > sum) {
								sad = sum;
								ve(mx, my).first  = px+dx;
								ve(mx, my).second = py+dy;
							}
						}
					}
				}
			}
		}

		return ve;
	}

	template <typename T>
	container<std::pair<int, int>> diamond_search (
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
				int x = mx * macro_brock_size;
				int y = my * macro_brock_size;
				int px = 0;
				int py = 0;

				container<bool> is_searched(
					macro_brock_size*2+1,
					macro_brock_size*2+1
				);

				std::vector<std::pair<int, int>> ldsp = {
					{-2, 0}, {-1, 1}, { 0, 2}, { 1, 1},
					{ 2, 0}, { 1,-1}, { 0,-2}, {-1,-1}
				};
				std::vector<std::pair<int, int>> sdsp = {
					{-1, 0}, { 0, 1}, { 1, 0}, { 0,-1}
				};

				//中心点の誤差計算
				double sad = 0;
				for (int iy=0; iy<macro_brock_size; ++iy) {
					for (int ix=0; ix<macro_brock_size; ++ix) {
						sad += abs(crtmap(x+ix, y+iy) - premap(x+ix, y+iy));
					}
				}
				is_searched(macro_brock_size, macro_brock_size) = true;

				//ダイアモンド(|x|+|y|==2)上を移動して検索
				while (true) {
					for (auto i = ldsp.begin(); i != ldsp.end(); ++i) {
						int dx = i->first;
						int dy = i->second;

						//画像端と処理済みは処理対象外
						if ((y+py+dy < 0) || (x+px+dx < 0) ||
							(y+py+dy+macro_brock_size > premap.height()) ||
							(x+px+dx+macro_brock_size > premap.width()) ||
							is_searched(px+dx+macro_brock_size, py+dy+macro_brock_size)
						) {
							continue;
						}

						//誤差計算
						double sum = 0;
						for (int iy=0; iy<macro_brock_size; ++iy) {
							for (int ix=0; ix<macro_brock_size; ++ix) {
								sum += abs(crtmap(x+ix, y+iy) - premap(x+ix+px+dx, y+iy+py+dy));
							}
						}
						is_searched(px+dx+macro_brock_size, py+dy+macro_brock_size) = true;

						//ベクトル保存
						if (sad > sum) {
							sad = sum;
							ve(mx, my).first  = px+dx;
							ve(mx, my).second = py+dy;
						}
					}

					//px, py に変更が無いなら次へ
					if (px == ve(mx, my).first && py == ve(mx, my).second) {
						break;
					}
					else {
						px = ve(mx, my).first;
						py = ve(mx, my).second;
					}
				}

				//近接4点の比較からベクトル抽出
				for (auto i = sdsp.begin(); i != sdsp.end(); ++i) {
					int dx = i->first;
					int dy = i->second;

					//画像端は処理対象外
					if ((y+py+dy < 0) || (x+px+dx < 0) ||
						(y+py+dy+macro_brock_size > premap.height()) ||
						(x+px+dx+macro_brock_size > premap.width())
					) {
						continue;
					}

					//誤差計算
					double sum = 0;
					for (int iy=0; iy<macro_brock_size; ++iy) {
						for (int ix=0; ix<macro_brock_size; ++ix) {
							sum += abs(crtmap(x+ix, y+iy) - premap(x+ix+px+dx, y+iy+py+dy));
						}
					}

					//ベクトル保存
					if (sad > sum) {
						sad = sum;
						ve(mx, my).first  = px+dx;
						ve(mx, my).second = py+dy;
					}
				}
			}
		}

		return ve;
	}
}

#endif

