#include "ProgressBar.hpp"

#include <util/TerminalSize.hpp>

#include <iostream>
#include <unistd.h>

void ProgressBar::draw(std::size_t solved)
{
    util::TerminalSize terminalSize = util::getTerminalSize(2);
    if (terminalSize.width < 2) {
        return;
    }

    int progressBarSize = terminalSize.width - STDERR_FILENO;

    if (solved <= iters_) {
        float div = static_cast<float>(iters_) / progressBarSize;
        std::size_t n1 = static_cast<std::size_t>(solved / div);
        if (n1 != lastTicks_) {
            std::size_t n2 = progressBarSize - n1;
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

