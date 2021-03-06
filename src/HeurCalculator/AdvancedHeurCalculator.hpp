#ifndef ADVANCEDHEURCALCULATOR_H_
#define ADVANCEDHEURCALCULATOR_H_

#include "Dumper/DumperFunctions.hpp"
#include "HeurCalculator/TableHeurCalculator.hpp"

#include <boost/optional.hpp>
#include <boost/serialization/vector.hpp>

#include <iostream>
#include <fstream>
#include <vector>

namespace sokoban {

class Solver;

class AdvancedStoneCalculator {
    struct Partition
    {
        Point pos;
        Matrix<bool> reachable;
        float heur;

        Partition() = default;
        Partition(size_t width, size_t height):
                reachable(width, height, false),
                heur(0)
        {}

        Partition(const Partition&) = default;
        Partition& operator=(const Partition&) = default;
        Partition(Partition&&) = default;
        Partition& operator=(Partition&&) = default;

        template <typename Archive>
        void serialize(Archive& ar, const unsigned int /*version*/)
        {
            ar & pos;
            ar & reachable;
            ar & heur;
        }
    };
    class HeurDumper {
        std::string filename_;
        std::ofstream file_;
        void open();
    public:
        HeurDumper(std::string filename): filename_(std::move(filename)) {}
        void dumpPartition(const Table& table, const Partition& partition);
        template<class T>
        void dumpMatrix(const Matrix<T>& arr)
        {
            ::sokoban::dumpMatrix<std::string>(file_, arr);
        }
        void printText(const char* text);
    };

    Matrix<std::vector<Partition>> partitions_;

    void init(const Table& table, const Solver& solver,
            std::size_t reverseSearchMaxDepth,
            const boost::optional<std::string>& filename);
    void initPartitions(const Table& table, Point p, const Solver& solver,
            std::size_t reverseSearchMaxDepth);
public:
    AdvancedStoneCalculator(const Table& table, const Solver& solver,
            std::size_t reverseSearchMaxDepth,
            const boost::optional<std::string>& filename)
    {
        init(table, solver, reverseSearchMaxDepth, filename);
    }

    AdvancedStoneCalculator() = default;

    AdvancedStoneCalculator(const AdvancedStoneCalculator&) = default;
    AdvancedStoneCalculator& operator=(const AdvancedStoneCalculator&) = default;
    AdvancedStoneCalculator(AdvancedStoneCalculator&&) = default;
    AdvancedStoneCalculator& operator=(AdvancedStoneCalculator&&) = default;

    float operator()(const Status& status, Point p) const;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int /*version*/)
    {
        ar & partitions_;
    }
};

using AdvancedHeurCalculator = TableHeurCalculator<AdvancedStoneCalculator>;

} // namespace sokoban

#endif /* ADVANCEDHEURCALCULATOR_H_ */
