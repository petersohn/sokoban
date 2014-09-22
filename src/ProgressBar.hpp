#ifndef PROGRESSBAR_H_
#define PROGRESSBAR_H_

class ProgressBar {
	int lastTicks_;
	int iters_;
public:
	ProgressBar(int iters):
		lastTicks_(0),
		iters_(iters)
	{}
	~ProgressBar();
	void draw(int solved);
};

#endif /* PROGRESSBAR_H_ */
