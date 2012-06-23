#include <boost/test/unit_test.hpp>
#include <DecisionTree.h>
#include <vector>
#include <set>


BOOST_AUTO_TEST_SUITE(DecisionTreeTest)

typedef std::set<int> Key;
typedef std::pair<Key, int> Value;

bool inSet(const Key& key, int arg) {
	return key.count(arg) > 0;
}

BOOST_AUTO_TEST_CASE(basic)
{
	std::vector<Value> valueList = {
			std::make_pair(Key{1,2,3}, 1),
			std::make_pair(Key{1,2,5}, 2),
			std::make_pair(Key{2,4,5}, 3),
			std::make_pair(Key{3,4,5}, 4),
			std::make_pair(Key{1,3,5}, 5)
	};
	std::vector<int> args = {1,2,3,4,5};
	auto decisionTree = decisionTree::buildNode(inSet, valueList, args);
	BOOST_CHECK_EQUAL(*decisionTree->get({1,2,3}), 1);
	BOOST_CHECK_EQUAL(*decisionTree->get({1,2,5}), 2);
	BOOST_CHECK_EQUAL(*decisionTree->get({2,4,5}), 3);
	BOOST_CHECK_EQUAL(*decisionTree->get({3,4,5}), 4);
	BOOST_CHECK_EQUAL(*decisionTree->get({1,3,5}), 5);
}

BOOST_AUTO_TEST_SUITE_END()
