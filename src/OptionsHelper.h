#ifndef OPTIONSHELPER_H_
#define OPTIONSHELPER_H_

#include <boost/program_options.hpp>
#include <boost/tuple/tuple.hpp>
#include <vector>
#include <string>

class BadOptions: public std::exception {
	std::string message_;
public:
	BadOptions(const std::string &msg) throw():message_(msg) {
	}
	~BadOptions() throw() {}
	virtual const char* what() const throw() {
		return message_.c_str();
	}
};

class IndexedArgument {
	typedef std::pair<std::string, int> EntryType;
	typedef std::vector<EntryType> MapType;

	MapType map_;
public:
	void addElement(const std::string name, int index);
	int getElement(const std::string value) const;
};

class OptionsHelper {
	typedef std::pair<std::string, bool*> FlagType;
	typedef boost::tuple<std::string, IndexedArgument, int*> IndexedType;

	boost::program_options::options_description commandLineDescription_;
	boost::program_options::options_description configFileDescription_;
	boost::program_options::options_description positionalParametersDescription_;
	boost::program_options::positional_options_description positionalParameters_;
	std::vector<FlagType> commandLineFlags_;
	std::vector<FlagType> boolOptions_;
	std::vector<IndexedType> indexedOptions_;

	void doParse(const boost::program_options::variables_map &vm);
	static std::string stripComma(const std::string &s);
public:
	void addCommandLineFlag(const std::string &name, bool *target, const char *description);
	void addBoolOption(const std::string &name, bool *target, const char *description);
	void addIndexedOption(const std::string &name, int *target, const IndexedArgument &arg,
			const char *description);

	template <class T>
	void addArgumentOption(const std::string &name, T *target, const char *description);
	template <class T>
	void addPositionalParameter(const std::string &name, T *target, const char *description);

	void parseCommandLine(int argc, char **argv);
	void parseConfigFile(const char *configFile);
	void print() const;
};


template <class T>
void OptionsHelper::addArgumentOption(const std::string &name, T *target, const char *description)
{
	using namespace boost::program_options;
	commandLineDescription_.add_options()(name.c_str(), value<T>(target), description);
	configFileDescription_.add_options()(name.c_str(), value<T>(target), description);
}

template <class T>
void OptionsHelper::addPositionalParameter(const std::string &name, T *target, const char *description)
{
	using namespace boost::program_options;
	positionalParametersDescription_.add_options()(name.c_str(), value<T>(target), description);
	positionalParameters_.add(name.c_str(), 1);
}


#endif /* OPTIONSHELPER_H_ */
