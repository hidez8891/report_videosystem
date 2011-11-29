#include <iostream>

int main(void)
{
	//オプションいる？

	//初期画像の読み込み
	auto premap = Image::load<float>(filename);

	for (int id = headframe + skipframe; id <= tailframe; id += skipframe) {
		//対象画像の読み込み
		auto crtmap = Image::load<float>(filename);

		//動きベクトル予測
		auto vec = Image::full_search(premap, crtmap);

		//予測画像の作成
		auto mcmap = Image::prediction(premap, vec);

		//PSNRを計算
		double psnr = pow(mcmap - crtmap, 2.0) / (crtmap.width() * crtmap.height());

		//元画像 ←  対象画像
		premap = std::move(crtmap);
	}
}

