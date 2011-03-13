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
	oh.addCommandLineFlag("help,h", &help, "produce help message");
	oh.addCommandLineFlag("old-style-output", &oldStyleOutput_,
			"Produce output messages of (x1, y1) --> (x2, y2)"
			" instead of (x1, y1) --> direction");
	oh.addPositionalParameter("filename", &filename_, "input file name");
	IndexedArgument ds;
	ds.addElement("none", dsNone);
	ds.addElement("text", dsText);
	ds.addElement("xml", dsXML);
	int dumpStyle = 0;
	oh.addIndexedOption("dump-style,d", &dumpStyle, ds,
			"Generate dump file of the process. Slows down calculation."
			"Values can be:\n"
			"      (n)one    \tDisable dumping.\n"
			"      (t)ext    \tText dump format.\n"
			"      (x)ml     \tXML dump format. Considerably increases memory consumption.");
	oh.addBoolOption("stone-pusher", &useStonePusher_,
			"Enable/disable automatic pushing of stones into the destination point.\n");
	oh.addBoolOption("movable-checker", &useMovableChecker_,
			"Enable/disable checking if a stone is not stuck.\n");
	oh.addBoolOption("corridor-checker", &useCorridorChecker_,
			"Enable/disable checking for corridors.\n");

	try {
		if (configFileName != NULL)
			oh.parseConfigFile(configFileName);
		oh.parseCommandLine(argc, argv);
		if (help) {
			oh.print();
			exit(0);
		}
		dumpStyle_ = static_cast<DumpStyle>(dumpStyle);
	} catch (BadOptions &e) {
		std::cerr << "Invalid command line argument. " <<
				e.what() << std::endl;
		oh.print();
		exit(1);
	} catch (std::exception &e) {
		std::cerr << "An error has happened while parsing the command line: " <<
				e.what() << std::endl;
		oh.print();
		exit(1);
	}
}

