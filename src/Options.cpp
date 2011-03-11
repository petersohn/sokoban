#include "config.h"
#include "Options.h"

#include <iostream>
#ifdef USE_PROGRAM_OPTIONS
# include <boost/program_options.hpp>
#endif
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <vector>

class addEnableOption {
	boost::program_options::options_description_easy_init &init_;
public:
	addEnableOption(boost::program_options::options_description_easy_init &init):
			init_(init)
	{}

	addEnableOption& operator()(const std::string& name, const char *description) {
		init_
			(("enable-"+name).c_str(), "")
			(("disable-"+name).c_str(), description);
		return *this;
	}
};

static void optionEnabled(const boost::program_options::variables_map &map,
		const std::string &name, bool &result)
{
	bool enable = map.count("enable-"+name) > 0;
	bool disable = map.count("disable-"+name) > 0;
	if (enable && disable)
		throw BadOptions((boost::format("The option \"%s\" cannot be enabled "
				"and disabled at the same time.")).str());
	else if (enable)
		result = true;
	else if (disbale)
		result = false;
	// else don't change the value
}

static std::string getValueFromList(const std::string &value,
		std::vector<std::string> list)
{
	for (std::vector<std::string>::iterator it = list.begin();
			it != list.end(); ++it)
		if (it->substr(0, value.size()) != value)
			it = list.erase(it);
	if (list.size() > 1)
		throw BadOptions("Ambiguous argument value: " + value);
	if (list.size() == 0)
		throw BadOptions("Unknown argument value: " + value);
	return list[0];
}

//for using on null-terminated string constant arrays
static std::string getValueFromList(const std::string &value,
		char *list[]) {
	std::vector<std::string> vec;
	while (*list != NULL) {
		vec.push_back(*list);
		++list;
	}
}

Options::Options(int argc, char **argv, const char *configFileName):
		enableDump_(false),
		enableXDump_(false),
		oldStyleOutput_(false),
		useStonePusher_(true),
		useMovableChecker_(true),
		useCorridorChecker_(true)
{
#ifdef USE_PROGRAM_OPTIONS
	using namespace boost::program_options;
	options_description desc;
	desc.add_options()
		("help", "produce help message")
		("old-style-output", "Produce output messages of (x1, y1) --> (x2, y2)"
				" instead of (x1, y1) --> direction")
		("filename,f", value<std::string>(), "input file name")
		("dump-stype,d", value<std::string>(), "Generate dump file of the process. Slows down calculation."
				"Values can be:\n"
				"\t(n)one    Disable dumping.\n"
				"\t(t)ext    Text dump format.\n"
				"\t(x)ml     XML dump format. Considerably increases memory consumption.")

		;
	addEnableOption(desc)
		("stone-pusher", "Enable/disable automatic pushing of stones into the destination point.")
		("movable-checker", "Enable/disable checking if a stone is not stuck.")
		("corridor-checker", "Enable/disable checking for corridors.")
		;

	positional_options_description p;
	p.add("filename", 1);
	variables_map vm;
	try {
		store(command_line_parser(argc, argv).
				options(desc).positional(p).run(), vm);
		notify(vm);
		if (vm.count("help") > 0) {
			std::cerr << desc << std::endl;
			exit(0);
		}
		if (vm.count("filename") == 0) {
			std::cerr << "No filename given." << std::endl <<
					desc << std::endl;
			exit(1);
		}
		std::string dumpStyle =
		enableDump_ = vm.count("enable-dump") > 0;
		enableXDump_ = vm.count("enable-xml-dump") > 0;
		oldStyleOutput_ = vm.count("old-style-output") > 0;
		filename_ = vm["filename"].as<std::string>();
	} catch (std::exception &e) {
		std::cerr << "An error has happened while parsing the command line: " <<
				e.what() << std::endl << desc << std::endl;
		exit(1);
	}
#else
	if (argc > 1)
		filename_ = argv[1];
	else {
		std::cerr << "No filename given." << std::endl;
		exit(1);
	}
#endif
}

