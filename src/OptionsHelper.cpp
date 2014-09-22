#include "OptionsHelper.h"
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>


void IndexedArgument::addElement(const std::string &name, int index)
{
	map_.push_back(EntryType(name, index));
}

int IndexedArgument::getElement(std::string value) const
{
	int mul = 1;
	if (allowMinus_) {
		char ch = value[0];
		if (ch == '+' || ch == '-') {
			if (ch == '-')
				mul = -1;
			value.erase(value.begin());
		}
	}
	MapType map(map_);
	MapType::iterator it = map.begin();
	while (it != map.end())
		if (it->first.substr(0, value.size()) != value)
			it = map.erase(it);
		else
			++it;
	if (map.size() > 1)
		throw BadOptions("Ambiguous argument value: " + value);
	if (map.empty())
		throw BadOptions("Unknown argument value: " + value);
	return map.begin()->second * mul;
}



std::string OptionsHelper::stripComma(const std::string &s)
{
	return s.substr(0, s.find(','));
}

void OptionsHelper::addCommandLineFlag(const std::string &name, bool *target, const char *description)
{
	using namespace boost::program_options;
	commandLineDescription_.add_options()(name.c_str(), bool_switch(target), description);
}

void OptionsHelper::addIndexedOption(const std::string &name, int *target,
		const IndexedArgument &arg, const char *description)
{
	using namespace boost::program_options;
	commandLineDescription_.add_options()(name.c_str(), value<std::string>(), description);
	configFileDescription_.add_options()(name.c_str(), value<std::string>(), description);
	indexedOptions_.push_back(IndexedType(stripComma(name), arg, target));
}

void OptionsHelper::addIndexedListOption(const std::string & name, std::vector<int> *target,
		const IndexedArgument & arg, const char *description)
{
	using namespace boost::program_options;
	commandLineDescription_.add_options()(name.c_str(), value<std::string>(), description);
	configFileDescription_.add_options()(name.c_str(), value<std::string>(), description);
	indexedListOptions_.push_back(IndexedListType(stripComma(name), arg, target));
}

void OptionsHelper::parseCommandLine(int argc, char **argv)
{
	using namespace boost::program_options;
	variables_map vm;
	options_description desc(commandLineDescription_);
	desc.add(positionalParametersDescription_);
	store(command_line_parser(argc, argv).
				options(desc).
				positional(positionalParameters_).
				run(), vm);
	notify(vm);
	doParse(vm);
}

void OptionsHelper::parseConfigFile(const char *configFile)
{
	using namespace boost::program_options;
	variables_map vm;
	store(parse_config_file<char>(configFile, configFileDescription_, false), vm);
	notify(vm);
	doParse(vm);
}

void OptionsHelper::doParse(const boost::program_options::variables_map &vm)
{
	for (IndexedType& value: indexedOptions_) {
		if (vm.count(std::get<0>(value)) == 0)
			continue;
		*(std::get<2>(value)) = std::get<1>(value).getElement(vm[std::get<0>(value)].as<std::string>());
	}
	for (IndexedListType& value: indexedListOptions_) {
		if (vm.count(std::get<0>(value)) == 0)
			continue;
		std::vector<std::string> sv;
		boost::split(sv, vm[std::get<0>(value)].as<std::string>(),
				boost::is_any_of(", "), boost::token_compress_on);
		std::vector<int> *res = std::get<2>(value);
		assert(res != NULL);
		res->clear();
		for (const std::string& str: sv) {
			res->push_back(std::get<1>(value).getElement(str));
		}
	}
}

void OptionsHelper::print(const char *programName) const
{
	using namespace std;
	cerr << "Usage:" << endl;
	cerr << "    " << programName << " [options]";
	for (std::size_t i = 0; i < positionalParameters_.max_total_count(); ++i)
		cerr << " " << positionalParameters_.name_for_position(i) << " ";
	cerr << endl << endl << "Options:" << endl;
	cerr << commandLineDescription_ << endl;
}
