#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>

class Options {
	std::string fn;
public:
	Options(int argc, char **argv):fn(argv[1]) {}

	const std::string &filename() { return fn; }
	bool enableDump() { return false; }
};

#endif /* OPTIONS_H_ */
