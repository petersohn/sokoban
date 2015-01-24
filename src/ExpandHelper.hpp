#ifndef SRC_EXPANDHELPER_HPP
#define SRC_EXPANDHELPER_HPP

#include "Status/Point.hpp"
#include "Status/Status.hpp"

template <typename Function>
void expandStatus(const Status& status, Function expandFunction)
{
	for (const auto& stone: status.state())
	{
		if (stone == status.table().destination())
			continue;
		expandFunction(stone, p10);
		expandFunction(stone, -p10);
		expandFunction(stone, p01);
		expandFunction(stone, -p01);
	}
}


#endif /* SRC_EXPANDHELPER_HPP */
