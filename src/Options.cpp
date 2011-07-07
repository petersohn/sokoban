#include "config.h"
#include "Options.h"
#include "OptionsHelper.h"
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <vector>
#include <cstdlib>


Options::Options(int argc, char **argv, const char *configFileName):
		dumpStyle_(dsNone),
		oldStyleOutput_(false),
		useStonePusher_(true),
		useMovableChecker_(true),
		useCorridorChecker_(true),
		blockListStones_(0),
		blockListDistance_(0)
{
	OptionsHelper oh;
	bool help = false;
	oh.addCommandLineFlag("help,h", &help, "Produce help message, then quit.");
	oh.addCommandLineFlag("old-style-output", &oldStyleOutput_,
			"Produce output messages of (x1, y1) --> (x2, y2)"
			" instead of (x1, y1) --> direction");
	oh.addPositionalParameter("filename", &filename_, "Input file name");
	oh.addArgumentOption<int>("blocklist-number", &blockListStones_, "The number of stones the blocklist contains. "
			"The calculation time of the blocklist is exponential in this number.");
	oh.addArgumentOption<int>("blocklist-distance", &blockListDistance_, "The maximum distance between stones in the "
			"blocklist. 0 means no limit.");
	IndexedArgument ds;
	ds.addElement("none", dsNone);
	ds.addElement("text", dsText);
	ds.addElement("xml", dsXML);
	int dumpStyle = 0;
	oh.addIndexedOption("dump-style,d", &dumpStyle, ds,
			"Generate dump file of the process. Dump generation slows down calculation.\n"
			"Values can be:\n"
			"    (n)one   \tDisable dumping.\n"
			"    (t)ext   \tText dump format. Slows down dump generation moderately.\n"
			"    (x)ml    \tXML dump format. Highly increases memory consumption"
			" and calculation time.");
	oh.addBoolOption("stone-pusher", &useStonePusher_,
			"Enable/disable automatic pushing of stones into the destination point.\n");
	oh.addBoolOption("movable-checker", &useMovableChecker_,
			"Enable/disable checking if a stone is not stuck.\n");
	oh.addBoolOption("corridor-checker", &useCorridorChecker_,
			"Enable/disable checking for corridors.\n");
	IndexedArgument cl;
	cl.addElement("time", ctTime);
	cl.addElement("heur", ctHeur);
	cl.addElement("depth", ctDepth);
	cl.allowMinus(true);
	std::vector<int> compare;
	oh.addIndexedListOption("compare,c", &compare, cl,
			"The compare algorithm to use when choosing equal elements.");
	try {
		try {
			if (configFileName != NULL)
				oh.parseConfigFile(configFileName);
		} catch (boost::program_options::reading_file &e) {
			// DON'T CARE
		}
		oh.parseCommandLine(argc, argv);
		if (help) {
			oh.print(argv[0]);
			exit(0);
		}
		if (filename_.size() == 0) {
			std::cerr << "No filename given." << std::endl;
			oh.print(argv[0]);
			exit(1);
		}
		oh.parseConfigFile(filename_.c_str());
		oh.parseCommandLine(argc, argv);
		dumpStyle_ = static_cast<DumpStyle>(dumpStyle);
		for (std::vector<int>::iterator it = compare.begin();
				it != compare.end(); ++it) {
			compare_.push_back(Compare(
					static_cast<CompareMethod>(std::abs(*it)), *it < 0));
		}
	} catch (BadOptions &e) {
		std::cerr << "Invalid command line argument. " <<
				e.what() << std::endl;
		oh.print(argv[0]);
		exit(1);
	} catch (std::exception &e) {
		std::cerr << "An error has happened while parsing the command line: " <<
				e.what() << std::endl;
		oh.print(argv[0]);
		exit(1);
	}
}

