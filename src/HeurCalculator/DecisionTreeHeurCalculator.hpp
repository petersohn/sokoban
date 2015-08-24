#ifndef HEURCALCULATOR_DECISIONTREEHEURCALCULATOR_HPP
#define HEURCALCULATOR_DECISIONTREEHEURCALCULATOR_HPP

#include "DecisionTree/DecisionTree.hpp"

#include "HeurCalculator/HeurInfo.hpp"
#include "HeurCalculator/DecisionTreeHeurCalculatorFwd.hpp"
#include "HeurCalculator/SubStatusHeurCalculator.hpp"

#include "Status/PseudoStatus.hpp"
#include "Status/Status.hpp"

#include "util/TimeMeter.hpp"

class DecisionTreeHeurListFactory {
    typedef decisionTree::Node<PseudoStatus, float> NodeType;
    typedef decisionTree::NodeTypes<PseudoStatus, float> NodeTypes;

    NodeType decisionTree_;

    static std::vector<Point> pointList(const Table& table);

    friend class Next;
    class Next {
        const DecisionTreeHeurListFactory* owner_ = nullptr;
        std::size_t lastSize_ = 0;
        const NodeTypes::ValueList* heurList_ = nullptr;
        NodeTypes::ValueList::const_iterator iterator_;

    public:

        explicit Next(const DecisionTreeHeurListFactory* owner):
            owner_(owner)
        {}

        const NodeTypes::Value* operator()(const PseudoStatus& pseudoStatus);
    };

public:
    DecisionTreeHeurListFactory() = default;

    template <class HeurListType>
    DecisionTreeHeurListFactory(
            const Table& table,
            const HeurListType& heurList,
            boost::optional<ComplexChecker> checker,
            std::size_t maxDepth,
            std::size_t numThreads)
    {
        util::TimeMeter timeMeter;
        NodeTypes::ValueList convertedHeurList;
        convertedHeurList.reserve(heurList.size());
        boost::transform(heurList, std::back_inserter(convertedHeurList),
                [](const HeurInfo& heurInfo)
                {
                    return std::make_shared<std::pair<PseudoStatus, float>>
                            (PseudoStatus(heurInfo.first), heurInfo.second);
                });
        decisionTree_ = decisionTree::buildNode<PseudoStatus, float>(
                convertedHeurList,
                pointList(table),
                std::move(checker),
                maxDepth,
                numThreads);
        std::cerr << "Processor time: " <<
                timeMeter.processorTime() <<
                "\nReal time: " <<
                timeMeter.realTime() << std::endl;
    }

    Next operator()() const
    {
        return Next{this};
    }

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/) {
        ar & decisionTree_;
    }
};

#endif // HEURCALCULATOR_DECISIONTREEHEURCALCULATOR_HPP
