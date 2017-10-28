#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

#include <cstdint>

namespace sokoban {

class ProgressBar {
    std::size_t lastTicks_;
    std::size_t iters_;

public:
    ProgressBar(std::size_t iters):
        lastTicks_(0),
        iters_(iters)
    {}
    ~ProgressBar();

    ProgressBar(const ProgressBar&) = delete;
    ProgressBar& operator=(const ProgressBar&) = delete;

    void draw(std::size_t solved);
};

} // namespace sokoban

#endif /* PROGRESSBAR_H_ */
