#ifndef HEURINFO_H_
#define HEURINFO_H_

#include "Status/Status.h"
#include <vector>
#include <memory>

typedef std::pair<Status, int> HeurInfo;
typedef std::vector<std::unique_ptr<HeurInfo>> HeurList;
typedef std::shared_ptr<HeurList> HeurListPtr;


#endif /* HEURINFO_H_ */
