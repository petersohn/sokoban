#include "OptionsHelper.h"
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>


void IndexedArgument::addElement(const std::string name, int index)
{
	map_.push_back(EntryType(name, index));
}

int IndexedArgument::getElement(const std::string value) const
{
	MapType map(map_);
	MapType::iterator it = map.begin();
	while (it != map.end())
		if (it->first.substr(0, value.size()) != value)
			it = map.erase(it);
		else
			++it;
	if (map.size() > 1)
		throw BadOptions("Ambiguous argument value: " + value);
	if (map.size() == 0)
		throw BadOptions("Unknown argument value: " + value);
	return map.begin()->second;
}



std::string OptionsHelper::stripComma(const std::string &s)
{
	return s.substr(0, s.find(','));
}

void OptionsHelper::addCommandLineFlag(const std::string &name, bool *target, const char *description)
{
	commandLineDescription_.add_options()(name.c_str(), description);
	commandLineFlags_.push_back(FlagType(stripComma(name), target));
}

void OptionsHelper::addBoolOption(const std::string &name, bool *target,
		const char *description)
{
	using namespace boost::program_options;
	std::string s = "enable-" + name;
	commandLineDescription_.add_options()(s.c_str(), "");
	s = "disable-" + name;
	commandLineDescription_.add_options()(s.c_str(), description);
	configFileDescription_.add_options()(name.c_str(), value<std::string>(), description);
	boolOptions_.push_back(FlagType(stripComma(name), target));
}

void OptionsHelper::addIndexedOption(const std::string &name, int *target,
		const IndexedArgument &arg, const char *description)
{
	using namespace boost::program_options;
	commandLineDescription_.add_options()(name.c_str(), value<std::string>(), description);
	configFileDescription_.add_options()(name.c_str(), value<std::string>(), description);
	indexedOptions_.push_back(IndexedType(stripComma(name), arg, target));
}

void OptionsHelper::parseCommandLine(int argc, char **argv)
{
	using namespace boost::program_options;
	variables_map vm;
	options_description desc(commandLineDescription_);
	store(command_line_parser(argc, argv).
				options(desc.add(positionalParametersDescription_)).
				positional(positionalParameters_).
				run(), vm);
	notify(vm);
	for (std::vector<FlagType>::iterator it = commandLineFlags_.begin();
			it != commandLineFlags_.end(); ++it)
		*(it->second) = vm.count(it->first) != 0;
	for (std::vector<FlagType>::iterator it = boolOptions_.begin();
			it != boolOptions_.end(); ++it) {
		bool enable = vm.count("enable-"+it->first) > 0;
		bool disable = vm.count("disable-"+it->first) > 0;
		if (enable && disable)
			throw BadOptions((boost::format("The option \"%s\" cannot be enabled "
					"and disabled at the same time.") % it->first).str());
		else if (enable)
			*(it->second) = true;
		else if (disable)
			*(it->second) = false;
	}
	doParse(vm);
}

void OptionsHelper::parseConfigFile(const char *configFile)
{
	using namespace boost::program_options;
	variables_map vm;
	store(parse_config_file<char>(configFile, configFileDescription_, false), vm);
	notify(vm);
	for (std::vector<FlagType>::iterator it = boolOptions_.begin();
			it != boolOptions_.end(); ++it) {
		if (vm.count(it->first) == 0)
			continue;
		std::string s = vm[it->first].as<std::string>();
		boost::to_upper(s);
		if (s == "YES")
			*(it->second) = true;
		else if (s == "NO")
			*(it->second) = false;
		else
			throw BadOptions((boost::format("Invalid value: \"%s\". Value"
					" must be \"YES\" or \"NO\"") % it->first).str());
	}
	doParse(vm);
}

void OptionsHelper::doParse(const boost::program_options::variables_map &vm)
{
	for (std::vector<IndexedType>::iterator it = indexedOptions_.begin();
			it != indexedOptions_.end(); ++it) {
		if (vm.count(it->get<0>()) == 0)
			continue;
		*(it->get<2>()) = it->get<1>().getElement(vm[it->get<0>()].as<std::string>());
	}
}

void OptionsHelper::print() const
{
	std::cerr << commandLineDescription_ << std::endl;
}
