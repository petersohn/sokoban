#ifndef QUEUE_INTERFACES_H_
#define QUEUE_INTERFACES_H_

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

template <class T>
class Pusher {
public:
	typedef boost::shared_ptr<Pusher> Ptr;
	virtual ~Pusher() {}
	virtual void push(const T&) = 0;
	virtual size_t size() = 0;
};

template <class T>
class Queue: public Pusher<T> {
public:
	typedef boost::shared_ptr<Queue> Ptr;
	virtual ~Queue() {}
	virtual T pop() = 0;
};

template <class T>
class Holder: public Pusher<T> {
public:
	typedef boost::shared_ptr<Holder<T> > Ptr;
	virtual ~Holder() {}
	virtual bool hasElem(const T&) const = 0;
};

class Node;
class Status;

typedef Pusher<boost::shared_ptr<Node> > NodePusher;
typedef Queue<boost::shared_ptr<Node> > NodeQueue;

typedef std::pair<const Status&, int> VisitedStateInput;
typedef Pusher<VisitedStateInput> VisitedStatePusher;
typedef Holder<VisitedStateInput> VisitedStateHolder;


#endif /* INTERFACES_H_ */
