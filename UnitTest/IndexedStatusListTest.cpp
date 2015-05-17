#include "Status/StatusCreator.hpp"

#include "CreateTestStatus.hpp"
#include "IndexedStatusList.hpp"

#include <boost/test/unit_test.hpp>

struct IndexedStatusListTestFixture {
    IndexedStatusList indexedStatusListUnderTest;
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
    BOOST_CHECK_EQUAL(indexedStatusListUnderTest.size(), 0);
    BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(status, Point{1, 0}));
    BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(status, Point{2, 1}));
}

BOOST_AUTO_TEST_CASE(hasSubStatus_contains_one_status)
{
    auto status = createTestStatus(*table, {
            ".o.",
            "..o",
            "y.."});
    indexedStatusListUnderTest.add(Status{status});
    BOOST_CHECK_EQUAL(indexedStatusListUnderTest.size(), 1);
    BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status, Point{1, 0}));
    BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status, Point{2, 1}));
}

BOOST_AUTO_TEST_CASE(hasSubStatus_only_works_for_points_within_status)
{
    auto status = createTestStatus(*table, {
            ".o.",
            "..o",
            "y.."});
    indexedStatusListUnderTest.add(Status{status});
    BOOST_CHECK_EQUAL(indexedStatusListUnderTest.size(), 1);
    BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(status, Point{0, 0}));
    BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(status, Point{2, 0}));
    BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(status, Point{0, 1}));
    BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(status, Point{1, 1}));
    BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(status, Point{0, 1}));
    BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(status, Point{1, 2}));
    BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(status, Point{2, 2}));
}

BOOST_AUTO_TEST_CASE(hasSubStatus_finds_proper_sub_status)
{
    auto status = createTestStatus(*table, {
            ".o.",
            "..o",
            "y.."});
    indexedStatusListUnderTest.add(Status{status});
    auto superStatus = createTestStatus(*table, {
            ".o.",
            "o.o",
            "y.."});
    BOOST_CHECK_EQUAL(indexedStatusListUnderTest.size(), 1);
    BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(superStatus, Point{1, 0}));
    BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(superStatus, Point{2, 1}));
}

BOOST_AUTO_TEST_CASE(hasSubStatus_finds_proper_sub_status_only_for_points_within_sub_status)
{
    auto status = createTestStatus(*table, {
            ".o.",
            "..o",
            "y.."});
    indexedStatusListUnderTest.add(Status{status});
    auto superStatus = createTestStatus(*table, {
            ".o.",
            "o.o",
            "y.."});
    BOOST_CHECK_EQUAL(indexedStatusListUnderTest.size(), 1);
    BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(superStatus, Point{0, 1}));
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
    indexedStatusListUnderTest.add(Status{status1});
    indexedStatusListUnderTest.add(Status{status2});
    indexedStatusListUnderTest.add(Status{status3});

    BOOST_CHECK_EQUAL(indexedStatusListUnderTest.size(), 3);
    BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status1, Point{1, 0}));
    BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status1, Point{2, 1}));

    BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status2, Point{1, 0}));
    BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status2, Point{0, 1}));

    BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status3, Point{1, 1}));
    BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status3, Point{1, 2}));
    BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status3, Point{2, 2}));
}

BOOST_AUTO_TEST_CASE(hasSubStatus_works_for_multiple_proper_sub_statuses)
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
    indexedStatusListUnderTest.add(Status{status1});
    indexedStatusListUnderTest.add(Status{status2});
    indexedStatusListUnderTest.add(Status{status3});

    auto superStatus = createTestStatus(*table, {
            ".o.",
            "o.o",
            "y.."});

    BOOST_CHECK_EQUAL(indexedStatusListUnderTest.size(), 3);
    BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(superStatus, Point{1, 0}));
    BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(superStatus, Point{2, 1}));
    BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(superStatus, Point{0, 1}));

    BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(superStatus, Point{1, 1}));
    BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(superStatus, Point{1, 2}));
    BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(superStatus, Point{2, 2}));
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
    indexedStatusListUnderTest.add(Status{status1});
    indexedStatusListUnderTest.add(Status{status2});
    indexedStatusListUnderTest.add(Status{status3});

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
    indexedStatusListUnderTest.add(Status{statusOk});

    BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(statusNok, Point{1, 0}));
    BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(statusNok, Point{2, 1}));
}

BOOST_AUTO_TEST_CASE(clear_clears)
{
    auto status = createTestStatus(*table, {
            ".o.",
            "..o",
            "y.."});
    indexedStatusListUnderTest.add(Status{status});

    BOOST_CHECK_EQUAL(indexedStatusListUnderTest.size(), 1);
    BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status, Point{1, 0}));
    BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status, Point{2, 1}));

    indexedStatusListUnderTest.clear();
    BOOST_CHECK_EQUAL(indexedStatusListUnderTest.size(), 0);
    BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(status, Point{1, 0}));
    BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(status, Point{2, 1}));

}

BOOST_AUTO_TEST_CASE(add_works_after_clear)
{
    auto status1 = createTestStatus(*table, {
            ".o.",
            "..o",
            "y.."});
    auto status2 = createTestStatus(*table, {
            ".o.",
            "o..",
            "y.."});
    indexedStatusListUnderTest.add(Status{status1});
    indexedStatusListUnderTest.clear();
    indexedStatusListUnderTest.add(Status{status2});

    BOOST_CHECK_EQUAL(indexedStatusListUnderTest.size(), 1);
    BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(status1, Point{1, 0}));
    BOOST_CHECK(!indexedStatusListUnderTest.hasSubStatus(status1, Point{2, 1}));
    BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status2, Point{1, 0}));
    BOOST_CHECK(indexedStatusListUnderTest.hasSubStatus(status2, Point{0, 1}));
}

BOOST_AUTO_TEST_SUITE_END()


