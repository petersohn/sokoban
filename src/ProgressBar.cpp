#include "ProgressBar.hpp"
#include <iostream>

void ProgressBar::draw(std::size_t solved)
{
    if (solved <= iters_) {
        float div = iters_ / 100.0f;
        std::size_t n1 = static_cast<std::size_t>(solved / div);
        if (n1 != lastTicks_) {
            std::size_t n2 = 100 - n1;
            std::cerr << "[";
            for (std::size_t i = 0; i < n1; ++i)
                std::cerr << "#";
            for (std::size_t i = 0; i < n2; ++i)
                std::cerr << "-";
            std::cerr << "]\r";
            std::cerr.flush();
            lastTicks_ = n1;
        }
    }
}

ProgressBar::~ProgressBar()
{
    std::cerr << "\n";
}

