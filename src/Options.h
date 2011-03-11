#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>
#include <exception>

class BadOptions: public std::exception {
	std::string message_;
public:
	BadOptions(const std::string &msg) throw():message_(msg) {}
	~BadOptions() throw() {}
	virtual const char* what() const throw() {
		return message_.c_str();
	}
};

class Options {
	bool enableDump_;
	bool enableXDump_;
	bool oldStyleOutput_;
	bool useStonePusher_;
	bool useMovableChecker_;
	bool useCorridorChecker_;
	std::string filename_;
public:
	Options(int argc, char **argv, const char *configFileName);

	bool enableDump() const { return enableDump_; }
	bool enableXDump() const { return enableXDump_; }
	bool oldStyleOutput() const { return oldStyleOutput_; }
	bool useStonePusher() const { return useStonePusher_; }
	bool useMovableChecker() const { return useMovableChecker_; }
	bool useCorridorChecker() const { return useCorridorChecker_; }
	const std::string &filename() const { return filename_; }
};

#endif /* OPTIONS_H_ */
