#ifndef HEURINFO_H_
#define HEURINFO_H_

#include "VisitedStateInfo.h"

struct HeurInfo {
	VisitedStateInfo info_;
	int heur_;
	HeurInfo(): heur_(0) {}
	HeurInfo(const VisitedStateInfo& info, int heur):
		info_(info), heur_(heur)
	{}
};


#endif /* HEURINFO_H_ */
