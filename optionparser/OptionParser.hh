#ifndef OPTION_PARSER_HH
#define OPTION_PARSER_HH

#include <OptionDetail.hh>

#include <vector>
#include <memory>

class OptionParser
{
public:
	OptionParser(int argc, char** argv);

	template <typename T>
	OptionWithValue <T>& add(const char* longName, char shortName = 0);	

	template <typename T>
	OptionWithValue <T>& add(char shortName);

	OptionWithoutValue& addSwitch(const char* longName, char shortName = 0);
	OptionWithoutValue& addSwitch(char shortName);

	template <typename T>
	bool getValue(OptionWithValue <T>& opt, T& value);

	bool getValue(OptionWithValue <const char*>& opt, const char*& value);
	bool getValue(OptionWithValue <std::string>& opt, std::string& value);

	bool find(OptionWithoutValue& opt);

private:
	void validateArguments();
	void listOptions();

	size_t getHyphenCount(const char* str);
	bool isValue(int index);

	const char* consumeValue(OptionDetail& opt);
	int consumeKey(OptionDetail& opt, const char*& attachedValue, bool& longName);

	int findKey(OptionDetail& opt);
	int updateOffset(OptionDetail& opt);

	int argc;
	int optionsEnd;
	bool validationDone = false;

	char** argv;
	std::vector <std::shared_ptr <OptionDetail>> options;
};

#include <OptionParser.tcc>
#endif
