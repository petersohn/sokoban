#ifndef COMMON_H_
#define COMMON_H_

#include <exception>
#include <memory>
#include <iostream>
#include <string>
#include <deque>
#include <queue>
#include <boost/format.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>

template<class T> class Array;
class Point;
class Status;
class Node;
class Table;

template<class T>
inline std::string toStr(const T &t) {
	return (boost::format("%1%") % t).str();
}

#endif /* COMMON_H_ */
