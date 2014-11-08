#include "VisitedStateInfo.hpp"
#include "Status/Status.hpp"
#include "Dumper/DumperFunctions.hpp"

VisitedStateInfo::VisitedStateInfo(const Status& status):
	state_(status.state())
{
	for (Point  p: arrayRange(status)) {
		if (status.reachable(p)) {
			firstReachable_ = p;
			return;
		}
	}
	dumpStatus(std::cerr, status);
	assert(false && "Impossible status");
}


bool operator==(const VisitedStateInfo& left, const VisitedStateInfo& right)
{
	return left.firstReachable() == right.firstReachable() && left.state() == right.state();
}






