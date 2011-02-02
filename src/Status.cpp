#include "Status.h"

Status::Status(const Problem &problem):
	problem_(new Problem(problem)),
	state_(problem_->beginningState()),
	fields_(problem()->width(), problem()->height()),
	reachOK(false)
{
	Point p;
	for (p.y = 0; p.y < problem()->height(); ++p.y)
		for (p.x = 0; p.x < problem()->width(); ++p.x)  {
			fields_[p] = problem_->table(p);
		}
	calculateStoneAt();
}

void Status::calculateStoneAt() {
	fieldAt_.reset(problem()->width(), problem()->height());
	for (int i = 0; i < state_.size(); ++i) {
		fieldAt_[state[i]] = i;
	}
}

void Status::calculateReachable() {
	floodFill(fields_, state().currentPos(), reachable_);
	reachOK_ = true;
}

bool Status::moveStone(int stone, const Point & p) {
	if (fields(state()[stone]) != ftStone && fields(p) != ftFloor)
		return false;
	fields_[state()[stone]] = ftFloor;
	state().moveStone(stone, p);
	if (p != problem().destination())
	{
		fields_[p] = ftStone;
		stoneAt_[p] = stone;
	}
}

