#include "Checker/ComplexCheckerFwd.hpp"
#include "Checker/ComplexCheckerBase.hpp"
#include "Status/StatusCreator.hpp"
#include "Status/Status.hpp"
#include "MockChecker.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/optional.hpp>
#include <array>

template <int N>
struct ComplexCheckerTestFixture {
    std::array<std::shared_ptr<MockChecker>, N> mockCheckers;
    std::unique_ptr<ComplexChecker> complexCheckerUnderTest;
    std::unique_ptr<Table> table;
    boost::optional<Status> status;
    Point p{1, 1};

    ComplexCheckerTestFixture()
    {
        for (auto& mockChecker: mockCheckers) {
            mockChecker = std::make_shared<MockChecker>();
            MOCK_EXPECT(mockChecker->errorMessage).returns("mock fail");
        }

        complexCheckerUnderTest = std::make_unique<ComplexChecker>(mockCheckers);

        std::tie(table, status) = createStatus(2, 2, {
            "x.",
            "yo"});

    }
};

BOOST_AUTO_TEST_SUITE(ComplexCheckerTest)

BOOST_FIXTURE_TEST_CASE(single_checker_pass, ComplexCheckerTestFixture<1>)
{
    MOCK_EXPECT(mockCheckers[0]->check).with(*status, p).returns(true);
    BOOST_CHECK(complexCheckerUnderTest->check(*status, p));
}

BOOST_FIXTURE_TEST_CASE(single_checker_fail, ComplexCheckerTestFixture<1>)
{
    MOCK_EXPECT(mockCheckers[0]->check).with(*status, p).returns(false);
    BOOST_CHECK(!complexCheckerUnderTest->check(*status, p));
}

BOOST_FIXTURE_TEST_CASE(multiple_checkers_pass, ComplexCheckerTestFixture<3>)
{
    MOCK_EXPECT(mockCheckers[0]->check).with(*status, p).returns(true);
    MOCK_EXPECT(mockCheckers[1]->check).with(*status, p).returns(true);
    MOCK_EXPECT(mockCheckers[2]->check).with(*status, p).returns(true);
    BOOST_CHECK(complexCheckerUnderTest->check(*status, p));
}

BOOST_FIXTURE_TEST_CASE(multiple_checkers_first_fail, ComplexCheckerTestFixture<3>)
{
    MOCK_EXPECT(mockCheckers[0]->check).with(*status, p).returns(false);
    MOCK_EXPECT(mockCheckers[1]->check).with(*status, p).returns(true);
    MOCK_EXPECT(mockCheckers[2]->check).with(*status, p).returns(true);
    BOOST_CHECK(!complexCheckerUnderTest->check(*status, p));
}

BOOST_FIXTURE_TEST_CASE(multiple_checkers_middle_fail, ComplexCheckerTestFixture<3>)
{
    MOCK_EXPECT(mockCheckers[0]->check).with(*status, p).returns(true);
    MOCK_EXPECT(mockCheckers[1]->check).with(*status, p).returns(false);
    MOCK_EXPECT(mockCheckers[2]->check).with(*status, p).returns(true);
    BOOST_CHECK(!complexCheckerUnderTest->check(*status, p));
}

BOOST_FIXTURE_TEST_CASE(multiple_checkers_last_fail, ComplexCheckerTestFixture<3>)
{
    MOCK_EXPECT(mockCheckers[0]->check).with(*status, p).returns(true);
    MOCK_EXPECT(mockCheckers[1]->check).with(*status, p).returns(true);
    MOCK_EXPECT(mockCheckers[2]->check).with(*status, p).returns(false);
    BOOST_CHECK(!complexCheckerUnderTest->check(*status, p));
}

BOOST_FIXTURE_TEST_CASE(multiple_checkers_all_fail, ComplexCheckerTestFixture<2>)
{
    MOCK_EXPECT(mockCheckers[0]->check).with(*status, p).returns(false);
    MOCK_EXPECT(mockCheckers[1]->check).with(*status, p).returns(false);
    BOOST_CHECK(!complexCheckerUnderTest->check(*status, p));
}

BOOST_AUTO_TEST_SUITE_END()

