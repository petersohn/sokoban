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
	int dx_;
	int dy_;
	Point back_;
	Point end_; ///< The position of the end iterator.
public:
	typedef Point value_type;
	friend class PointRangeIterator;

	PointRange(const Point& begin, const Point& end):
		begin_(begin),
		dx_(end.x > begin.x ? 1 : -1),
		dy_(end.y > begin.y ? 1 : -1),
		back_(end.x - dx_, end.y - dy_),
		end_(begin.x, end.y)
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
	const Point& back() const
	{
		if (begin_ == end_) {
			BOOST_THROW_EXCEPTION(std::out_of_range("back() cannot be called on empty PointRange."));
		}
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
			p_.x = owner_.begin_.x;
			p_.y += owner_.dy_;
		} else {
			p_.x += owner_.dx_;
		}
	}
	void decrement()
	{
		if (p_.x == owner_.begin_.x) {
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
	return iterator(*this, begin_);
}

inline
PointRange::iterator PointRange::end() const
{
	return iterator(*this, end_);
}


#endif /* POINTRANGE_H_ */
