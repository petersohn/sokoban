#ifndef DECISIONTREEHEURCALCULATOR_H_
#define DECISIONTREEHEURCALCULATOR_H_

#include "HeurCalculator/HeurInfo.hpp"
#include "DecisionTree/DecisionTree.hpp"
#include "util/TimeMeter.hpp"
#include "HeurCalculator/SubStatusHeurCalculator.hpp"
#include "Status/PseudoStatus.hpp"
#include "Status/Status.hpp"

class DecisionTreeHeurListFactory {
    typedef decisionTree::Node<PseudoStatus, float> NodeType;

    std::unique_ptr<const NodeType> decisionTree_;

    static std::vector<Point> pointList(const Table& table);

    friend class Next;
    class Next {
        const DecisionTreeHeurListFactory* owner_ = nullptr;
        std::size_t lastSize_ = 0;
        const NodeType::ValueList* heurList_ = nullptr;
        NodeType::ValueList::const_iterator iterator_;

    public:

        explicit Next(const DecisionTreeHeurListFactory* owner):
            owner_(owner)
        {}

        const NodeType::Value* operator()(const PseudoStatus& pseudoStatus);
    };

public:
    template <class HeurListType>
    DecisionTreeHeurListFactory(
            const Table& table,
            const HeurListType& heurList,
            boost::optional<ComplexChecker> checker,
            std::size_t maxDepth,
            std::size_t numThreads)
    {
        util::TimeMeter timeMeter;
        NodeType::ValueList convertedHeurList;
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

};

using DecisionTreeHeurCalculator = SubStatusHeurCalculator<DecisionTreeHeurListFactory>;


#endif /* DECISIONTREEHEURCALCULATOR_H_ */