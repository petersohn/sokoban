#include "IndexedStatusList.hpp"
#include "Status/StatusCreator.hpp"
#include "CreateTestStatus.hpp"
#include <boost/test/unit_test.hpp>

struct IndexedStatusListTestFixture {
	IndexedStatusList<int> indexedStatusListUnderTest;
	std::unique_ptr<Table> table;

	IndexedStatusListTestFixture()
	{
		auto data = createStatus(3, 3, {
				"x..",
				"y.o",
				"..."});
		table = std::move(data.first);
	}
};

BOOST_FIXTURE_TEST_SUITE(IndexedStatusListTest, IndexedStatusListTestFixture)

BOOST_AUTO_TEST_CASE(hasSubStatus_empty_list)
{
	auto status = createTestStatus(*table, {
			".o.",
			"..o",
			"y.."});
	BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(status, Point{1, 0}));
	BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(status, Point{2, 1}));
}

BOOST_AUTO_TEST_CASE(hasSubStatus_contains_one_status)
{
	auto status = createTestStatus(*table, {
			".o.",
			"..o",
			"y.."});
	indexedStatusListUnderTest.add(Status{status}, 0);
	BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status, Point{1, 0}));
	BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status, Point{2, 1}));
}

BOOST_AUTO_TEST_SUITE_END()


