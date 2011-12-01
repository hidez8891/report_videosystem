#ifndef _IMAGE_MATH_
#define _IMAGE_MATH_

#include "container.hpp"
#include <cmath>
#include <numeric>

namespace std
{
	/**
	 * 指数演算
	 *
	 * @param a 適用変数
	 * @param b 指数
	 * @return 指数演算結果
	 */
	template <typename T>
	Image::container<T> pow (const Image::container<T>& a, double b)
	{
		return a.apply([b](T c) { return pow(c, b); });
	}

	/**
	 * 総和演算
	 *
	 * @param a 対象コンテナ
	 * @return 総和結果
	 */
	template <typename T>
	double sum (const Image::container<T>& a)
	{
		return std::accumulate(a.begin(), a.end(), 0.0);
	}
}

#endif
/* vim: set ts=2 sw=2 sts=2 noexpandtab ff=unix ft=cpp fenc=utf-8 : */
