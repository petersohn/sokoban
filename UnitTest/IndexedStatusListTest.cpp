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

BOOST_AUTO_TEST_CASE(hasSubStatus_contains_more_statuses)
{
	auto status1 = createTestStatus(*table, {
			".o.",
			"..o",
			"y.."});
	auto status2 = createTestStatus(*table, {
			".o.",
			"o..",
			"y.."});
	auto status3 = createTestStatus(*table, {
			"...",
			".o.",
			"yoo"});
	indexedStatusListUnderTest.add(Status{status1}, 0);
	indexedStatusListUnderTest.add(Status{status2}, 0);
	indexedStatusListUnderTest.add(Status{status3}, 0);

	BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status1, Point{1, 0}));
	BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status1, Point{2, 1}));

	BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status2, Point{1, 0}));
	BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status2, Point{0, 1}));

	BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status3, Point{1, 1}));
	BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status3, Point{1, 2}));
	BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status3, Point{2, 2}));
}

BOOST_AUTO_TEST_CASE(hasSubStatus_mismatch)
{
	auto status1 = createTestStatus(*table, {
			".o.",
			"..o",
			"y.."});
	auto status2 = createTestStatus(*table, {
			".o.",
			"o..",
			"y.."});
	auto status3 = createTestStatus(*table, {
			"...",
			".o.",
			"yoo"});
	indexedStatusListUnderTest.add(Status{status1}, 0);
	indexedStatusListUnderTest.add(Status{status2}, 0);
	indexedStatusListUnderTest.add(Status{status3}, 0);

	auto notExistingStatus = createTestStatus(*table, {
			".o.",
			"...",
			"yo."});

	BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(notExistingStatus, Point{1, 0}));
	BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(notExistingStatus, Point{1, 2}));
}

BOOST_AUTO_TEST_CASE(hasSubStatus_reachability)
{
	auto statusOk = createTestStatus(*table, {
			".o.",
			"..o",
			"y.."});
	auto statusNok = createTestStatus(*table, {
			".oy",
			"..o",
			"..."});
	indexedStatusListUnderTest.add(Status{statusOk}, 0);

	BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(statusNok, Point{1, 0}));
	BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(statusNok, Point{2, 1}));
}

BOOST_AUTO_TEST_SUITE_END()


