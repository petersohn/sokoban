#include "PrioNodeQueue.hpp"
#include "Status/StatusCreator.hpp"
#include "ArrayIO.hpp"
#include "FakeNodeFactory.hpp"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(PrioNodeQueueTest)

struct BaseFixture {
	FakeNodeFactory nodeFactory;
};

struct SimpleFixture: BaseFixture {
	PrioNodeQueue queueUnderTest{{}};
};

BOOST_FIXTURE_TEST_SUITE(SimpleTests, SimpleFixture)

BOOST_AUTO_TEST_CASE(empty_at_the_beginning)
{
	BOOST_CHECK_EQUAL(queueUnderTest.size(), 0);
	auto peekedNode = queueUnderTest.peek();
	BOOST_CHECK_EQUAL(peekedNode.get(), (Node*)nullptr);
	auto poppedNode = queueUnderTest.pop();
	BOOST_CHECK_EQUAL(poppedNode.get(), (Node*)nullptr);
}

BOOST_AUTO_TEST_SUITE_END() // SimpleTests

BOOST_AUTO_TEST_SUITE_END() // PrioNodeQueueTest




