#include "ProgressBar.hpp"
#include <iostream>

void ProgressBar::draw(int solved)
{
	if (solved <= iters_) {
		float div = iters_ / 100.0f;
		int n1 = static_cast<int>(solved / div);
		if (n1 != lastTicks_) {
			int n2 = 100 - n1;
			std::cerr << "[";
			for (int i = 0; i < n1; ++i)
				std::cerr << "#";
			for (int i = 0; i < n2; ++i)
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

