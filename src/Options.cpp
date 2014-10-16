#include "Options.hpp"
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <vector>
#include <cstdlib>

namespace po = boost::program_options;

namespace {

template <typename T>
po::typed_value<T>* defaultValue(T* v)
{
	return po::value(v)->default_value(*v);
}

void parseConfigFile(po::variables_map& vm,
		const po::options_description& optionsDescription,
		const char* configFile)
{
	po::store(po::parse_config_file<char>(configFile, optionsDescription), vm);
	po::notify(vm);
}

void printHelp(const char* name, const po::options_description& optionsDescription)
{
	std::cerr << "Usage: " << name << " [options] filename\n" <<
		optionsDescription << std::endl;
}

}

Options parseOptions(int argc, char **argv, const char *configFileName)
{
	bool help = false;
	Options options;
	po::options_description commandLineDesctiption("Command-line only options");

	commandLineDesctiption.add_options()
			("help,h", po::bool_switch(&help),
			 "Produce help message, then quit.")
			("test", defaultValue(&options.test_),
			"Instead of solving a problem, solve each possible problem with the given number "
			"of stones on the table.")
			("filename", po::value(&options.filename_),
			 "Input file name")
		;

	po::options_description generalDescription("General options");
	generalDescription.add_options()
			("old-style-output", defaultValue(&options.oldStyleOutput_),
			"Produce output messages of (x1, y1) --> (x2, y2)"
			" instead of (x1, y1) --> direction")
			("blocklist-number", defaultValue(&options.blockListStones_),
			"The number of stones the blocklist contains. "
			"The calculation time of the blocklist is exponential in this number.")
			("blocklist-distance", defaultValue(&options.blockListDistance_),
			"The maximum distance between stones in the "
			"blocklist. 0 means no limit.")
			("work-queue-length", defaultValue(&options.workQueueLength_),
			 "The granularity of scheduling during blocklist generation.")
			("thread-num,t", defaultValue(&options.numThreads_),
			"The maximum number of threads to use.")
			("parallel-outer-expand", defaultValue(&options.parallelOuterExpand_),
			"Expand several nodes in parallel. Only if --thread-num > 1.")
			("dump-style,d", defaultValue(&options.dumpStyle_),
			"Generate dump file of the process. Dump generation slows down calculation.\n"
			"Values can be:\n"
			"    (n)one   \tDisable dumping.\n"
			"    (t)ext   \tText dump format. Slows down dump generation moderately.\n"
			"    (x)ml    \tXML dump format. Highly increases memory consumption"
			" and calculation time.")
			("stone-pusher", defaultValue(&options.useStonePusher_),
			"Enable/disable automatic pushing of stones into the destination point.\n")
			("movable-checker", defaultValue(&options.useMovableChecker_),
			"Enable/disable checking if a stone is not stuck.\n")
			("corridor-checker", defaultValue(&options.useCorridorChecker_),
			"Enable/disable checking for corridors.\n")
			("advanced-heur-calculator", defaultValue(&options.useAdvancedHeurCalculator_),
			"Enable/disable advanced heur calculator.\n")
			("heur-list-size", defaultValue(&options.maxHeurListSize_),
			"The maximum size for the heur list when blocklist-heur-calculator is enabled. 0 means no limitation.")
			("blocklist-heur-calculator", defaultValue(&options.blocklistHeurCalculatorType_),
			"The type of the blocklist heur calculator. Only meaningful if --blocklist-number > 1. "
			"Possible values:\n"
			"    (n)one          \tDisable blocklist heur calculator.\n"
			"    (v)ector        \tUse vector and linear search. Slower than decision tree.\n"
			"    (d)ecision-tree \tUse decision tree. Faster but requires preprocessing.\n")
			("max-decision-tree-depth", defaultValue(&options.maxDecisionTreeDepth_),
			"The maximum depth of the decision tree.")
			("blocklist-decision-tree-depth", defaultValue(&options.blocklistDecisionTreeDepth_),
			"The depth of the decision tree used for preprocessing. "
			"0 means do not use decision tree for preprocessing. "
			"It is only effective if --blocklist-heur-calculator=decision-tree is set.")
			("decision-tree-checker", defaultValue(&options.useCheckerForDecisionTree_),
			"Enable/disable usage of checker in DecisionTree building.\n")
			("compare,c", po::value(&options.compare_)->multitoken(),
			"The compare algorithm to use when choosing equal elements.")
			("status-pool", defaultValue(&options.statusPoolSize_),
			"The size of Status pool to avoid many calculateReachable() calls. 0 means no pooling.")
		;

	po::positional_options_description positionalParameters;
	positionalParameters.add("filename", 1);

	po::options_description optionsDescription;
	optionsDescription.add(commandLineDesctiption);
	optionsDescription.add(generalDescription);

	try {
		po::variables_map vm;
		po::store(po::command_line_parser(argc, argv).
				options(optionsDescription).
				positional(positionalParameters).
				run(), vm);
		po::notify(vm);

		if (help) {
			printHelp(argv[0], optionsDescription);
			exit(0);
		}
		if (options.filename_.empty()) {
			std::cerr << "No filename given." << std::endl;
			printHelp(argv[0], optionsDescription);
			exit(1);
		}

		parseConfigFile(vm, generalDescription, options.filename_.c_str());
		try {
			if (configFileName) {
				parseConfigFile(vm, generalDescription, configFileName);
			}
		} catch (boost::program_options::reading_file &e) {
			// DON'T CARE
		}
	} catch (std::exception &e) {
		std::cerr << "An error has happened while parsing the command line: " <<
				e.what() << std::endl;
		printHelp(argv[0], optionsDescription);
		exit(1);
	}

	return options;
}

std::istream& operator>>(std::istream& is, Compare& value)
{
	std::string s;
	is >> s;
	value = compares().at(s);
	return is;
}

std::ostream& operator<<(std::ostream& os, const Compare& value)
{
	auto it = std::find_if(compares().begin(), compares().end(),
			[&](const util::PrefixMap<Compare>::value_type& compare)
			{ return compare.second == value; });
	assert(it != compares().end());
	return os << it->first;
}
