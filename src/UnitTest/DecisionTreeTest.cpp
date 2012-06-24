#include <boost/test/unit_test.hpp>
#include <DecisionTree/OptimalSplitting.h>
#include <vector>
#include <set>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/lexical_cast.hpp>


namespace std {

template <class T>
ostream& operator<<(ostream& s, const set<T>& set)
{
	s << "{" << boost::algorithm::join(set | boost::adaptors::transformed(boost::lexical_cast<std::string, T>), ",") << "}";
	return s;
}

}


BOOST_AUTO_TEST_SUITE(DecisionTreeTest)

typedef std::set<int> Key;
typedef std::pair<Key, int> Value;

bool inSet(const Key& key, int arg) {
	return key.count(arg) > 0;
}

class Fixture {
public:
	std::vector<Value> valueList;
	std::vector<int> args;
	std::unique_ptr<decisionTree::Node<Key, int>> tree;
	Fixture():
		valueList{
			std::make_pair(Key{1,2,3}, 1),
			std::make_pair(Key{1,2,5}, 2),
			std::make_pair(Key{2,4,5}, 3),
			std::make_pair(Key{3,4,5}, 4),
			std::make_pair(Key{1,3,5}, 5)
		},
		args{1,2,3,4,5},
		tree(decisionTree::buildNode(inSet, valueList, args,
				decisionTree::OptimalSplitting()))
	{
	}
};

BOOST_FIXTURE_TEST_CASE(basic, Fixture)
{
	BOOST_CHECK_EQUAL(tree->get({1,2,3})->second, 1);
	BOOST_CHECK_EQUAL(tree->get({1,2,5})->second, 2);
	BOOST_CHECK_EQUAL(tree->get({2,4,5})->second, 3);
	BOOST_CHECK_EQUAL(tree->get({3,4,5})->second, 4);
	BOOST_CHECK_EQUAL(tree->get({1,3,5})->second, 5);
}

BOOST_FIXTURE_TEST_CASE(extended, Fixture)
{
	BOOST_CHECK_EQUAL(tree->get({1,2,3,6,7})->second, 1);
	BOOST_CHECK_EQUAL(tree->get({1,2,5,6,8})->second, 2);
	BOOST_CHECK_EQUAL(tree->get({2,4,5,9,10})->second, 3);
	BOOST_CHECK_EQUAL(tree->get({3,4,5,8,9})->second, 4);
	BOOST_CHECK_EQUAL(tree->get({1,3,5,7,8})->second, 5);
}

//BOOST_FIXTURE_TEST_CASE(multiChoice, Fixture)
//{
//	BOOST_CHECK_EQUAL(tree->get({1,2,3,5})->second, 1);
//	BOOST_CHECK_EQUAL(tree->get({1,2,4,5})->second, 2);
//	BOOST_CHECK_EQUAL(tree->get({2,3,4,5})->second, 3);
//	BOOST_CHECK_EQUAL(tree->get({1,3,4,5})->second, 4);
//}



BOOST_AUTO_TEST_SUITE_END()
