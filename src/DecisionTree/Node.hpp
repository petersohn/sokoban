#ifndef DECISIONTREE_NODE_H_
#define DECISIONTREE_NODE_H_

#include <memory>
#include <vector>

namespace decisionTree {

template <class Status, class T>
class Node {
public:
	typedef std::pair<Status, T> Value;
	typedef std::shared_ptr<Value> ValuePtr;
	typedef std::vector<ValuePtr> ValueList;
	virtual const ValueList& get(const Status& key) const = 0;
	virtual ~Node() {}
};

}


#endif /* DECISIONTREE_NODE_H_ */
