#ifndef _IMAGE_ALGORITHM_
#define _IMAGE_ALGORITHM_

#include <limits>
#include <utility>
#include <cmath>
#include "container.hpp"

namespace Image 
{
	typedef std::pair<int, int> ve_pair;
	typedef container<ve_pair>  ve_container;

	/**
	 * 動きベクトル検出
	 *
	 * @param premap 原画像
	 * @param crtmap 次画像
	 * @param macro_block_size ブロックのサイズ
	 * @param search_size ブロックの探索範囲
	 * @param func 検出アルゴリズム
	 * @return マクロブロックの動きベクトルのコンテナ
	 */
	template <typename T, typename Function>
	ve_container motion_vector_search (
		const container<T> &premap,
		const container<T> &crtmap,
		const unsigned int macro_block_size,
		const unsigned int search_size,
		const Function &func )
	{
		ve_container ve (
			premap.width()  / macro_block_size,
			premap.height() / macro_block_size );

		//探索開始点
		for (int my=0; my * macro_block_size < premap.height(); ++my) {
			for (int mx=0; mx * macro_block_size < premap.width(); ++mx) {
				//探索
				double sad = std::numeric_limits<double>::max();
				int x = mx * macro_block_size;
				int y = my * macro_block_size;

				//動きベクトル取得
				ve(mx, my) = func(premap, crtmap, x, y, macro_block_size, search_size);
			}
		}

		return ve;
	}


	namespace search
	{
		/**
		 * 検出アルゴリズム ベースクラス
		 */
		struct _base_search_algorithm
		{
			/**
			 * マクロブロックが対象画像からはみ出すか検査
			 *
			 * @param premap 対象画像
			 * @param x マクロブロック左上 x座標
			 * @param y マクロブロック左上 y座標
			 * @param macro_block_size ブロックのサイズ
			 * @return true:マクロブロックが対象画像からはみ出す
			 */
			template <typename T>
			inline
			bool is_over_edge (
				const container<T> &imgmap,
				const int x, const int y,
				const unsigned int macro_block_size ) const
			{
				return (
					(y < 0) || (x < 0) ||
					(y+macro_block_size > imgmap.height()) ||
					(x+macro_block_size > imgmap.width()) );
			}

			/**
			 * 2ブロック間の差分絶対値和
			 *
			 * @param map1 ブロック1
			 * @param x1 ブロック1の左上 x座標
			 * @param y1 ブロック1の左上 y座標
			 * @param map2 ブロック2
			 * @param x2 ブロック2の左上 x座標
			 * @param y2 ブロック2の左上 y座標
			 * @param macro_block_size ブロックのサイズ
			 * @return 差分絶対値和
			 */
			template <typename T>
			inline
			double sum_of_absolute_difference (
				const container<T> &map1, const int x1, const int y1,
				const container<T> &map2, const int x2, const int y2,
				const unsigned int block_size ) const
			{
				double sum = 0;

				for (int iy=0; iy<block_size; ++iy) {
					for (int ix=0; ix<block_size; ++ix) {
						sum += abs(map1(x1+ix, y1+iy) - map2(x2+ix, y2+iy));
					}
				}

				return sum;
			}
		};

		/**
		 * 検出アルゴリズム full search
		 */
		struct full : public _base_search_algorithm
		{
			/**
			 * @param premap 原画像
			 * @param crtmap 次画像
			 * @param x マクロブロック左上 x座標
			 * @param y マクロブロック左上 y座標
			 * @param macro_block_size ブロックのサイズ
			 * @param search_size ブロックの探索範囲
			 * @return 動きベクトル
			 */
			template <typename T>
			ve_pair operator() (
				const container<T> &premap,
				const container<T> &crtmap,
				const int x, const int y,
				const unsigned int macro_block_size,
				const unsigned int search_size ) const
			{
				double sad = std::numeric_limits<double>::max();
				int vex = 0;
				int vey = 0;

				// -search_size 〜 search_size の範囲で探索
				int search = static_cast<int>(search_size);
				for (int dy = -search; dy <= search; ++dy) {
					for (int dx = -search; dx <= search; ++dx) {
						//画像端は処理対象外
						if (is_over_edge(premap, x+dx, y+dy, macro_block_size)) {
							continue;
						}

						//誤差計算
						double sum = sum_of_absolute_difference (
							crtmap, x, y,
							premap, x+dx, y+dy,
							macro_block_size
						);

						//ベクトル保存
						if (sad > sum) {
							sad = sum;
							vex = dx;
							vey = dy;
						}
					}
				}

				return {vex, vey};
			}
		};

