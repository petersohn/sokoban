#ifndef HEURINFO_H_
#define HEURINFO_H_

#include <vector>
#include <memory>

namespace sokoban {

class Status;

typedef std::pair<Status, float> HeurInfo;
typedef std::vector<HeurInfo> HeurList;

} // namespace sokoban

#endif /* HEURINFO_H_ */
