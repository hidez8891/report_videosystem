#define NDEBUG
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cmath>

#include "image/container.hpp"
#include "image/io.hpp"
#include "image/math.hpp"
#include "image/utils.hpp"
#include "image/algorithm.hpp"

/**
 * 整数を文字列化
 *
 * @param num 整数
 * @param width 最小文字幅
 * @param fill 文字幅に満たない場合に埋める文字
 * @return 整数の文字列
 */
const std::string itos (const int num, const int width, const char fill)
{
	std::stringstream str;
	str << std::setw(width) << std::setfill(fill) << num;
	return str.str();
}

/**
 * main関数
 */
int main(void)
{
	//オプションいる？

	//設定: 動き保証のパラメータ
	const int block_size  = 16;
	const int search_size = 7;

	//設定: 対象画像サイズ
	const int width  = 352;
	const int height = 288;

	//設定: 対象画像名
	const std::string filepath = "../coast/coast";
	const std::string filesuffix = ".cif";
	const int headframe = 2;
	const int skipframe = 3;
	const int tailframe = 23;

	//設定: 出力設定
	std::cout.precision(6);
	std::cout.setf(std::ios::fixed, std::ios::floatfield);

	//初期画像ファイル名
	std::string filename = filepath + itos(headframe, 3, '0') + filesuffix;

	//初期画像の読み込み
	auto premap = Image::load<float>(filename, width, height);

	for (int id = headframe + skipframe; id <= tailframe; id += skipframe) {
		//対象画像ファイル名
		filename = filepath + itos(id, 3, '0') + filesuffix;

		//対象画像の読み込み
		auto crtmap = Image::load<float>(filename, width, height);

		//動きベクトル予測
		// auto func = Image::search::full();
		// auto func = Image::search::three_step();
		// auto func = Image::search::greedy();
		// auto func = Image::search::diamond();
		auto func = Image::search::hexagon();
		auto vec = Image::motion_vector_search(
		   premap, crtmap, block_size, search_size, func
		);

		//予測画像の作成
		auto mcmap = Image::prediction(premap, vec, block_size);

		//PSNRを計算
		double mse = sum(pow(mcmap - crtmap, 2.0)) / (crtmap.width() * crtmap.height());
		double psnr = 20.0 * std::log10(255.0 / std::sqrt(mse));

		//PSNRの出力
		std::cout << "[" << filename << "] PSNR = " << psnr << std::endl;

		//元画像 ←  対象画像
		premap = std::move(crtmap);
	}

	return 0;
}

/* vim: set ts=2 sw=2 sts=2 noexpandtab ff=unix ft=cpp fenc=utf-8 : */
