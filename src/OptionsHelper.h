#ifndef OPTIONSHELPER_H_
#define OPTIONSHELPER_H_

#include <boost/program_options.hpp>
#include <tuple>
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
	bool allowMinus_;
public:
	IndexedArgument():allowMinus_(false) {}
	void addElement(const std::string &name, int index);
	int getElement(std::string value) const;
	bool allowMinus() const { return allowMinus_; }
	void allowMinus(bool value) { allowMinus_ = value; }
};

class OptionsHelper {
	typedef std::pair<std::string, bool*> FlagType;
	typedef std::tuple<std::string, IndexedArgument, int*> IndexedType;
	typedef std::tuple<std::string, IndexedArgument, std::vector<int>* > IndexedListType;

	boost::program_options::options_description commandLineDescription_;
	boost::program_options::options_description configFileDescription_;
	boost::program_options::options_description positionalParametersDescription_;
	boost::program_options::positional_options_description positionalParameters_;
	std::vector<FlagType> commandLineFlags_;
	std::vector<IndexedType> indexedOptions_;
	std::vector<IndexedListType> indexedListOptions_;

	void doParse(const boost::program_options::variables_map &vm);
	static std::string stripComma(const std::string &s);
public:
	void addCommandLineFlag(const std::string &name, bool *target, const char *description);
	void addBoolOption(const std::string &name, bool *target, const char *description)
	{
		addArgumentOption<bool>(name, target, description);
	}

	void addIndexedOption(const std::string &name, int *target, const IndexedArgument &arg,
			const char *description);
	void addIndexedListOption(const std::string &name, std::vector<int> *target,
			const IndexedArgument &arg, const char *description);

	template <class T>
	void addArgumentOption(const std::string &name, T *target, const char *description)
	{
		using namespace boost::program_options;
		commandLineDescription_.add_options()
				(name.c_str(), value<T>(target), description);
		configFileDescription_.add_options()
				(name.c_str(), value<T>(target), description);
	}
	template <class T>
	void addPositionalParameter(const std::string &name, T *target, const char *description)
	{
		using namespace boost::program_options;
		positionalParametersDescription_.add_options()(name.c_str(), value<T>(target), description);
		positionalParameters_.add(name.c_str(), 1);
	}

	void parseCommandLine(int argc, char **argv);
	void parseConfigFile(const char *configFile);
	void print(const char *programName) const;
};


#endif /* OPTIONSHELPER_H_ */
