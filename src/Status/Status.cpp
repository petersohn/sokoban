#include "Status/Status.h"
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
	reachOK_ = false;
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

namespace {

class FloodFillIter {
	const Status& status;
	Array<bool>& result;
	Status::BorderType* border;
	MinMax* minmax;
public:
	FloodFillIter(const Status &status, Array<bool> &result,
		Status::BorderType *border, MinMax *minmax):
			status(status),
			result(result),
			border(border),
			minmax(minmax)
	{}
	void iter(const Point& p);
	void checkAndIter(const Point& p)
	{
		if (status.value(p) != ftFloor || result[p])
		{
			if (border != NULL && status.value(p) == ftStone)
				border->push_back(p);
		} else {
			iter(p);
		}
	}
};

void FloodFillIter::iter(const Point & p)
{
	result[p] = true;
	if (minmax != NULL) {
		minmax->minX = std::min(minmax->minX, p.x);
		minmax->maxX = std::max(minmax->maxX, p.x);
		minmax->minY = std::min(minmax->minY, p.y);
		minmax->maxY = std::max(minmax->maxY, p.y);
	}
	checkAndIter(p+Point::p10);
	checkAndIter(p+Point::pm10);
	checkAndIter(p+Point::p01);
	checkAndIter(p+Point::p0m1);
}

}

void floodFill(const Status &table, const Point &p0, Array<bool> &result,
			Status::BorderType *border, MinMax *minmax)
{
	result.fill(false);
	if (minmax != NULL) {
		minmax->minX = table.width();
		minmax->maxX = 0;
		minmax->minY = table.height();
		minmax->maxY = 0;
	}
	FloodFillIter(table, result, border, minmax).checkAndIter(p0);
}

std::vector<Status::Ptr> getPartitions(FixedTable::Ptr table, const State &state)
{
	Array<bool> kell(table->get().width(), table->get().height(), false);
	int kellNum = 0;
	Point pp;
	for (pp.y = 0; pp.y < table->get().height(); pp.y++)
		for (pp.x = 0; pp.x < table->get().width(); pp.x++)
			if (!table->get().wall(pp) && !state[pp])
			{
				kell[pp] = true;
				++kellNum;
			} else
				kell[pp] = false;
	std::vector<Status::Ptr> result;
	while (kellNum > 0) {
		for (pp.y = 0; pp.y < table->get().height(); pp.y++)
			for (pp.x = 0; pp.x < table->get().width(); pp.x++)
				if (kell[pp])
					goto ki;
ki:
		Status::Ptr status(new Status(table, state));
		status->currentPos(pp);
		for (pp.y = 0; pp.y < table->get().height(); pp.y++)
			for (pp.x = 0; pp.x < table->get().width(); pp.x++) {
				if (status->reachable(pp) && kell[pp])
				{
					kell[pp] = false;
					--kellNum;
				}
			}
		result.push_back(status);
	}
	return result;
}

bool checkStatus(const Status& status)
{
	Point p;
	for (p.x = 0; p.x < status.width(); ++p.x) {
		for (p.y = 0; p.y < status.height(); ++p.y) {
			FieldType fieldNeeded;
			if (status.table().wall(p)) {
				fieldNeeded = ftWall;
			} else if (status.state()[p]) {
				fieldNeeded = ftStone;
			} else {
				fieldNeeded = ftFloor;
			}
			if (status.value(p) != fieldNeeded) {
				return false;
			}
		}
	}
	return true;
}