		/**
		 * 検出アルゴリズム three step search
		 */
		struct three_step : public _base_search_algorithm
		{
			/**
			 * @param premap 原画像
			 * @param crtmap 次画像
			 * @param x マクロブロック左上 x座標
			 * @param y マクロブロック左上 y座標
			 * @param macro_block_size ブロックのサイズ
			 * @param search_size ブロックの探索範囲
			 * @return 動きベクトル
			 */
			template <typename T>
			ve_pair operator() (
				const container<T> &premap,
				const container<T> &crtmap,
				const int x, const int y,
				const unsigned int macro_block_size,
				const unsigned int search_size ) const
			{
				//中心点の誤差計算
				double sad = sum_of_absolute_difference (
					crtmap, x, y,
					premap, x, y,
					macro_block_size
				);
				int vex = 0;
				int vey = 0;

				// n = 4, 2, 1 で近傍探索
				for (int n=4; n > 0; n >>= 1) {
					int px = vex;
					int py = vey;

					//n-step 8近傍セルと比較
					for (int dy = -n; dy <= n; dy += n) {
						for (int dx = -n; dx <= n; dx += n) {
							//画像端と画像中央は処理対象外
							if (is_over_edge(premap, x+px+dx, y+py+dy, macro_block_size)
								|| (dy == 0 && dx == 0))
							{
								continue;
							}

							//誤差計算
							double sum = sum_of_absolute_difference (
								crtmap, x, y,
								premap, x+px+dx, y+py+dy,
								macro_block_size
							);

							//ベクトル保存
							if (sad > sum) {
								sad = sum;
								vex = px+dx;
								vey = py+dy;
							}
						}
					}
				}

				return {vex, vey};
			}
		};

		/**
		 * 検出アルゴリズム ベースクラス (テンプレート検索)
		 */
		struct _shape_based_algorithm : public _base_search_algorithm
		{
			/**
			 * @param premap 原画像
			 * @param crtmap 次画像
			 * @param x マクロブロック左上 x座標
			 * @param y マクロブロック左上 y座標
			 * @param macro_block_size ブロックのサイズ
			 * @param search_size ブロックの探索範囲
			 * @param ldsp 検索範囲テンプレート (広範囲用)
			 * @param sdsp 検索範囲テンプレート (狭範囲用)
			 * @return 動きベクトル
			 */
			template <typename T, typename E>
			ve_pair operator() (
				const container<T> &premap,
				const container<T> &crtmap,
				const int x, const int y,
				const unsigned int macro_block_size,
				const unsigned int search_size,
				const std::vector<std::pair<E, E>> &ldsp,
				const std::vector<std::pair<E, E>> &sdsp ) const
			{
				int px = 0, py = 0;
				int vex = 0, vey = 0;

				container<bool> is_searched (
					macro_block_size*2+1,
					macro_block_size*2+1
				);

				//中心点の誤差計算
				double sad = sum_of_absolute_difference (
					crtmap, x, y,
					premap, x, y,
					macro_block_size
				);
				is_searched(macro_block_size, macro_block_size) = true;

				//主要処理を関数化
				auto main_search_func = [&](const std::vector<std::pair<E, E>> &map) {
					//マップ上を探索
					for (auto i = map.begin(); i != map.end(); ++i) {
						int dx = i->first;
						int dy = i->second;

						//画像端と処理済みは処理対象外
						if (is_over_edge(premap, x+px+dx, y+py+dy, macro_block_size)
							|| is_searched(px+dx+macro_block_size, py+dy+macro_block_size))
						{
							continue;
						}

						//誤差計算
						double sum = sum_of_absolute_difference (
							crtmap, x, y,
							premap, x+px+dx, y+py+dy,
							macro_block_size
						);
						is_searched(px+dx+macro_block_size, py+dy+macro_block_size) = true;

						//ベクトル保存
						if (sad > sum) {
							sad = sum;
							vex = px+dx;
							vey = py+dy;
						}
					}
				};

				//LDSP上を検索
				while (true) {
					main_search_func(ldsp);

					if (px == vex && py == vey) {
						break;
					}
					else {
						px = vex;
						py = vey;
					}
				}

				//SDSP上を検索
				main_search_func(sdsp);

				return {vex, vey};
			}
		};

