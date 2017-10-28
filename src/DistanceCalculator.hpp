#ifndef DISTANCECALCULATOR_HPP
#define DISTANCECALCULATOR_HPP

#include "Matrix.hpp"
#include "Status/Point.hpp"
#include "Status/PointRange.hpp"

namespace sokoban {

namespace detail {

template<typename Functor>
class DistanceCalculator {
public:
    DistanceCalculator(const Functor& functor, Matrix<int>& distances) :
            functor(functor),
            distances(distances) {
    }

    void calculate(std::size_t width, std::size_t height, Point base) {
        distances.reset(width, height, 0);
        distances[base] = 1.0f;
        bool touched;
        do {
            touched = false;
            for (Point  p: PointRange{Point{0, 0}, Point(width, height)}) {
                if (matrixAt(distances, p, 0) == 0)
                    continue;
                float newDist = distances[p]+1.0f;
                if (checkDistance(p, p10, newDist))
                    touched = true;
                if (checkDistance(p, -p10, newDist))
                    touched = true;
                if (checkDistance(p, p01, newDist))
                    touched = true;
                if (checkDistance(p, -p01, newDist))
                    touched = true;
            }
        } while (touched);
        for (auto& distance: distances) {
            distance -= 1.0f;
        }
    }

private:
    bool checkDistance(Point  p, Point  d, int dist) {
        Point pd = p+d;
        int value = matrixAt(distances, pd, -1);
        bool result = functor(p, d) && (value == 0 || value > dist);
        if (result) {
            distances[pd] = dist;
        }
        return result;
    }

    const Functor& functor;
    Matrix<int>& distances;
};

} // namespace detail

template<typename Functor>
Matrix<int> calculateDistances(std::size_t width, std::size_t height,
        Point base, const Functor& functor) {
    Matrix<int> result;
    detail::DistanceCalculator<Functor>{functor, result}.calculate(
            width, height, base);
    return result;
}
} // namespace sokoban

#endif // DISTANCECALCULATOR_HPP
