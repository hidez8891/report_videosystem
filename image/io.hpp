#ifndef _IMAGE_IO_
#define _IMAGE_IO_

#include <istream>
#include <fstream>
#include <string>

#include "container.hpp"

namespace Image
{
	class file_open_exception : public std::ios::failure
	{
	public:
		file_open_exception(const std::string &cause)
			: std::ios::failure(cause)
		{
		}
	};

	class file_read_exception : public std::ios::failure
	{
	public:
		file_read_exception(const std::string &cause)
			: std::ios::failure(cause)
		{
		}
	};

	template <typename T, typename InnerType = unsigned char>
	container<T> load(
		const std::string &filename,
		const unsigned int width,
		const unsigned int height
	){
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
}

#endif

