#include "PrioQueue.hpp"
#include <boost/test/unit_test.hpp>

struct IntPtrCompare {
	bool operator()(const std::shared_ptr<int>& lhs,
			const std::shared_ptr<int>& rhs) {
		return *lhs > *rhs;
	}
};

struct PrioQueueTestFixture {
	PrioQueue<std::shared_ptr<int>, IntPtrCompare> queueUnderTest;
};

BOOST_FIXTURE_TEST_SUITE(PrioQueueTest, PrioQueueTestFixture)

BOOST_AUTO_TEST_CASE(empty_at_the_beginning)
{
	BOOST_CHECK_EQUAL(queueUnderTest.size(), 0);
	auto peeked = queueUnderTest.peek();
	BOOST_CHECK_EQUAL(peeked.get(), (int*)nullptr);
	auto popped = queueUnderTest.pop();
	BOOST_CHECK_EQUAL(popped.get(), (int*)nullptr);
}

BOOST_AUTO_TEST_CASE(add_and_peek_one_value)
{

	auto value = std::make_shared<int>(1);
	queueUnderTest.push(value);
	BOOST_CHECK_EQUAL(queueUnderTest.size(), 1);
	auto peeked = queueUnderTest.peek();
	BOOST_CHECK_EQUAL(peeked.get(), value.get());
	BOOST_CHECK_EQUAL(queueUnderTest.size(), 1);
}

BOOST_AUTO_TEST_CASE(add_and_pop_one_value)
{
	auto value = std::make_shared<int>(1);
	queueUnderTest.push(value);
	BOOST_CHECK_EQUAL(queueUnderTest.size(), 1);
	auto peeked= queueUnderTest.pop();
	BOOST_CHECK_EQUAL(peeked.get(), value.get());
	BOOST_CHECK_EQUAL(queueUnderTest.size(), 0);
}

BOOST_AUTO_TEST_SUITE_END()




