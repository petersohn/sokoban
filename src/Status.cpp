#include "Status.h"

Status::Status(const Problem &problem):
	problem_(new Problem(problem)),
	state_(problem_->beginningState()),
	reachOK(false)
{
	calculate();
}

void Status::calculate() {

}

void Status::calculateReachable() {

}

bool Status::moveStone(int stone, const Point & p) {

}

bool Status::step(int stone, const Point &d) {

}
