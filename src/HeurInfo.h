#ifndef HEURINFO_H_
#define HEURINFO_H_

#include "Status/Status.h"
#include <vector>
#include <memory>

typedef std::pair<Status, int> HeurInfo;
typedef std::shared_ptr<const HeurInfo> HeurInfoConstPtr;
typedef std::vector<HeurInfoConstPtr> HeurList;
typedef std::shared_ptr<HeurList> HeurListPtr;


#endif /* HEURINFO_H_ */
