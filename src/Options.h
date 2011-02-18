#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>

class Options {
	bool enableDump_;
	std::string filename_;
public:
	Options(int argc, char **argv);

	bool enableDump() { return enableDump_; }
	const std::string &filename() { return filename_; }
};

#endif /* OPTIONS_H_ */
