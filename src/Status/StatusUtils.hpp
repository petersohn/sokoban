#ifndef STATUSUTILS_H_
#define STATUSUTILS_H_

#include "Status.hpp"

class Checker;

void floodFill(const Status &table, Point p0, Array<bool> &result,
			Status::BorderType *border = NULL, MinMax *minmax = NULL);
std::vector<Status> getPartitions(const Table& table, const State &state);
bool checkStatus(Checker& checker, const Status& status);
bool checkState(Checker& checker, const Table& table, const State& state);



template <class Status1, class Status2>
inline bool isSubStatus(const Status1& subStatus, const Status2& status)
{
	return subStatus.reachable(status.currentPos()) &&
			isSubState(subStatus.state(), status.state());
}

template <class Status>
inline bool isStone(const Status& status, Point  p)
{
	return status.value(p) == ftStone;
}



#endif /* STATUSUTILS_H_ */
