#ifndef SOLUTIONCHECKER_H_
#define SOLUTIONCHECKER_H_

#include <queue>
#include <boost/shared_ptr.hpp>

class Status;
class Node;


bool checkResult(const Status& initialStatus, const std::deque<boost::shared_ptr<Node> >& solution);



#endif /* SOLUTIONCHECKER_H_ */
