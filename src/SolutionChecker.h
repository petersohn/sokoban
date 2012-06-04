#ifndef SOLUTIONCHECKER_H_
#define SOLUTIONCHECKER_H_

#include <queue>
#include <memory>

class Status;
class Node;


bool checkResult(const Status& initialStatus, const std::deque<std::shared_ptr<Node> >& solution);



#endif /* SOLUTIONCHECKER_H_ */
