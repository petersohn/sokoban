#include "config.h"
#include "Options.h"
#include "OptionsHelper.h"
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <vector>


Options::Options(int argc, char **argv, const char *configFileName):
		dumpStyle_(dsNone),
		oldStyleOutput_(false),
		useStonePusher_(true),
		useMovableChecker_(true),
		useCorridorChecker_(true)
{
	OptionsHelper oh;
	bool help = false;
	oh.addCommandLineFlag("help,h", &help, "Produce help message, then quit.");
	oh.addCommandLineFlag("old-style-output", &oldStyleOutput_,
			"Produce output messages of (x1, y1) --> (x2, y2)"
			" instead of (x1, y1) --> direction");
	oh.addPositionalParameter("filename", &filename_, "Input file name");
	IndexedArgument ds;
	ds.addElement("none", dsNone);
	ds.addElement("text", dsText);
	ds.addElement("xml", dsXML);
	int dumpStyle = 0;
	oh.addIndexedOption("dump-style,d", &dumpStyle, ds,
			"Generate dump file of the process. Dump generation slows down calculation."
			"Values can be:\n"
			"    (n)one   \tDisable dumping.\n"
			"    (t)ext   \tText dump format. Slows down dump generation moderately\n"
			"    (x)ml    \tXML dump format. Highly increases memory consumption"
			" and calculation time.");
	oh.addBoolOption("stone-pusher", &useStonePusher_,
			"Enable/disable automatic pushing of stones into the destination point.\n");
	oh.addBoolOption("movable-checker", &useMovableChecker_,
			"Enable/disable checking if a stone is not stuck.\n");
	oh.addBoolOption("corridor-checker", &useCorridorChecker_,
			"Enable/disable checking for corridors.\n");

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
		dumpStyle_ = static_cast<DumpStyle>(dumpStyle);
		if (filename_.size() == 0) {
			std::cerr << "No filename given." << std::endl;
			oh.print(argv[0]);
			exit(1);
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

