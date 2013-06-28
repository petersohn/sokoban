#include "Status/Status.h"
#include "Status/StatusUtils.h"
#include "Node.h"

int Status::copyCount(0);
int Status::calculateReachableCount(0);

Status::Status(FixedTable::Ptr table):
	table_(table),
	state_(),
	fields_(table->get().width(), table->get().height())
{
	init();
}


Status::Status(FixedTable::Ptr table, const State &state):
	table_(table),
	state_(state),
	fields_(table->get().width(), table->get().height())
{
	init();
}

Status::Status(FixedTable::Ptr table, const Node &node):
		table_(table),
		state_(node.state()),
		currentPos_(node.from()),
		fields_(table->get().width(), table->get().height())
{
	init();
}

void Status::init() {
	Point p;
	for (const Point& p: arrayRange(table())) {
		fields_[p] =
				table().wall(p) ? ftWall : ftFloor;
	}
	for (State::const_iterator it = state_.begin(); it != state_.end(); ++it) {
		fields_[*it] = ftStone;
	}
}

void Status::calculateReachable() const {
	++calculateReachableCount;
	calculatedData.reset(new CalculatedData(width(), height()));
	floodFill(*this, currentPos_, calculatedData->reachable_,
			&calculatedData->border_);
}

bool Status::addStone(const Point &p) {
	if (value(p) != ftFloor)
		return false;
	fields_[p] = ftStone;
	state_.addStone(p);
	calculatedData.reset();
	return true;
}

bool Status::removeStone(const Point &p) {
	if (value(p) != ftStone)
		return false;
	fields_[p] = ftFloor;
	state_.removeStone(p);
	calculatedData.reset();
	return true;
}

void Status::state(const State &value) {
	state_ = value;
	calculatedData.reset();
	init();
}

bool Status::currentPos(const Point & p) {
	currentPos_ = p;
	if (calculatedData && !calculatedData->reachable_[p]) {
		calculatedData.reset();
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
	calculatedData.reset();
	return true;
}

void Status::set(const Node &node) {
	state_ = node.state();
	currentPos_ = node.from();
	calculatedData.reset();
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

