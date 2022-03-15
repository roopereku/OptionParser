#include "OptionParser.hh"

#include <cstring>

OptionParser::OptionParser(int argc, char** argv)
{
	//	Loop through argv excluding the program name
	for(int i = 1; i < argc; i++)
	{
		unsigned hyphens = 0;

		//	How many hyphens are there
		while(argv[i][hyphens] == '-') hyphens++;

		//	TODO make sure that the option names aren't whitespace

		//	Short option
		if(hyphens == 1)
		{
			/*	Because options like -abc should be parsed like
			 *	-a -b -c, add each character as their own option */
			for(size_t c = 1; argv[i][c] != 0; c++)
				passed.emplace_back(argv[i] + c, false);
		}

		//	Long option
		else if(hyphens == 2)
		{
			size_t length = strlen(argv[i]);

			//	If the option is "--", append the rest to nonOptions
			if(length == 2)
			{
				for(i++; i < argc; i++)
					nonOptions.emplace_back(argv[i]);

				break;
			}

			//	Add a long option excluding the hyphens
			passed.emplace_back(argv[i] + 2, true);
		}

		//	This is not an option
		else
		{
			//	If the latest option doesn't have a value, assume this to be it
			if(!passed.empty() && passed.back().value == nullptr)
			{
				passed.back().value = argv[i];
				continue;
			}

			nonOptions.emplace_back(argv[i]);
			continue;
		}
	}
}

size_t OptionParser::describe(const char* longName, char shortName, const char* description, bool value)
{
	//	TODO make sure that this descriptor is unique

	size_t index = descriptors.size();
	descriptors.emplace_back(longName, shortName, description, value);

	//	Do some validation for each passed option matching this descriptor
	for(auto& opt : passed)
	{
		if(descriptors[index] == opt)
		{
			/*	If the descriptor doesn't take a value but the user passed
			 *	a value, move the value to non-options */
			if(!value && opt.value)
			{
				printf("Moving '%s'\n", opt.value);
				nonOptions.emplace_back(opt.value);
				opt.value = nullptr;
			}

			//	If the descriptor requires a value but none was passed, report an error
			else if(value && !opt.value)
			{
				//	TODO show the short name if it was passed
				fprintf(stderr, "Option '%s' requires a value\n", descriptors[index].longName);
				return -1;
			}
		}
	}

	return index;
}

size_t OptionParser::describe(const char* longName, const char* description, bool value)
{
	return describe(longName, 0, description, value);
}

bool OptionParser::find(size_t descriptor)
{
	return findPassedOption(descriptor) != -1UL;
}

const std::vector <char*>& OptionParser::getArguments()
{
	return nonOptions;
}

size_t OptionParser::findPassedOption(size_t descriptor)
{
	//	There's no such descriptor
	if(descriptor >= descriptors.size())
		return -1UL;

	for(size_t i = 0; i < passed.size(); i++)
	{
		//	Ignore consumed options
		if(passed[i].consumed) continue;

		//	Does the passed option match the descriptor
		if(descriptors[descriptor] == passed[i])
		{
			passed[i].consumed = true;
			return i;
		}
	}

	//	No matches
	return -1UL;
}

bool OptionParser::undescribed()
{
	/*	For each option, check if said option matches
	 *	some descriptor. If there's even a single invalid option,
	 *	report an error and stop */

	for(auto& opt : passed)
	{
		bool found = false;

		for(auto& desc : descriptors)
		{
			if(desc == opt)
			{
				found = true;
				break;
			}
		}

		if(!found)
		{
			//	TODO show the short name if the user passed it
			printf("Invalid option '%s'\n\n", opt.name);
			list();
			return true;
		}
	}

	return false;
}

void OptionParser::list()
{
	int longestName = 0;

	//	Find the longest "long name" which will be user later for alignment
	for(auto& opt : descriptors)
	{
		int length = strlen(opt.longName);
		if(length > longestName)
			longestName = length;
	}

	printf("Valid options:\n");

	for(auto& opt : descriptors)
	{
		//	The length between long and short name
		int gap = longestName + 5 - strlen(opt.longName);
		bool sn = opt.shortName != 0;

		printf("  %s%*c%c     %s\n",
				opt.longName, gap,
				sn ? '-' : ' ', sn ? opt.shortName : ' ',
				opt.description);
	}
}

bool OptionParser::Descriptor::operator==(const PassedOption& rhs)
{
	if(rhs.isLong)
	{
		//	TODO the strings contained could be std::strings
		//	Does the long name match
		if(!strcmp(rhs.name, longName))
			return true;
	}

	//	Does the short name match
	else if(rhs.name[0] == shortName) return true;

	return false;
}
