#ifndef STATUSCREATOR_H_
#define STATUSCREATOR_H_

#include <memory>
#include <string>
#include <vector>

namespace sokoban {

class Status;
class Table;

std::pair<std::unique_ptr<Table>, Status>
createStatus(int width, int height, const std::vector<std::string>& lines);

std::pair<std::unique_ptr<Table>, Status> loadStatusFromFile(const char *filename);

} // namespace sokoban

#endif /* STATUSCREATOR_H_ */
