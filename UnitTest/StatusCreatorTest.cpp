#include "Status/StatusCreator.hpp"
#include "Status/Status.hpp"
#include "Exception.hpp"
#include "FieldType.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(StatusCreatorTest)

BOOST_AUTO_TEST_CASE(simple_table)
{
	auto data = createStatus(3, 2,{
			"x.o",
			"..y"
		});
	BOOST_CHECK_EQUAL(data.first->width(), 3);
	BOOST_CHECK_EQUAL(data.first->height(), 2);
	BOOST_CHECK_EQUAL(data.first->wall(Point{0, 0}), false);
	BOOST_CHECK_EQUAL(data.first->wall(Point{1, 0}), false);
	BOOST_CHECK_EQUAL(data.first->wall(Point{2, 0}), false);
	BOOST_CHECK_EQUAL(data.first->wall(Point{0, 1}), false);
	BOOST_CHECK_EQUAL(data.first->wall(Point{1, 1}), false);
	BOOST_CHECK_EQUAL(data.first->wall(Point{2, 1}), false);

	BOOST_CHECK_EQUAL(&data.second.table(), data.first.get());
	BOOST_CHECK_EQUAL(data.second.state().size(), 1);
	BOOST_CHECK_EQUAL(data.second.value(Point{2, 0}), FieldType::stone);
	BOOST_CHECK_EQUAL(data.second.currentPos(), (Point{2, 1}));
}

BOOST_AUTO_TEST_CASE(walls)
{
	auto data = createStatus(3, 2,{
			"x.o",
			"*y*"
		});
	BOOST_CHECK_EQUAL(data.first->width(), 3);
	BOOST_CHECK_EQUAL(data.first->height(), 2);
	BOOST_CHECK_EQUAL(data.first->wall(Point{0, 0}), false);
	BOOST_CHECK_EQUAL(data.first->wall(Point{1, 0}), false);
	BOOST_CHECK_EQUAL(data.first->wall(Point{2, 0}), false);
	BOOST_CHECK_EQUAL(data.first->wall(Point{0, 1}), true);
	BOOST_CHECK_EQUAL(data.first->wall(Point{1, 1}), false);
	BOOST_CHECK_EQUAL(data.first->wall(Point{2, 1}), true);

	BOOST_CHECK_EQUAL(&data.second.table(), data.first.get());
	BOOST_CHECK_EQUAL(data.second.state().size(), 1);
	BOOST_CHECK_EQUAL(data.second.value(Point{2, 0}), FieldType::stone);
	BOOST_CHECK_EQUAL(data.second.currentPos(), (Point{1, 1}));
}

BOOST_AUTO_TEST_CASE(missing_destination)
{
	BOOST_CHECK_THROW(createStatus(3, 2,{
			"..o",
			".y."
		}), SokobanException);
}

BOOST_AUTO_TEST_CASE(missing_starting_position)
{
	BOOST_CHECK_THROW(createStatus(3, 2,{
			"x.o",
			"..."
		}), SokobanException);
}

BOOST_AUTO_TEST_CASE(missing_stones)
{
	BOOST_CHECK_THROW(createStatus(3, 2,{
			"x..",
			".y."
		}), SokobanException);
}

BOOST_AUTO_TEST_SUITE_END()

