#include "Options.hpp"

#include <boost/format.hpp>
#include <boost/program_options.hpp>

#include <cstdlib>
#include <iostream>
#include <vector>

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

Options parseOptions(int argc, const char* argv[], const char *configFileName)
{
    bool help = false;
    Options options;
    po::options_description commandLineDesctiption("Command-line only options");

    commandLineDesctiption.add_options()
            ("help,h", po::bool_switch(&help),
             "Produce help message, then quit.\n")
            ("filename", po::value(&options.filename_),
             "The input file name.\n")
            ("load-preprocessed-result", defaultValue(&options.preprocessLoadFilename_),
             "Load preprocessed results from this file. No preprocessing is done. "
             "--filename and all preprocessing arguments are ignored.\n")
            ("save-preprocessed-result", defaultValue(&options.preprocessSaveFilename_),
             "Save preprocessed results to this file. If this file is loaded the same "
             "results and arguments are used.\n")
            ("save-progress", defaultValue(&options.saveProgress_),
             "Save progress into the given file. If the file already exists, "
             "load the results from this file and continue from that points. "
             "All general options except --num-threads, --output-format, "
             "--solution-dump-filename and --work-queue-length are saved to this "
             "file and these values are ignored from command line when loading.\n")
            ("test", defaultValue(&options.test_),
             "Instead of solving a problem, solve each possible problem with "
             "the given number of stones on the table.\n")
        ;

    po::options_description generalDescription("General options");
    generalDescription.add_options()
            ("advanced-heur-calculator", defaultValue(&options.useAdvancedHeurCalculator_),
             "Enable/disable advanced heur calculator.\n")
            ("blocklist-decision-tree-depth", defaultValue(&options.blocklistDecisionTreeDepth_),
             "The depth of the decision tree used for preprocessing. "
             "0 means do not use decision tree for preprocessing. "
             "It is only effective if --blocklist-heur-calculator=decision-tree is set.\n")
            ("blocklist-distance", defaultValue(&options.blockListDistance_),
             "The maximum distance between stones in the "
             "blocklist. 0 means no limit.\n")
            ("blocklist-heur-calculator", defaultValue(&options.blocklistHeurCalculatorType_),
             "The type of the blocklist heur calculator. Only meaningful if "
             "--blocklist-number > 1.\n"
             "Possible values:\n"
             "    (n)one          \tDisable blocklist heur calculator.\n"
             "    (v)ector        \tUse vector and linear search. Slower than decision tree.\n"
             "    (d)ecision-tree \tUse decision tree. Faster but requires preprocessing.\n")
            ("blocklist-number", defaultValue(&options.blockListStones_),
             "The number of stones the blocklist contains. "
             "The calculation time of the blocklist is exponential in this number.\n")
            ("choke-point-distance", defaultValue(&options.chokePointDistance_),
             "The minimum distance between two points when finding choke points.\n")
            ("choke-point-distant-number", defaultValue(&options.chokePointDistantNum_),
             "The maximum number of points that can be farther away from a choke "
             "point than blocklist-distance.\n")
            ("choke-point-number", defaultValue(&options.chokePointNum_),
             "The number of points to put on the table for finding choke points. "
             "0 means do not check for choke poitns.\n")
            ("compare,c", po::value(&options.compare_)->multitoken(),
             "The compare algorithm to use when choosing equal elements.\n")
            ("corridor-checker", defaultValue(&options.corridorCheckerType_),
             "Type of corridor checking.\n")
            ("decision-tree-checker", defaultValue(&options.useCheckerForDecisionTree_),
             "Enable/disable usage of checker in DecisionTree building.\n")
            ("distance-limiter", defaultValue(&options.distanceLimiter_),
             "If a state was once found that had empty places near the deatination, "
             "do not allow more states that has stones in that empty place. "
             "Does not work if --stone-pusher is disabled. May cut valid results.")
            ("dump-filename", po::value(&options.dumpFilename_),
             "The file to save dumps to. The default depends on the dump style.\n")
            ("dump-filter", po::value(&options.dumpFilter_),
             "The filter string when dump-filter-type is set.\n")
            ("dump-filter-type", defaultValue(&options.dumpFilterType_),
             "Only dump nodes where the description matches this filter.\n"
             "Values can be:\n"
             "    (n)one   \tNo filtering.\n"
             "    (t)ext   \tMatch for exact substring.\n"
             "    (r)egex  \tMatch for regex.\n"
             "Only works with text dumper.\n")
            ("dump-style,d", defaultValue(&options.dumpStyle_),
             "Generate dump file of the process. Dump generation slows down calculation.\n"
             "Values can be:\n"
             "    (n)one       \tDisable dumping.\n"
             "    (t)ext       \tText dump format. Slows down dump generation moderately.\n"
             "    (x)ml        \tXML dump format. Highly increases memory consumption"
             " and calculation time.\n"
             "    (s)tatistics \tPrint statistics of what happens to a csv file. Statistics "
             "are printed each second. Has low performance impact.\n"
             "    (b)est       \tPrint each node added to the queue when the node has lower "
             "heuristics than any previous one. Has low performance impact.\n")
            ("expanded-node-limit", defaultValue(&options.expandedNodeLimit_),
             "The maximum number of nodes to expand. After this limit is reached, "
             "the elements in the queue are still expanded, but no more "
             "nodes are added to the queue.\n")
            ("heur-list-size", defaultValue(&options.maxHeurListSize_),
             "The maximum size for the heur list when blocklist-heur-calculator "
             "is enabled. 0 means no limitation.\n")
            ("heur-multiplier", defaultValue(&options.heurMultiplier_),
             "Multiply the heuristic value by this amount. Higher values may cause the "
             "algorithm to finish faster, at the cost of possibly longer solutions.\n")
            ("max-decision-tree-depth", defaultValue(&options.maxDecisionTreeDepth_),
             "The maximum depth of the decision tree.\n")
            ("movable-checker", defaultValue(&options.movableCheckerType_),
             "Check if a stone is not stuck. The extended checker does a more thorough "
             "checking but is slower.\n")
            ("output-format", defaultValue(&options.outputFormat_),
             "The format of the text printed to stdout.\n")
            ("parallel-expand", defaultValue(&options.parallelExpand_),
             "Use parallelization when expanding nodes.\n")
            ("partitions-dump-filename", defaultValue(&options.partitionsDumpFilename_),
             "The file to save the heuristics table generated by AdvancedHeurCalculator.\n")
            ("reverse-search-max-depth", defaultValue(&options.reverseSearchMaxDepth_),
             "The maximum depth of the tree when checking that a state is "
             "possible during preprocessing.\n")
            ("status-pool", defaultValue(&options.statusPoolSize_),
             "The size of Status pool to avoid many calculateReachable() calls. "
             "0 means no pooling.\n")
            ("solution-dump-filename", defaultValue(&options.solutionDumpFilename_),
             "The file to save the dump for the solution.\n")
            ("stone-pusher", defaultValue(&options.useStonePusher_),
             "Enable/disable automatic pushing of stones into the destination point.\n")
            ("thread-num,t", defaultValue(&options.numThreads_),
             "The maximum number of threads to use.\n")
            ("work-queue-length", defaultValue(&options.workQueueLength_),
             "The granularity of scheduling during blocklist generation.\n")
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
        if (options.filename_.empty() && options.preprocessLoadFilename_.empty()) {
            std::cerr << "No filename given." << std::endl;
            exit(1);
        }

        parseConfigFile(vm, generalDescription, options.filename_.c_str());
        try {
            if (configFileName) {
                parseConfigFile(vm, generalDescription, configFileName);
            }
        } catch (boost::program_options::reading_file& e) {
            // DON'T CARE
        }
    } catch (std::exception& e) {
        std::cerr << "An error has happened while parsing the command line: " <<
                e.what() << std::endl;
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
