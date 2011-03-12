#include "config.h"
#include "Options.h"

#include <iostream>
#ifdef USE_PROGRAM_OPTIONS
# include <boost/program_options.hpp>
#endif
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <vector>

class AddEnableOption {
	boost::program_options::options_description_easy_init init_;
public:
	AddEnableOption(boost::program_options::options_description_easy_init init):
			init_(init)
	{}

	AddEnableOption& operator()(const std::string& name, const char *description) {
		init_
			(("enable-"+name).c_str(), "")
			(("disable-"+name).c_str(), description);
		return *this;
	}
};

static AddEnableOption addEnableOption(boost::program_options::options_description &init) {
	return AddEnableOption(init.add_options());
}

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
	else if (disable)
		result = false;
	// else don't change the value
}



/* Options */

const char *Options::dumpStyles_[] = {"none", "text", "xml", NULL};
Options::ArgumentList Options::dumpStyles(
		Options::createArgumentList(dumpStyles_));

Options::ArgumentList Options::createArgumentList(const char *list[]) {
	ArgumentList vec;
	int pos = 0;
	while (*list != NULL) {
		vec.insert(std::make_pair(*list, pos++));
		++list;
	}
	return vec;
}

int Options::getValueFromList(const std::string &value,
		ArgumentList list)
{
	ArgumentList::iterator it = list.begin();
	while (it != list.end())
		if (it->first.substr(0, value.size()) != value)
			it = list.erase(it);
		else
			++it;
	if (list.size() > 1)
		throw BadOptions("Ambiguous argument value: " + value);
	if (list.size() == 0)
		throw BadOptions("Unknown argument value: " + value);
	return list.begin()->second;
}


Options::Options(int argc, char **argv, const char *configFileName):
		dumpStyle_(dsNone),
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
		("dump-style,d", value<std::string>(), "Generate dump file of the process. Slows down calculation."
				"Values can be:\n"
				"      (n)one    \tDisable dumping.\n"
				"      (t)ext    \tText dump format.\n"
				"      (x)ml     \tXML dump format. Considerably increases memory consumption.")

		;
	addEnableOption(desc)
		("stone-pusher", "Enable/disable automatic pushing of stones into the destination point.\n")
		("movable-checker", "Enable/disable checking if a stone is not stuck.\n")
		("corridor-checker", "Enable/disable checking for corridors.\n")
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
		if (vm.count("dump-style") > 0)
			dumpStyle_ = static_cast<DumpStyle>(getValueFromList(vm["dump-style"].as<std::string>(),
								dumpStyles));
		oldStyleOutput_ = vm.count("old-style-output") > 0;
		filename_ = vm["filename"].as<std::string>();
		optionEnabled(vm, "stone-pusher", useStonePusher_);
		optionEnabled(vm, "movable-checker", useMovableChecker_);
		optionEnabled(vm, "corridor-checker", useCorridorChecker_);
/*	} catch (BadOptions &e) {
		std::cerr << "Invalid command line argument. " <<
				e.what() << std::endl << desc << std::endl;
		exit(1);*/
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

