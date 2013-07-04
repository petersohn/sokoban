#include "config.h"
#include "Options.h"
#include "OptionsHelper.h"
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <vector>
#include <cstdlib>


Options::Options(int argc, char **argv, const char *configFileName):
		dumpStyle_(dsNone),
		oldStyleOutput_(false),
		useStonePusher_(true),
		useMovableChecker_(true),
		useCorridorChecker_(true),
		useAdvancedHeurCalculator_(true),
		statusPoolSize_(0),
		blocklistHeurCalculatorType_(bhNone),
		parallelOuterExpand_(false),
		blockListStones_(0),
		blockListDistance_(0),
		maxHeurListSize_(0),
		numThreads_(1),
		test_(0),
		maxDecisionTreeDepth_(10),
		useCheckerForDecisionTree_(false)
{
	OptionsHelper oh;
	bool help = false;
	oh.addCommandLineFlag("help,h", &help, "Produce help message, then quit.");
	oh.addCommandLineFlag("old-style-output", &oldStyleOutput_,
			"Produce output messages of (x1, y1) --> (x2, y2)"
			" instead of (x1, y1) --> direction");
	oh.addPositionalParameter("filename", &filename_, "Input file name");
	oh.addArgumentOption<int>("blocklist-number", &blockListStones_,
			"The number of stones the blocklist contains. "
			"The calculation time of the blocklist is exponential in this number.");
	oh.addArgumentOption<int>("blocklist-distance", &blockListDistance_,
			"The maximum distance between stones in the "
			"blocklist. 0 means no limit.");
	oh.addArgumentOption<int>("thread-num,t", &numThreads_,
			"The maximum number of threads to use.");
	oh.addBoolOption("parallel-outer-expand", &parallelOuterExpand_,
			"Expand several nodes in parallel. Only if --thread-num > 1.");
	oh.addArgumentOption<int>("test", &test_,
			"Instead of solving a problem, solve each possible problem with the given number "
			"of stones on the table.");

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
	oh.addBoolOption("advanced-heur-calculator", &useAdvancedHeurCalculator_,
			"Enable/disable advanced heur calculator.\n");
	oh.addArgumentOption<int>("heur-list-size", &maxHeurListSize_,
			"The maximum size for the heur list when blocklist-heur-calculator is enabled. 0 means no limitation.");
	IndexedArgument bh;
	bh.addElement("none", bhNone);
	bh.addElement("vector", bhVector);
	bh.addElement("decision-tree", bhDecisionTree);
	int blocklistHeurCalculatorType;
	oh.addIndexedOption("blocklist-heur-calculator", &blocklistHeurCalculatorType, bh,
			"The type of the blocklist heur calculator. Only meaningful if --blocklist-number > 1. "
			"Possible values:\n"
			"    (n)one          \tDisable blocklist heur calculator.\n"
			"    (v)ector        \tUse vector and linear search. More optimal, but slower than decision tree.\n"
			"    (d)ecision-tree \tUse decision tree. Faster but less optimal.\n");
	oh.addArgumentOption<int>("max-decision-tree-depth", &maxDecisionTreeDepth_,
			"The maximum depth of the decision tree.");
	oh.addBoolOption("decision-tree-checker", &useCheckerForDecisionTree_,
			"Enable/disable usage of checker in DecisionTree building.\n");
	IndexedArgument cl;
	cl.addElement("time", ctTime);
	cl.addElement("heur", ctHeur);
	cl.addElement("depth", ctDepth);
	cl.allowMinus(true);
	std::vector<int> compare;
	oh.addIndexedListOption("compare,c", &compare, cl,
			"The compare algorithm to use when choosing equal elements.");
	oh.addArgumentOption<int>("status-pool", &statusPoolSize_,
			"The size of Status pool to avoid many calculateReachable() calls. 0 means no pooling.");


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
		if (filename_.empty()) {
			std::cerr << "No filename given." << std::endl;
			oh.print(argv[0]);
			exit(1);
		}
		oh.parseConfigFile(filename_.c_str());
		oh.parseCommandLine(argc, argv);
		dumpStyle_ = static_cast<DumpStyle>(dumpStyle);
		blocklistHeurCalculatorType_ = static_cast<BlockListHeurType>(blocklistHeurCalculatorType);
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

