#include "Common.h"
#include "Array.h"
#include "Status.h"
#include "Node.h"
#include "Table.h"
#include <algorithm>
#include <boost/format.hpp>


using namespace std;

std::string direction(const Point &d) {
	return
			d.x > 0 ? "right" :
			d.x < 0 ? "left" :
			d.y > 0 ? "down" :
			d.y < 0 ? "up" : "???";
}
