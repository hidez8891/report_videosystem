#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "../image/container.hpp"
#include "../image/io.hpp"

using namespace Image;
using namespace std;
using namespace boost::unit_test_framework;

BOOST_AUTO_TEST_CASE(container_create_default)
{
	container<float> c;
	BOOST_CHECK_EQUAL(c.width()  , 0);
	BOOST_CHECK_EQUAL(c.height() , 0);
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

