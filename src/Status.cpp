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

void Status::calculateReachable() {
	floodFill(*this, state().currentPos(), reachable_);
	reachOK_ = true;
}

bool Status::addStone(const Point &p) {
	if (value(p) != ftFloor)
		return false;
	fields_[p] = ftStone;
	state_.
}

bool Status::removeStone(const Point &p) {

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
}

