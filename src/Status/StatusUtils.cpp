#include "Status/StatusUtils.h"
#include "Checker.h"

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

} // namespace

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

bool checkStatus(Checker& checker, const Status& status)
{
	for (const Point &pp: status.state()) {
		if (!checker.check(status, pp)) {
			return false;
		}
	}
	return true;
}

bool checkState(Checker& checker, const FixedTable::Ptr& table, const State& state)
{
	std::vector<Status::Ptr> partitions = getPartitions(table, state);
	for (const Status::Ptr& status: partitions) {
		if (checkStatus(checker, *status)) {
			return true;
		}
	}
	return false;
}




