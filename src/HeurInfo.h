#ifndef HEURINFO_H_
#define HEURINFO_H_

#include "Status/Status.h"
#include <vector>
#include <memory>

struct HeurInfo {
	Status status_;
	int heur_;
//	HeurInfo(): heur_(0) {}
	HeurInfo(const Status& status, int heur):
		status_(status), heur_(heur)
	{}
};

typedef std::vector<std::unique_ptr<HeurInfo>> HeurList;
typedef std::shared_ptr<HeurList> HeurListPtr;


#endif /* HEURINFO_H_ */