		/**
		 * 検出アルゴリズム greedy search
		 */
		struct greedy : public _shape_based_algorithm
		{
			/**
			 * @param premap 原画像
			 * @param crtmap 次画像
			 * @param x マクロブロック左上 x座標
			 * @param y マクロブロック左上 y座標
			 * @param macro_block_size ブロックのサイズ
			 * @param search_size ブロックの探索範囲
			 * @return 動きベクトル
			 */
			template <typename T>
			ve_pair operator() (
				const container<T> &premap,
				const container<T> &crtmap,
				const int x, const int y,
				const unsigned int macro_block_size,
				const unsigned int search_size ) const
			{
				std::vector<ve_pair> ldsp = {
					{-1,-1}, { 0,-1}, { 1,-1},
					{-1, 0}, { 0, 0}, { 1, 0},
					{-1, 1}, { 0, 1}, { 1, 1}
				};
				std::vector<ve_pair> sdsp = {};

				return _shape_based_algorithm::operator() (
					premap, crtmap, x, y,
					macro_block_size, search_size,
					ldsp, sdsp
				);
			}
		};

		/**
		 * 検出アルゴリズム diamond search
		 */
		struct diamond : public _shape_based_algorithm
		{
			/**
			 * @param premap 原画像
			 * @param crtmap 次画像
			 * @param x マクロブロック左上 x座標
			 * @param y マクロブロック左上 y座標
			 * @param macro_block_size ブロックのサイズ
			 * @param search_size ブロックの探索範囲
			 * @return 動きベクトル
			 */
			template <typename T>
			ve_pair operator() (
				const container<T> &premap,
				const container<T> &crtmap,
				const int x, const int y,
				const unsigned int macro_block_size,
				const unsigned int search_size ) const
			{
				std::vector<ve_pair> ldsp = {
					{-2, 0}, {-1, 1}, { 0, 2}, { 1, 1},
					{ 2, 0}, { 1,-1}, { 0,-2}, {-1,-1}
				};
				std::vector<ve_pair> sdsp = {
					{-1, 0}, { 0, 1}, { 1, 0}, { 0,-1}
				};

				return _shape_based_algorithm::operator() (
					premap, crtmap, x, y,
					macro_block_size, search_size,
					ldsp, sdsp
				);
			}
		};

		/**
		 * 検出アルゴリズム hexagon based search
		 */
		struct hexagon : public _shape_based_algorithm
		{
			/**
			 * @param premap 原画像
			 * @param crtmap 次画像
			 * @param x マクロブロック左上 x座標
			 * @param y マクロブロック左上 y座標
			 * @param macro_block_size ブロックのサイズ
			 * @param search_size ブロックの探索範囲
			 * @return 動きベクトル
			 */
			template <typename T>
			ve_pair operator() (
				const container<T> &premap,
				const container<T> &crtmap,
				const int x, const int y,
				const unsigned int macro_block_size,
				const unsigned int search_size ) const
			{
				std::vector<ve_pair> ldsp = {
					{-2, 0}, {-1, 2}, { 1, 2},
					{ 2, 0}, { 1,-2}, {-1,-2},
				};
				std::vector<ve_pair> sdsp = {
					{-1, 0}, { 0, 1}, { 1, 0}, { 0,-1}
				};

				return _shape_based_algorithm::operator() (
					premap, crtmap, x, y,
					macro_block_size, search_size,
					ldsp, sdsp
				);
			}
		};

	}
}

#endif
/* vim: set ts=2 sw=2 sts=2 noexpandtab ff=unix ft=cpp fenc=utf-8 : */
