#ifndef TABLE_H_
#define TABLE_H_

#include "Common.h"
#include "Array.h"
#include "boost/shared_ptr.hpp"

class Table {
public:
	typedef boost::shared_ptr<Table> Ptr;
private:
	Array<bool> walls_;
	Point destination_;
public:
	Table() {}
	Table(size_t width, size_t height):walls_(width, height, false) {}

	size_t width() { return walls_.width(); }
	size_t height() { return walls_.height(); }
	bool wall(const Point &p) const {
		return arrayAt<bool>(walls_, p, true);
	}
	const Point& destination() const { return destination; }

	void setWall(const Point &p, bool value) {
		walls_[p] = value;
	}
	void destination(const Point& p) {
		assert(p.x >= 0 && p.x < width() && p.y >= 0 && p.y < height());
		destination_ = p;
	}
};

typedef FixedObject<Table> FixedTable;

#endif /* TABLE_H_ */
