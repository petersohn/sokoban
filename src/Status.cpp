#include "Status.h"

Status::Status(FixedTable::Ptr table, const VisitedState &state):
	table_(table),
	state_(state),
	fields_(table->width(), table->height()),
	reachOK(false)
{
	init();
}

Status::Status(FixedTable::Ptr table, const Node &node):
		table_(table),
		state_(node),
		fields_(table->width(), table->height()),
		reachOK(false)
{
	init();
}

void Status::init() {
	Point p;
	for (p.y = 0; p.y < table().height(); ++p.y)
		for (p.x = 0; p.x < table().width(); ++p.x)  {
			fields_[p] = table().wall(p) ? ftWall : ftFloor;
		}
	for (int i = 0; i < state_.size(); ++i) {
		fields_[state_[i]] = ftStone;
		stoneAt_[state_[i]] = i;
	}
}

void Status::calculateReachable() const {
	reachable_.reset(width(), height(), false);
	floodFill(*this, state().currentPos(), reachable_, border_);
	reachOK_ = true;
}

bool Status::addStone(const Point &p) {
	if (value(p) != ftFloor)
		return false;
	fields_[p] = ftStone;
	fieldAt_[p] = state_.addStone(p);
	reachOK_ = false;
	return true;
}

bool Status::removeStone(const Point &p) {
	if (value(p) != ftStone)
		return false;
	fields_[p] = ftFloor;
	state_.removeStone(stoneAt(p));
	reachOK_ = false;
	return true;
}

void Status::state(const VisitedState &value) {
	state_ = value;
	init();
}

bool Status::moveStone(int stone, const Point & p) {
	if (value(state()[stone]) != ftStone && value(p) != ftFloor)
		return false;
	fields_[state()[stone]] = ftFloor;
	state().moveStone(stone, p);
	if (p != problem().destination())
	{
		fields_[p] = ftStone;
		stoneAt_[p] = stone;
	}
	return true;
}

