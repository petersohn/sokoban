#ifndef TABLE_H_
#define TABLE_H_

#include "Common.hpp"
#include "Array.hpp"
#include <memory>

class Table {
public:
	typedef std::shared_ptr<Table> Ptr;
private:
	Array<bool> walls_;
	Point destination_;
public:
	Table() {}
	Table(size_t width, size_t height):walls_(width, height, true) {}

	size_t width() const { return walls_.width(); }
	size_t height() const { return walls_.height(); }
	bool wall(Point p) const {
		return arrayAt<bool>(walls_, p, true);
	}
	Point  destination() const { return destination_; }

	void wall(Point p, bool value) {
		walls_[p] = value;
	}
	void destination(Point  p) {
		assert(p.x >= 0 && p.x < static_cast<int>(width()) && 
				p.y >= 0 && p.y < static_cast<int>(height()));
		destination_ = p;
	}
};


typedef FixedObject<Table> FixedTable;


#endif /* TABLE_H_ */