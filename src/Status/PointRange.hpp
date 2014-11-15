#ifndef POINTRANGE_H_
#define POINTRANGE_H_

#include "Status/Point.hpp"
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
	int width_, diffX_, diffY_;
	std::size_t size_;

	Point calculate(int n) const {
		return Point{begin_.x + diffX_ * n % width_, begin_.y + diffY_ * n / width_};
	}
public:
	typedef Point value_type;
	friend class PointRangeIterator;

	PointRange(Point  begin, Point  end):
		begin_(begin),
		end_(end),
		width_(std::abs(end.x - begin.x)),
		diffX_(end.x < begin.x ? -1 : 1),
		diffY_(end.y < begin.y ? -1 : 1),
		size_(std::abs((end.y - begin.y) * (end.x - begin.x)))
	{}

	iterator begin() const;
	iterator end() const;
	Point  front() const
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
		return calculate(size_ - 1);
	}
}; // class PointRange

class PointRangeIterator: public boost::iterator_facade<
		PointRangeIterator,
		Point,
		boost::bidirectional_traversal_tag,
		Point > {


	const PointRange& owner_;
	int n_;

	friend class PointRange;
	friend class boost::iterator_core_access;

	PointRangeIterator(const PointRange& owner, int n):
		owner_(owner),
		n_(n)
	{
	}

	Point  dereference() const { return owner_.calculate(n_); }
	void increment()
	{
		++n_;
	}
	void decrement()
	{
		--n_;
	}

	bool equal(const PointRangeIterator& other) const
	{
		return n_ == other.n_;
	}
}; // class PointRangeIterator

inline
PointRange::iterator PointRange::begin() const
{
	return iterator(*this, 0);
}

inline
PointRange::iterator PointRange::end() const
{
	return iterator(*this, size_);
}


#endif /* POINTRANGE_H_ */
