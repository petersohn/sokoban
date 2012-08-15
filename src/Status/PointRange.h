#ifndef POINTRANGE_H_
#define POINTRANGE_H_

#include "Status/Point.h"
#include <boost/iterator/iterator_facade.hpp>

class PointRangeIterator;

class PointRange {
public:
	typedef PointRangeIterator iterator;
private:
	Point front_;
	Point back_;
	int dx_;
	int dy_;
	Point end_;
public:
	typedef Point value_type;
	friend class PointRangeIterator;

	PointRange(const Point& front, const Point& back):
		front_(front),
		back_(back),
		dx_(back.x > front.x ? 1 : -1),
		dy_(back.y > front.y ? 1 : -1),
		end_(front_.x, back_.y+dy_)
	{}

	iterator begin() const;
	iterator end() const;
	const Point& front() const
	{
		return front_;
	}
	const Point& back() const
	{
		return back_;
	}
}; // class PointRange

class PointRangeIterator: public boost::iterator_facade<
		PointRangeIterator,
		Point,
		boost::bidirectional_traversal_tag,
		const Point&> {


	const PointRange& owner_;
	Point p_;

	friend class PointRange;
	friend class boost::iterator_core_access;

	PointRangeIterator(const PointRange& owner, const Point& p):
		owner_(owner),
		p_(p)
	{}
	const Point& dereference() const { return p_; }
	void increment()
	{
		if (p_.x == owner_.back_.x) {
			p_.x = owner_.front_.x;
			p_.y += owner_.dy_;
		} else {
			p_.x += owner_.dx_;
		}
	}
	void decrement()
	{
		if (p_.x == owner_.front_.x) {
			p_.x = owner_.back_.x;
			p_.y -= owner_.dy_;
		} else {
			p_.x -= owner_.dx_;
		}
	}

	bool equal(const PointRangeIterator& other) const
	{
		return p_ == other.p_;
	}
}; // class PointRangeIterator

inline
PointRange::iterator PointRange::begin() const
{
	return iterator(*this, front_);
}

inline
PointRange::iterator PointRange::end() const
{
	return iterator(*this, end_);
}


#endif /* POINTRANGE_H_ */
