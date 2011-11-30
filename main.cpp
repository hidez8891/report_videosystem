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
#include "image/argorithm.hpp"

const std::string itos(const int num, const int width, const char fill)
{
	std::stringstream str;
	str << std::setw(width) << std::setfill(fill) << num;
	return str.str();
}

int main(void)
{
	//オプションいる？

	//設定: 動き保証のパラメータ
	const int brock_size  = 16;
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
		auto vec = Image::full_search(premap, crtmap, brock_size, search_size);

		//予測画像の作成
		auto mcmap = Image::prediction(premap, vec, brock_size);

		//PSNRを計算
		double mse = sum(pow(mcmap - crtmap, 2.0)) / (crtmap.width() * crtmap.height());
		double psnr = 20.0 * std::log10(255.0 / std::sqrt(mse));

		//PSNRの出力
		std::cout << "[" << filename << "] PSNR = " << psnr << std::endl;

		//元画像 ←  対象画像
		premap = std::move(crtmap);
	}
}

