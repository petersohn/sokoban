#include "ExpandHelper.hpp"
#include "Status/Status.hpp"
#include "Status/StatusCreator.hpp"
#include <boost/test/unit_test.hpp>
#include <turtle/mock.hpp>

BOOST_AUTO_TEST_SUITE(ExpandHelperTest)

struct ExpandStatusFixture {
	~ExpandStatusFixture()
	{
		mock::verify();
		mock::reset();
	}
};

BOOST_FIXTURE_TEST_SUITE(ExpandStatus, ExpandStatusFixture)

MOCK_FUNCTION(mockExpandFunction, 2, void(Point, Point));

BOOST_AUTO_TEST_CASE(expandStatus_expand_one_stone)
{
	auto data = createStatus(4, 3,
			{"x...",
			".o..",
			"y..."});

	MOCK_EXPECT(mockExpandFunction).once().with(Point(1, 1), Point(1, 0));
	MOCK_EXPECT(mockExpandFunction).once().with(Point(1, 1), Point(-1, 0));
	MOCK_EXPECT(mockExpandFunction).once().with(Point(1, 1), Point(0, 1));
	MOCK_EXPECT(mockExpandFunction).once().with(Point(1, 1), Point(0, -1));

	expandStatus(data.second, mockExpandFunction);
}

BOOST_AUTO_TEST_CASE(expandStatus_expand_more_stones)
{
	auto data = createStatus(4, 3,
			{"xo..",
			"...o",
			"y..."});

	MOCK_EXPECT(mockExpandFunction).once().with(Point(1, 0), Point(1, 0));
	MOCK_EXPECT(mockExpandFunction).once().with(Point(1, 0), Point(-1, 0));
	MOCK_EXPECT(mockExpandFunction).once().with(Point(1, 0), Point(0, 1));
	MOCK_EXPECT(mockExpandFunction).once().with(Point(1, 0), Point(0, -1));

	MOCK_EXPECT(mockExpandFunction).once().with(Point(3, 1), Point(1, 0));
	MOCK_EXPECT(mockExpandFunction).once().with(Point(3, 1), Point(-1, 0));
	MOCK_EXPECT(mockExpandFunction).once().with(Point(3, 1), Point(0, 1));
	MOCK_EXPECT(mockExpandFunction).once().with(Point(3, 1), Point(0, -1));

	expandStatus(data.second, mockExpandFunction);
}

BOOST_AUTO_TEST_SUITE_END() // ExpandStatus

BOOST_AUTO_TEST_SUITE_END() //ExpandHelperTest


