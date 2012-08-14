#include "Status/Status.h"
#include "Status/StatusUtils.h"
#include "Node.h"

int Status::copyCount(0);
int Status::calculateReachableCount(0);

Status::Status(FixedTable::Ptr table):
	table_(table),
	state_(),
	fields_(table->get().width(), table->get().height()),
	reachOK_(false)
{
	init();
}


Status::Status(FixedTable::Ptr table, const State &state):
	table_(table),
	state_(state),
	fields_(table->get().width(), table->get().height()),
	reachOK_(false)
{
	init();
}

Status::Status(FixedTable::Ptr table, const Node &node):
		table_(table),
		state_(node.state()),
		currentPos_(node.from()),
		fields_(table->get().width(), table->get().height()),
		reachOK_(false)
{
	init();
}

void Status::init() {
	Point p;
	for (p.y = 0; p.y < table().height(); ++p.y) {
		for (p.x = 0; p.x < table().width(); ++p.x)  {
			fields_[p] =
					table().wall(p) ? ftWall : ftFloor;
		}
	}
	for (State::const_iterator it = state_.begin(); it != state_.end(); ++it) {
		fields_[*it] = ftStone;
	}
}

void Status::calculateReachable() const {
	++calculateReachableCount;
	reachable_.reset(width(), height(), false);
	border_.clear();
	floodFill(*this, currentPos_, reachable_, &border_);
	reachOK_ = true;
}

bool Status::addStone(const Point &p) {
	if (value(p) != ftFloor)
		return false;
	fields_[p] = ftStone;
	state_.addStone(p);
	reachOK_ = false;
	return true;
}

bool Status::removeStone(const Point &p) {
	if (value(p) != ftStone)
		return false;
	fields_[p] = ftFloor;
	state_.removeStone(p);
	reachOK_ = false;
	return true;
}

void Status::state(const State &value) {
	state_ = value;
	reachOK_ = false;
	init();
}

bool Status::currentPos(const Point & p) {
	currentPos_ = p;
	if (reachOK_ && !reachable_[p]) {
		reachOK_ = false;
	}
	return true;
}

bool Status::moveStone(const Point &from, const Point &to) {
	if (value(from) != ftStone && value(to) != ftFloor)
		return false;
	fields_[from] = ftFloor;
	currentPos_ = from;
	state_.removeStone(from);
	if (to != table().destination())
	{
		fields_[to] = ftStone;
		state_.addStone(to);
	}
	reachOK_ = false;
	return true;
}

void Status::set(const Node &node) {
	state_ = node.state();
	currentPos_ = node.from();
	reachOK_ = false;
	init();
}

bool Status::shiftIter(const Point &p)
{
	if (value(p) == ftFloor && reachable(p)) {
		currentPos_ = p;
		return true;
	}
	return false;
}

void Status::shiftCurrentPos()
{
	Point p = currentPos();
	if (shiftIter(p+Point::p01))
		return;
	if (shiftIter(p+Point::p0m1))
		return;
	if (shiftIter(p+Point::p10))
		return;
	if (shiftIter(p+Point::pm10))
		return;
}

