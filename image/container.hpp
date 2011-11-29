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
		const T& operator() (unsigned int x, unsigned int y) const
		{
			assert(x < _width && y < _height);
			return _image[x + width * y];
		}

		inline
		T& operator() (unsigned int x, unsigned int y)
		{
			assert(x < _width && y < _height);
			return _image[x + _width * y];
		}

		template <typename P>
		inline
		container apply(P func)
		{
			auto tmp = *this;

			for (auto it = tmp._image.begin() ; it != tmp._image.end(); ++it) {
				*it = func(*it);
			}

			return tmp;
		}

		inline
		container operator+ (const container &obj) const
		{
			assert(width == obj.width() && height == obj.height());

			auto tmp = *this;
			auto it = tmp._image.begin();
			const auto cit = obj._image.begin();

			for ( ; it != tmp._image.end(); ++it, ++cit) {
				*it += *cit;
			}

			return tmp;
		}

		inline
		container operator- (const container &obj) const
		{
			assert(width == obj.width() && height == obj.height());

			auto tmp = *this;
			auto it = tmp._image.begin();
			const auto cit = obj._image.begin();

			for ( ; it != tmp._image.end(); ++it, ++cit) {
				*it -= *cit;
			}

			return tmp;
		}

	};
};

#endif

