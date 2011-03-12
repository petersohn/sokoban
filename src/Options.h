#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>
#include <exception>
#include <vector>
#include <boost/unordered_map.hpp>

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
public:
	enum DumpStyle {dsNone, dsText, dsXML};
private:
	DumpStyle dumpStyle_;
	bool oldStyleOutput_;
	bool useStonePusher_;
	bool useMovableChecker_;
	bool useCorridorChecker_;
	std::string filename_;

	typedef boost::unordered_map<std::string, int> ArgumentList;

	static const char *dumpStyles_[];
	static ArgumentList dumpStyles;

	static ArgumentList createArgumentList(const char *list[]);
	static int getValueFromList(const std::string &value, ArgumentList list);
public:
	Options(int argc, char **argv, const char *configFileName = "");

	DumpStyle dumpStyle() const { return dumpStyle_; }
	bool oldStyleOutput() const { return oldStyleOutput_; }
	bool useStonePusher() const { return useStonePusher_; }
	bool useMovableChecker() const { return useMovableChecker_; }
	bool useCorridorChecker() const { return useCorridorChecker_; }
	const std::string &filename() const { return filename_; }
};

#endif /* OPTIONS_H_ */
