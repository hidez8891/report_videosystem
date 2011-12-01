#ifndef _IMAGE_CONTAINER_
#define _IMAGE_CONTAINER_

#include <vector>
#include <cassert>

namespace Image
{

	template <
		typename T ,
		typename ContainerType = std::vector<T>
	>
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

		container()
			: _width(0), _height(0), _image()
		{
		}

		container(unsigned int width, unsigned int height)
			: _width(width), _height(height), _image(width * height)
		{
		}

		template <typename Iterator>
		container(int width, int height, Iterator start, Iterator end)
			: _width(width), _height(height), _image(start, end)
		{
			assert(width * height == _image.size());
		}

		inline
		int width() const
		{
			return _width;
		}

		inline
		int height() const
		{
			return _height;
		}

		inline
		bool operator== (const container &obj) const
		{
			return _image == obj._image;
		}

		inline
		const_reference
		operator() (unsigned int x, unsigned int y) const
		{
			assert(x < _width && y < _height);
			return _image[x + _width * y];
		}

		inline
		reference
		operator() (unsigned int x, unsigned int y)
		{
			assert(x < _width && y < _height);
			return _image[x + _width * y];
		}

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

		inline
		const_iterator begin() const
		{
			return _image.begin();
		}

		inline
		const_iterator end() const
		{
			return _image.end();
		}

		template <typename P>
		inline
		container apply(P func) const
		{
			auto tmp = *this;

			for (auto it = tmp._image.begin() ; it != tmp._image.end(); ++it) {
				*it = func(*it);
			}

			return tmp;
		}

		template <typename T1, typename T2>
		friend 
		Image::container<T2>& std::copy (
			const Image::container<T1> &src,
			unsigned int sx, unsigned int sy,
			unsigned int sw, unsigned int sh,
			Image::container<T2> &dst,
			unsigned int dx, unsigned int dy
		);

	};
};

namespace std
{
	template <typename T1, typename T2>
	inline
	Image::container<T2>& copy (
		const Image::container<T1> &src,
		unsigned int sx, unsigned int sy,
		unsigned int sw, unsigned int sh,
		Image::container<T2> &dst,
		unsigned int dx, unsigned int dy
	) {
		assert(sx + sw <= src._width && sy + sh <= src._height);
		assert(dx + sw <= dst._width && dy + sh <= dst._height);

		int sp = sx + sy * src._width;
		int dp = dx + dy * dst._width;

		for (int i = 0; i < sh; ++i) {
			std::copy(
				src._image.begin() + sp+i*src._width,
				src._image.begin() + sp+i*src._width + sw,
				dst._image.begin() + dp+i*dst._width
			);
		}

		return dst;
	}
}
#endif

