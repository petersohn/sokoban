#ifndef SRC_SUBSTATUSHEURCALCULATOR_HPP
#define SRC_SUBSTATUSHEURCALCULATOR_HPP

#include "HeurCalculator.hpp"
#include "HeurInfo.hpp"
#include "Status/PseudoStatus.hpp"

template <typename Next>
class SubStatusHeurCalculator: public HeurCalculator {
	HeurCalculator::Ptr baseCalculator_;
	FixedTable::Ptr table_;
	Next next_;
public:
	SubStatusHeurCalculator(
			const HeurCalculator::Ptr& baseCalculator,
			FixedTable::Ptr table, Next next = Next{}):
				baseCalculator_(std::move(baseCalculator)),
				table_(std::move(table)),
				next_(std::move(next))
	{
	}

	int calculateStone(const Status &status, Point p) override
	{
		assert(status.tablePtr() == table_);
		return baseCalculator_->calculateStone(status, p);
	}

	int calculateStatus(
			const Status &status,
			const MoveDescriptor* /*moveDescriptor*/,
			const std::shared_ptr<Node>& ancestor) override
	{
		assert(status.tablePtr() == table_);
		PseudoStatus pseudoStatus(status);
		int result = 0;
		next_.start();
		while (const auto& subset = next_(pseudoStatus)) {
			if (isSubStatus(subset->first, pseudoStatus)) {
				result += subset->second;
				for (Point  p: subset->first.state()) {
					pseudoStatus.state().removeStone(p);
				}
			}
		}
		for (Point  p: pseudoStatus.state()) {
			result += baseCalculator_->calculateStone(status, p);
		}
		if (ancestor) {
			return std::max(result, ancestor->heur() - 1);
		}
		return result;
	}

};

#endif /* SRC_SUBSTATUSHEURCALCULATOR_HPP */
