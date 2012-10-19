#ifndef POINTRANGE_H_
#define POINTRANGE_H_

#include "Status/Point.h"
#include <boost/iterator/iterator_facade.hpp>
#include <boost/exception/all.hpp>
#include <stdexcept>

class PointRangeIterator;

class PointRange {
public:
	typedef PointRangeIterator iterator;
private:
	Point begin_;
	Point end_;
	int dx() const
	{
		return end_.x > begin_.x ? 1 : -1;
	}
	int dy() const
	{
		return end_.y > begin_.y ? 1 : -1;
	}
public:
	typedef Point value_type;
	friend class PointRangeIterator;

	PointRange(const Point& begin, const Point& end):
		begin_(begin),
		end_(end)
	{}

	iterator begin() const;
	iterator end() const;
	const Point& front() const
	{
		if (begin_ == end_) {
			BOOST_THROW_EXCEPTION(std::out_of_range("front() cannot be called on empty PointRange."));
		}
		return begin_;
	}
	Point back() const
	{
		if (begin_ == end_) {
			BOOST_THROW_EXCEPTION(std::out_of_range("back() cannot be called on empty PointRange."));
		}
		return Point(end_.x - dx(), end_.y - dy());
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
		p_.x += owner_.dx();
		if (p_.x == owner_.end_.x) {
			p_.x = owner_.begin_.x;
			p_.y += owner_.dy();
		}
	}
	void decrement()
	{
		if (p_.x == owner_.begin_.x) {
			p_.x = owner_.end_.x - owner_.dx();
			p_.y -= owner_.dy();
		} else {
			p_.x -= owner_.dx();
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
	return iterator(*this, begin_);
}

inline
PointRange::iterator PointRange::end() const
{
	return iterator(*this, Point(begin_.x, end_.y));
}


#endif /* POINTRANGE_H_ */
