#ifndef _IMAGE_IO_
#define _IMAGE_IO_

#include <istream>
#include <ostream>
#include <fstream>
#include <string>

#include "container.hpp"

namespace Image
{
	/**
	 * 例外クラス ファイルオープン失敗
	 */
	class file_open_exception : public std::ios::failure
	{
	public:
		file_open_exception (const std::string &cause)
			: std::ios::failure(cause)
		{
		}
	};

	/**
	 * 例外クラス ファイル読み込み失敗
	 */
	class file_read_exception : public std::ios::failure
	{
	public:
		file_read_exception (const std::string &cause)
			: std::ios::failure(cause)
		{
		}
	};

	/**
	 * ファイルの読み込み
	 *
	 * @param filename 読み込みファイル
	 * @param width 読み込みデータの横幅
	 * @param height 読み込みデータの縦幅
	 * @return 読み込んだデータのコンテナ
	 */
	template <typename T, typename InnerType = unsigned char>
	container<T> load (
		const std::string &filename,
		const unsigned int width,
		const unsigned int height )
	{
		//読み込みファイルのオープン
		std::ifstream in(filename.c_str(), std::ios::binary);
		if (in.fail()) {
			throw file_open_exception("Can't open " + filename);
		}

		//読み込み用配列
		InnerType *buf = new InnerType[width * height];

		//読み込み
		in.read((char*)buf, sizeof(InnerType) * width * height);
		if (in.bad() || in.gcount() != sizeof(InnerType) * width * height) {
			in.close();
			throw file_read_exception("Read failed [" + filename + "]");
		}
		in.close();

		//コンテナに保存
		auto ret = container<T>(width, height, buf, buf + width * height);

		//読み込み用配列の破棄
		delete[] buf;

		return ret;
	}

	/**
	 * ファイルの書き込み
	 *
	 * @param filename 書き込みファイル
	 * @param image    書き込むデータ
	 * @return 書き込みの成功
	 */
	template <typename T, typename InnerType = unsigned char>
	bool write (
		const std::string &filename,
		const container<T> &image )
	{
		//書き込みファイルのオープン
		std::ofstream out(filename.c_str(), std::ios::binary);
		if (out.fail()) {
			throw file_open_exception("Can't open " + filename);
		}

		//書き込み
		out << "P5" << std::endl;
		out << image.width() << " " << image.height() << std::endl;
		out << "255" << std::endl;

		for (auto it = image.begin(); it != image.end(); ++it) {
			out << static_cast<char>(*it);
		}
		
		out.close();

		return true;
	}
}

#endif
/* vim: set ts=2 sw=2 sts=2 noexpandtab ff=unix ft=cpp fenc=utf-8 : */
