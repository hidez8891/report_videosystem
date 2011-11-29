#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "../image/container.hpp"
#include "../image/io.hpp"
#include "../image/math.hpp"

using namespace Image;
using namespace std;
using namespace boost::unit_test_framework;

BOOST_AUTO_TEST_CASE(container_create_default)
{
	container<float> c;
	BOOST_CHECK_EQUAL(c.width()  , 0);
	BOOST_CHECK_EQUAL(c.height() , 0);
}

BOOST_AUTO_TEST_CASE(container_create_array)
{
	vector<char> a = {1, 2, 3, 4};
	container<float> c(2, 2, a.begin(), a.end());
	BOOST_CHECK_EQUAL(c.width()  , 2);
	BOOST_CHECK_EQUAL(c.height() , 2);
	BOOST_CHECK_EQUAL(c(0,0), 1.0);
	BOOST_CHECK_EQUAL(c(1,0), 2.0);
	BOOST_CHECK_EQUAL(c(0,1), 3.0);
	BOOST_CHECK_EQUAL(c(1,1), 4.0);
}

BOOST_AUTO_TEST_CASE(container_applys)
{
	vector<char> a = {1, 2, 3, 4};
	container<float> c(2, 2, a.begin(), a.end());
	container<float> d(2, 2, a.begin(), a.end());

	auto e = c+d;
	BOOST_CHECK_EQUAL(e.width()  , 2);
	BOOST_CHECK_EQUAL(e.height() , 2);
	BOOST_CHECK_EQUAL(e(0,0), 2.0);
	BOOST_CHECK_EQUAL(e(1,0), 4.0);
	BOOST_CHECK_EQUAL(e(0,1), 6.0);
	BOOST_CHECK_EQUAL(e(1,1), 8.0);

	e = c-d;
	BOOST_CHECK_EQUAL(e.width()  , 2);
	BOOST_CHECK_EQUAL(e.height() , 2);
	BOOST_CHECK_EQUAL(e(0,0), 0.0);
	BOOST_CHECK_EQUAL(e(1,0), 0.0);
	BOOST_CHECK_EQUAL(e(0,1), 0.0);
	BOOST_CHECK_EQUAL(e(1,1), 0.0);

	e = c.apply([](float a){ return a+2.0; });
	BOOST_CHECK_EQUAL(e.width()  , 2);
	BOOST_CHECK_EQUAL(e.height() , 2);
	BOOST_CHECK_EQUAL(e(0,0), 3.0);
	BOOST_CHECK_EQUAL(e(1,0), 4.0);
	BOOST_CHECK_EQUAL(e(0,1), 5.0);
	BOOST_CHECK_EQUAL(e(1,1), 6.0);
}

BOOST_AUTO_TEST_CASE(image_load_throw_file_open_exception)
{
	BOOST_CHECK_THROW(
		load<float>("./sample/image_none.dat", 0, 0),
		file_open_exception
	);
}

BOOST_AUTO_TEST_CASE(image_load_throw_file_read_exception)
{
	BOOST_CHECK_THROW(
		load<float>("./sample/image_empty.dat", 2, 2),
		file_read_exception
	);
	BOOST_CHECK_THROW(
		load<float>("./sample/image_test1.dat", 3, 2),
		file_read_exception
	);
}

BOOST_AUTO_TEST_CASE(image_load_success)
{
	auto c = load<float>("./sample/image_test1.dat", 2, 2);
	BOOST_CHECK_EQUAL(c(0,0), 48.0);
	BOOST_CHECK_EQUAL(c(1,0), 49.0);
	BOOST_CHECK_EQUAL(c(0,1), 50.0);
	BOOST_CHECK_EQUAL(c(1,1), 51.0);
}

BOOST_AUTO_TEST_CASE(math_pow)
{
	vector<char> a = {1, 2, 3, 4};
	container<float> c(2, 2, a.begin(), a.end());

	auto e = pow(c, 2.0);
	BOOST_CHECK_EQUAL(e.width()  , 2);
	BOOST_CHECK_EQUAL(e.height() , 2);
	BOOST_CHECK_EQUAL(e(0,0),  1.0);
	BOOST_CHECK_EQUAL(e(1,0),  4.0);
	BOOST_CHECK_EQUAL(e(0,1),  9.0);
	BOOST_CHECK_EQUAL(e(1,1), 16.0);
}

BOOST_AUTO_TEST_CASE(math_abs)
{
	vector<char> a = {-1, -2, -3, -4};
	container<float> c(2, 2, a.begin(), a.end());

	auto e = abs(c);
	BOOST_CHECK_EQUAL(e.width()  , 2);
	BOOST_CHECK_EQUAL(e.height() , 2);
	BOOST_CHECK_EQUAL(e(0,0), 1.0);
	BOOST_CHECK_EQUAL(e(1,0), 2.0);
	BOOST_CHECK_EQUAL(e(0,1), 3.0);
	BOOST_CHECK_EQUAL(e(1,1), 4.0);
}

