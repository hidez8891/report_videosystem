#ifndef _IMAGE_CONTAINER_
#define _IMAGE_CONTAINER_

#include <vector>
#include <cassert>

namespace Image
{

	/**
	 * 画像のコンテナクラス
	 */
	template <
		typename T ,
		typename ContainerType = std::vector<T> >
	class container
	{
	private:
		unsigned int _width;
		unsigned int _height;
		ContainerType _image;

	public:
		typedef T value_type;
		typedef typename ContainerType::iterator iterator;
		typedef typename ContainerType::const_iterator const_iterator;
		typedef typename ContainerType::reference reference;
		typedef typename ContainerType::const_reference const_reference;

		/**
		 * デフォルトコンストラクタ
		 *
		 */
		container ()
			: _width(0), _height(0), _image()
		{
		}

		/**
		 * デフォルトコンストラクタ
		 *
		 * @param width コンテナの横幅
		 * @param height コンテナの縦幅
		 */
		container (unsigned int width, unsigned int height)
			: _width(width), _height(height), _image(width * height)
		{
		}

		/**
		 * デフォルトコンストラクタ
		 *
		 * @param width コンテナの横幅
		 * @param height コンテナの縦幅
		 * @param start イテレータ先頭
		 * @param end イテレータ末尾
		 */
		template <typename Iterator>
		container (int width, int height, Iterator start, Iterator end)
			: _width(width), _height(height), _image(start, end)
		{
			assert(width * height == _image.size());
		}

		/**
		 * 横幅の取得
		 *
		 * @return コンテナの横幅
		 */
		inline
		int width () const
		{
			return _width;
		}

		/**
		 * 縦幅の取得
		 *
		 * @return コンテナの縦幅
		 */
		inline
		int height () const
		{
			return _height;
		}

		/**
		 * コンテナの等価評価
		 *
		 * @param obj コンテナ
		 * @return ture:等価
		 */
		inline
		bool operator== (const container &obj) const
		{
			return _image == obj._image;
		}

		/**
		 * 要素参照
		 *
		 * @param x 横方向インデックス
		 * @param y 縦方向インデックス
		 * @return 要素参照
		 */
		inline
		const_reference
		operator() (unsigned int x, unsigned int y) const
		{
			assert(x < _width && y < _height);
			return _image[x + _width * y];
		}

		/**
		 * 要素参照
		 *
		 * @param x 横方向インデックス
		 * @param y 縦方向インデックス
		 * @return 要素参照
		 */
		inline
		reference
		operator() (unsigned int x, unsigned int y)
		{
			assert(x < _width && y < _height);
			return _image[x + _width * y];
		}

		/**
		 * 加算
		 *
		 * @param obj コンテナ
		 * @return 加算結果
		 */
		inline
		container operator+ (const container &obj) const
		{
			assert(_width == obj._width && _height == obj._height);

			auto tmp = *this;
			auto it = tmp._image.begin();
			auto cit = obj._image.begin();

			for ( ; it != tmp._image.end(); ++it, ++cit) {
				*it += *cit;
			}

			return tmp;
		}

		/**
		 * 減算
		 *
		 * @param obj コンテナ
		 * @return 減算結果
		 */
		inline
		container operator- (const container &obj) const
		{
			assert(_width == obj._width && _height == obj._height);

			auto tmp = *this;
			auto it = tmp._image.begin();
			auto cit = obj._image.begin();

			for ( ; it != tmp._image.end(); ++it, ++cit) {
				*it -= *cit;
			}

			return tmp;
		}

		/**
		 * 要素参照イテレータの取得
		 *
		 * @return 先頭イテレータ
		 */
		inline
		const_iterator begin () const
		{
			return _image.begin();
		}

		/**
		 * 要素参照イテレータの取得
		 *
		 * @return 末尾イテレータ
		 */
		inline
		const_iterator end () const
		{
			return _image.end();
		}

		/**
		 * 全要素に関数を適用
		 *
		 * @param func 適用関数
		 * @return 適用結果
		 */
		template <typename P>
		inline
		container apply (P func) const
		{
			auto tmp = *this;

			for (auto it = tmp._image.begin() ; it != tmp._image.end(); ++it) {
				*it = func(*it);
			}

			return tmp;
		}

		/**
		 * friend関数宣言
		 */
		template <typename T1, typename T2>
		friend 
		Image::container<T2>& std::copy (
			const Image::container<T1> &src,
			unsigned int sx, unsigned int sy,
			unsigned int sw, unsigned int sh,
			Image::container<T2> &dst,
			unsigned int dx, unsigned int dy );

	};
};

namespace std
{
	/**
	 * コンテナに部分コンテナをコピー
	 *
	 * @param src コピー元
	 * @param sx コピー元 左上 x座標
	 * @param sy コピー元 左上 y座標
	 * @param sw コピー横幅
	 * @param sh コピー縦幅
	 * @param dst コピー先
	 * @param dx コピー先 左上 x座標
	 * @param dy コピー先 左上 y座標
	 * @return コピー先参照
	 */
	template <typename T1, typename T2>
	inline
	Image::container<T2>& copy (
		const Image::container<T1> &src,
		unsigned int sx, unsigned int sy,
		unsigned int sw, unsigned int sh,
		Image::container<T2> &dst,
		unsigned int dx, unsigned int dy )
	{
		assert(sx + sw <= src._width && sy + sh <= src._height);
		assert(dx + sw <= dst._width && dy + sh <= dst._height);

		int sp = sx + sy * src._width;
		int dp = dx + dy * dst._width;

		for (int i = 0; i < sh; ++i) {
			std::copy(
				src._image.begin() + sp+i*src._width,
				src._image.begin() + sp+i*src._width + sw,
				dst._image.begin() + dp+i*dst._width );
		}

		return dst;
	}
}

#endif
/* vim: set ts=2 sw=2 sts=2 noexpandtab ff=unix ft=cpp fenc=utf-8 : */
