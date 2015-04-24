#include "Status/State.hpp"
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(IsSubStateTest)

struct Fixture {
    State state1;
    State state2;
    AddStonesToState adder1;
    AddStonesToState adder2;
    Fixture():
        adder1(state1),
        adder2(state2)
    {}
    void clear()
    {
        state1.clear();
        state2.clear();
    }
};

BOOST_FIXTURE_TEST_CASE(Equal, Fixture)
{
    BOOST_CHECK(isSubState(
            adder1(Point(1,1)).get(),
            adder2(Point(1,1)).get()
        ));
    clear();
    BOOST_CHECK(isSubState(
            adder1(Point(1,1))(Point(2,1))(Point(3,2)).get(),
            adder2(Point(1,1))(Point(2,1))(Point(3,2)).get()
        ));
}

BOOST_FIXTURE_TEST_CASE(NotEqual, Fixture)
{
    BOOST_CHECK(!isSubState(
            adder1(Point(1,1)).get(),
            adder2(Point(2,1)).get()
        ));
    clear();
    BOOST_CHECK(!isSubState(
            adder1(Point(1,1))(Point(2,1))(Point(3,2)).get(),
            adder2(Point(3,1))(Point(2,1))(Point(3,2)).get()
        ));
}

BOOST_FIXTURE_TEST_CASE(SubState, Fixture)
{
    BOOST_CHECK(isSubState(
            adder1(Point(2,1)).get(),
            adder2(Point(1,1))(Point(2,1)).get()
        ));
    clear();
    BOOST_CHECK(isSubState(
            adder1(Point(1,1)).get(),
            adder2(Point(1,1))(Point(2,1))(Point(3,2)).get()
        ));
    clear();
    BOOST_CHECK(isSubState(
            adder1(Point(1,1))(Point(3,2)).get(),
            adder2(Point(1,1))(Point(2,1))(Point(3,2)).get()
        ));
}

BOOST_FIXTURE_TEST_CASE(InverseSubState, Fixture)
{
    BOOST_CHECK(!isSubState(
            adder1(Point(1,1))(Point(2,1)).get(),
            adder2(Point(2,1)).get()
        ));
    clear();
    BOOST_CHECK(!isSubState(
            adder1(Point(1,1))(Point(2,1))(Point(3,2)).get(),
            adder2(Point(1,1)).get()
        ));
    clear();
    BOOST_CHECK(!isSubState(
            adder1(Point(1,1))(Point(2,1))(Point(3,2)).get(),
            adder2(Point(1,1))(Point(3,2)).get()
        ));
}



BOOST_AUTO_TEST_SUITE_END() // IsSubState
