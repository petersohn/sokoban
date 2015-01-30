#ifndef STATUSUTILS_H_
#define STATUSUTILS_H_

#include "Status.hpp"
#include "FieldType.hpp"
#include "ComplexCheckerFwd.hpp"

std::vector<Status> getPartitions(const Table& table, const State& state,
			std::size_t maxDepth);
bool checkStatus(const ComplexChecker& checker, const Status& status);
bool checkState(const ComplexChecker& checker, const Table& table, const State& state);



template <class Status1, class Status2>
inline bool isSubStatus(const Status1& subStatus, const Status2& status)
{
	return subStatus.reachable(status.currentPos()) &&
			isSubState(subStatus.state(), status.state());
}

template <class Status>
inline bool isStone(const Status& status, Point  p)
{
	return status.value(p) == FieldType::stone;
}



#endif /* STATUSUTILS_H_ */
