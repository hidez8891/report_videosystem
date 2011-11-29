#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "../image/container.hpp"

using namespace Image;
using namespace std;
using namespace boost::unit_test_framework;

BOOST_AUTO_TEST_CASE(container_create_default)
{
	container<float> c;
	BOOST_CHECK_EQUAL(c.width()  , 0);
	BOOST_CHECK_EQUAL(c.height() , 0);
}

