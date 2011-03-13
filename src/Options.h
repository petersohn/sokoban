#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <string>
#include <exception>
#include <vector>
#include <boost/unordered_map.hpp>
#include <iostream>

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
public:
	Options(int argc, char **argv, const char *configFileName = NULL);

	DumpStyle dumpStyle() const { return dumpStyle_; }
	bool oldStyleOutput() const { return oldStyleOutput_; }
	bool useStonePusher() const { return useStonePusher_; }
	bool useMovableChecker() const { return useMovableChecker_; }
	bool useCorridorChecker() const { return useCorridorChecker_; }
	const std::string &filename() const { return filename_; }
};

#endif /* OPTIONS_H_ */
