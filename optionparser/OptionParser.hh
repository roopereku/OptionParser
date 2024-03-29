#ifndef OPTION_PARSER_HH
#define OPTION_PARSER_HH

#include <OptionDetail.hh>

#include <vector>
#include <memory>

class OptionParser
{
public:
	OptionParser(int argc, char** argv, bool addHelp = true);

	template <typename T>
	OptionWithValue <T>& add(const char* longName, char shortName = 0);	

	template <typename T>
	OptionWithValue <T>& add(char shortName);

	OptionWithoutValue& addSwitch(const char* longName, char shortName = 0);
	OptionWithoutValue& addSwitch(char shortName);

	void validateArguments();
	const char* nextArgument();

private:
	void fillValue(OptionDetail& opt, int& i, bool longName, char* equals);
	void fillValues(OptionDetail& opt);
	void listOptions();

	size_t getHyphenCount(const char* str);
	bool isValue(int index);

	int argc;
	int optionsEnd;
	bool validationDone = false;

	char** argv;
	std::vector <std::shared_ptr <OptionDetail>> options;
	OptionWithoutValue* help = nullptr;
};

#include <OptionParser.tcc>
#endif
