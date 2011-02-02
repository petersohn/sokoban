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
		fieldAt[state[i]] = i;
	}
}

void Status::calculateReachable() {

}

bool Status::moveStone(int stone, const Point & p) {

}

bool Status::step(int stone, const Point &d) {

}
