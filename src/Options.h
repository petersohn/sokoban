#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>

class Options {
	bool enableDump_;
	bool oldStyleOutput_;
	std::string filename_;
public:
	Options(int argc, char **argv);

	bool enableDump() const { return enableDump_; }
	bool oldStyleOutput() const { return oldStyleOutput_; }
	const std::string &filename() const { return filename_; }
};

#endif /* OPTIONS_H_ */
