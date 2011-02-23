#include "State.h"
#include "Node.h"
#include <boost/functional/hash.hpp>

using namespace std;



size_t hash_value(const State &state)
{
	size_t result = 0;
	boost::hash<Point> h;
	for (State::const_iterator it = state.begin();
			it != state.end(); ++it) {
		result += h(*it); // the order of elements doesn't count
	}
	return boost::hash<size_t>()(result);
}




