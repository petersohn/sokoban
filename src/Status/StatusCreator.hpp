#ifndef STATUSCREATOR_H_
#define STATUSCREATOR_H_

#include <memory>
#include <string>
#include <vector>

namespace sokoban {

class Status;
class Table;

std::pair<std::unique_ptr<Table>, Status>
createStatus(const std::vector<std::string>& lines);

std::pair<std::unique_ptr<Table>, Status> loadStatusFromFile(const char *filename);
std::pair<std::unique_ptr<Table>, Status> loadStatusFromStream(std::istream& file);

} // namespace sokoban

#endif /* STATUSCREATOR_H_ */
