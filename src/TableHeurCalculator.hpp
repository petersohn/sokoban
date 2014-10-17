#ifndef TABLEHEURCALCULATOR_H_
#define TABLEHEURCALCULATOR_H_

#include "HeurCalculator.hpp"
#include "Array.hpp"
#include "Status/Status.hpp"
#include "Status/Table.hpp"
#include "Mutexes.hpp"
#include <boost/scoped_ptr.hpp>
#include <boost/thread/locks.hpp>

template <typename StoneCalculator>
class TableHeurCalculator: public HeurCalculator {
	StoneCalculator calculator_;
public:
	TableHeurCalculator(StoneCalculator calculator):
		calculator_(std::move(calculator))
	{}
	int calculateStatus(
			const Status& status,
			const MoveDescriptor*,
			const std::shared_ptr<Node>&) const override
	{
		int result = 0;
		for (auto stone: status.state()) {
			int val = calculator_(status, stone);
			if (val < 0)
				return -1;
			result += val;
		}
		return result;
	}
	int calculateStone(const Status& status, Point p) const override
	{
		return calculator_(status, p);
	}

};

#endif /* TABLEHEURCALCULATOR_H_ */
