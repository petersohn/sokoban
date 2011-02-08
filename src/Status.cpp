#include "Status.h"

Status::Status(FixedProblem::Ptr problem):
	problem_(problem),
	state_(problem()->beginningState()),
	fields_(problem()->width(), problem()->height()),
	reachOK(false)
{
	init();
}

Status::Status(FixedProblem::Ptr problem, const Node &node):
		problem_(problem),
		state_(node.state()),
		fields_(problem()->width(), problem()->height()),
		reachOK(false)
{
	initNode(node);
}

void Status::init() {
	Point p;
	for (p.y = 0; p.y < problem()->height(); ++p.y)
		for (p.x = 0; p.x < problem()->width(); ++p.x)  {
			fields_[p] = problem_->value(p);
		}
	for (int i = 0; i < state_.size(); ++i) {
		stoneAt_[state_[i]] = i;
	}
}

void Status::initNode(const Node &node) {
	Point p;
	for (p.y = 0; p.y < problem()->height(); ++p.y)
		for (p.x = 0; p.x < problem()->width(); ++p.x)  {
			fields_[p] = problem_->tableValue(p);
		}
	for (size_t i = 0; i < state_.size(); ++i)
		if (state_[i] != problem()->destination())
		{
			fields_[state_[i]] = ftStone;
			stoneAt_[state_[i]] = i;
		}
	state_.currentPos = state_[node.stone()] - node.d();
}

void Status::calculateReachable() {
	floodFill(*this, state().currentPos(), reachable_);
	reachOK_ = true;
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

