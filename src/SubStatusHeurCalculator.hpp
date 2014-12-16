#ifndef SRC_SUBSTATUSHEURCALCULATOR_HPP
#define SRC_SUBSTATUSHEURCALCULATOR_HPP

#include "HeurCalculator.hpp"
#include "Status/PseudoStatus.hpp"
#include "Node.hpp"

template <typename NextFactory>
class SubStatusHeurCalculator: public HeurCalculator {
	float heurMultiplier_;
	std::shared_ptr<const HeurCalculator> baseCalculator_;
	NextFactory nextFactory_;
public:
	SubStatusHeurCalculator(
			float heurMultiplier,
			const std::shared_ptr<const HeurCalculator>& baseCalculator,
			NextFactory nextFactory = NextFactory{}):
				heurMultiplier_(heurMultiplier),
				baseCalculator_(std::move(baseCalculator)),
				nextFactory_(std::move(nextFactory))
	{
	}

	float calculateStone(const Status &status, Point p) const override
	{
		return baseCalculator_->calculateStone(status, p) * heurMultiplier_;
	}

	float calculateStatus(
			const Status &status,
			const MoveDescriptor* /*moveDescriptor*/,
			const std::shared_ptr<Node>& ancestor) const override
	{
		PseudoStatus pseudoStatus(status);
		float result = 0;
		auto next = nextFactory_();
		while (const auto& subset = next(pseudoStatus)) {
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
			result = std::max(result, ancestor->heur() / heurMultiplier_ - 1.0f);
		}
		return result * heurMultiplier_;
	}

};

#endif /* SRC_SUBSTATUSHEURCALCULATOR_HPP */
