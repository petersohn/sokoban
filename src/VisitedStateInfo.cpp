#include "VisitedStateInfo.h"
#include "Status/Status.h"
#include "Dumper/DumperFunctions.h"


VisitedStateInfo::VisitedStateInfo(const Status& status):
	state_(status.state())
{
	Point p;
	for (p.x = 0; p.x < status.width(); ++p.x) {
		for (p.y = 0; p.y < status.height(); ++p.y) {
			if (status.reachable(p)) {
				firstReachable_ = p;
				return;
			}
		}
	}
	dumpStatus(std::cerr, status);
	assert(false && "Impossible status");
}


bool operator==(const VisitedStateInfo& left, const VisitedStateInfo& right)
{
	return left.firstReachable() == right.firstReachable() && left.state() == right.state();
}






