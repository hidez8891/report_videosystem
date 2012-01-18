#define NDEBUG
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cmath>
#include <map>

#include "image/container.hpp"
#include "image/io.hpp"
#include "image/math.hpp"
#include "image/utils.hpp"
#include "image/algorithm.hpp"

#if defined(MODE_FULL)
	auto func = Image::search::full();
	std::string mode = "Full Search";
#elif defined(MODE_TSS)
	auto func = Image::search::three_step();
	std::string mode = "Three Step Search";
#elif defined(MODE_GS)
	auto func = Image::search::greedy();
	std::string mode = "Greedy Search";
#elif defined(MODE_DS)
	auto func = Image::search::diamond();
	std::string mode = "Diamond Search";
#elif defined(MODE_HEX)
	auto func = Image::search::hexagon();
	std::string mode = "Hexagon-based Search";
#else
	#error 検索アルゴリズムを定義してください
#endif

/**
 * main関数
 */
int main(int argc, char* argv[])
{
	//ターゲットファイル名
	std::vector<std::string> img_files;

	//デフォルト設定: 動き保証のパラメータ
	unsigned int block_size  = 16;
	unsigned int search_size = 7;

	//デフォルト設定: 対象画像サイズ
	unsigned int width  = 352;
	unsigned int height = 288;

	//コマンドライン引数の確認
	if (argc < 3) {
		std::cout
			<< "Usage: " << argv[0] << " initial-file other-files..."
			<< std::endl;
		return 0;
	}

	//設定: 出力設定
	std::cout.precision(6);
	std::cout.setf(std::ios::fixed, std::ios::floatfield);

	//初期画像の読み込み
	auto premap = Image::load<float>(argv[1], width, height);
	// Image::write(std::string(argv[1]) + ".pgm", premap);

	//設定の表示
	std::cout << "Initial file:" << argv[1] << std::endl;
	std::cout << "File width: " << width << std::endl;
	std::cout << "File height: " << height << std::endl;
	std::cout << "Macro block size: " << block_size << std::endl;
	std::cout << "Search pixel size: " << search_size << std::endl;
	std::cout << "Algorithm: " << mode << std::endl;
	std::cout << "-----" << std::endl;

	for (int i=2; i < argc; ++i) {
		//対象画像の読み込み
		auto crtmap = Image::load<float>(argv[i], width, height);
		// Image::write(std::string(argv[i]) + ".pgm", crtmap);

		//動きベクトル予測
		double info;
		auto vec = Image::motion_vector_search(
		   premap, crtmap, block_size, search_size, func, &info
		);

		//予測画像の作成
		auto mcmap = Image::prediction(premap, vec, block_size);

		//PSNRを計算
		double mse = sum(pow(mcmap - crtmap, 2.0)) / (crtmap.width() * crtmap.height());
		double psnr = 20.0 * std::log10(255.0 / std::sqrt(mse));

		//PSNRと平均マッチング回数の出力
		std::cout << "[" << argv[i] << "] PSNR = " << psnr
		          << " Match = " << info << std::endl;

		//元画像 ←  対象画像
		premap = std::move(crtmap);
	}

	return 0;
}

/* vim: set ts=2 sw=2 sts=2 noexpandtab ff=unix ft=cpp fenc=utf-8 : */
