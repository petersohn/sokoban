#ifndef INDENTEDOUTPUT_H_
#define INDENTEDOUTPUT_H_

#include <ostream>
#include "Status/Status.h"
#include "Status/MockStatus.h"
#include "Dumper/DumperFunctions.h"

template <class T>
inline
void indentedOutput(std::ostream& os, const T& t, int level)
{
	for (int i = 0; i < level; ++i) {
		os << ' ';
	}
	os << t << std::endl;
}

template <>
inline
void indentedOutput(std::ostream& os, const Status& status, int level)
{
	dumpStatus(os, status, "", &status.reachableArray(), level);
}

template <>
inline
void indentedOutput(std::ostream& os, const MockStatus& status, int level)
{
	dumpStatus(os, status, "", &status.reachableArray(), level);
}



#endif /* INDENTEDOUTPUT_H_ */
