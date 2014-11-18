#ifndef QUEUE_INTERFACES_H_
#define QUEUE_INTERFACES_H_

#include <memory>
#include <boost/function.hpp>

template <class T>
class Pusher {
public:
	typedef std::shared_ptr<Pusher> Ptr;
	virtual ~Pusher() {}
	virtual void push(const T&) = 0;
	virtual size_t size() const = 0;
};

template <class T>
class Queue: public Pusher<T> {
public:
	typedef std::shared_ptr<Queue> Ptr;
	virtual ~Queue() {}
	virtual T pop() = 0;
	virtual T peek() const = 0;
};

template <class T>
class Holder {
public:
	typedef std::shared_ptr<Holder<T> > Ptr;
	virtual ~Holder() {}
	virtual bool checkAndPush(const T&) = 0;
	virtual size_t size() const = 0;
	bool empty() const { return this->size() == 0; }
};

class Node;
class Status;

typedef Pusher<std::shared_ptr<Node> > NodePusher;
typedef Queue<std::shared_ptr<Node> > NodeQueue;

typedef std::function<NodeQueue::Ptr()> QueueFactory;

#endif /* INTERFACES_H_ */
