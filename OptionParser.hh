#ifndef OPTION_PARSER_HEADER
#define OPTION_PARSER_HEADER

#include <sstream>
#include <vector>

class OptionParser
{
public:
	OptionParser(int argc, char** argv);
	~OptionParser() {}

	size_t describe(const char* longName, char shortName, const char* description, bool value);
	size_t describe(const char* longName, const char* description, bool value);

	template <typename T>
	bool find(size_t descriptor, T& value)
	{
		//	Find the next possible passed option matching the descriptor
		size_t passedOptionIndex = findPassedOption(descriptor);
		if(passedOptionIndex == -1UL) return false;

		//	Safety check because this function can be called when user passes no value
		if(descriptors[descriptor].requireValue)
		{
			//	Put the value of the passed option into a stringstream
			PassedOption& opt = passed[passedOptionIndex];
			std::istringstream ss(std::string(opt.value));

			//	Return the value back to the user
			ss >> value;
		}

		return true;
	}

	//	Returns the CLI arguments that are not options
	const std::vector <char*>& getArguments();

	bool find(size_t descriptor);
	bool undescribed();
	void list();

private:
	struct PassedOption
	{
		PassedOption(char* name, bool isLong)
			: name(name), isLong(isLong)
		{}

		char* name = nullptr;
		char* value = nullptr;

		bool isLong;
		bool consumed = false;
	};

	struct Descriptor
	{
		Descriptor(const char* longName, char shortName, const char* description, bool value)
			:	description(description), longName(longName),
				shortName(shortName), requireValue(value)
		{}

		//	Helper to check if a descriptor matches a passed option by name
		bool operator==(const PassedOption& rhs);

		const char* description;
		const char* longName;
		char shortName;

		bool requireValue;
	};

	//	Finds a passed option described by descriptor
	size_t findPassedOption(size_t descriptor);

	//	Options that the user passed on the CLI
	std::vector <PassedOption> passed;

	//	These are used to check if an option is valid
	std::vector <Descriptor> descriptors;

	//	CLI arguments that weren't options
	std::vector <char*> nonOptions;
};

#endif
