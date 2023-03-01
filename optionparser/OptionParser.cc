#include <OptionParser.hh>

#include <cstdio>
#include <cstring>

OptionParser::OptionParser(int argc, char** argv) : argc(argc - 1), argv(argv + 1)
{
	for(int i = 0; i < this->argc; i++)
	{
		size_t hyphenCount = getHyphenCount(this->argv[i]);

		/* Options stop at the current argument if it has
		 * 2 hyphens and a null terminator after them */
		if(hyphenCount == 2 && *(this->argv[i] + 2) == 0)
		{
			optionsEnd = i;
			return;
		}
	}

	optionsEnd = this->argc;
}

size_t OptionParser::getHyphenCount(const char* str)
{
	size_t count = 0;
	while(str[count] == '-')
		count++;

	return count;
}

bool OptionParser::isValue(int index)
{
	return	index < optionsEnd &&
			argv[index] != nullptr &&
			argv[index][0] != '-';
}

void OptionParser::fillValue(OptionDetail& opt, int& i, bool longName, char* equals)
{
	// If the option requires a value, use the attached value or the next argument
	if(opt.requiresValue())
	{
		// Is there no "=" symbol?
		if(!equals)
		{
			// Make sure that the next argument is a valid value
			if(!isValue(i + 1))
			{
				if(longName) printf("Option --%s requires a value\n", opt.longName);
				else printf("Option -%c requires a value\n", opt.shortName);

				std::quick_exit(1);
			}

			// Save the next argument to the given option
			opt.values.push(argv[++i]);
			argv[i] = nullptr;
		}

		// Save the attached value to the given option
		else opt.values.push(equals + 1);
	}

	// Error out if a value is attached to a switch option
	else if(equals)
	{
		if(longName) printf("Option --%s doesn't take a value\n", opt.longName);
		else printf("Option -%c doesn't take a value\n", opt.shortName);

		std::quick_exit(1);
	}

	// Save a blank value to a switch option
	else opt.values.push("");
}

void OptionParser::fillValues(OptionDetail& opt)
{
	for(int i = 0; i < optionsEnd; i++)
	{
		// Don't process consumed arguments
		if(argv[i] == nullptr)
			continue;

		size_t hyphenCount = getHyphenCount(argv[i]);
		bool found = false;
		bool longName;

		// If there's an attached value, null terminate the delimiter
		char* equals = strchr(argv[i], '=');
		if(equals) *equals = 0;

		if(hyphenCount == 2 && opt.longName)
		{
			// Does the current argument match the given option?
			if(strcmp(argv[i] + hyphenCount, opt.longName) == 0)
			{
				found = true;
				argv[i] = nullptr;

				// Add a new value to the matching option
				fillValue(opt, i, true, equals);
			}
		}

		if(hyphenCount == 1 && opt.shortName)
		{
			bool isLast = false;
			for(char* c = argv[i] + 1; *c != 0; c++)
			{
				// Does the current character match the given option?
				if(*c == opt.shortName)
				{
					/* The current character is the last one if
					 * the next character is a null terminator */
					isLast = *(c + 1) == 0;

					// Only the last character can receive a value
					if(opt.requiresValue() && !isLast)
					{
						printf("Option -%c requires a value\n", *c);
						std::quick_exit(1);
					}

					// "Cancel" the current character as it's no longer needed
					*c = 0x18;
					found = true;

					// Add a new value to the matching option
					fillValue(opt, i, false, isLast ? equals : nullptr);
				}
			}

			/* If the matching option was the last character in a group
			 * of short options, let's not restore the "=" symbol */
			if(isLast) continue;
		}

		if(equals) *equals = '=';
	}
}

OptionWithoutValue& OptionParser::addSwitch(const char* longName, char shortName)
{
	// TODO Check if the given names already exist
	options.emplace_back(std::make_shared <OptionWithoutValue> (*this, longName, shortName));
	fillValues(*options.back());
	return static_cast <OptionWithoutValue&> (*options.back());
}

OptionWithoutValue& OptionParser::addSwitch(char shortName)
{
	return addSwitch(nullptr, shortName);
}

void OptionParser::validateArguments()
{
	if(validationDone)
		return;

	for(int i = 0; i < argc; i++)
	{
		if(argv[i] == nullptr || i == optionsEnd)
			continue;

		size_t hyphenCount = getHyphenCount(argv[i]);

		if(hyphenCount == 2)
		{
			printf("Invalid option '%s'\n", argv[i]);
			listOptions();
		}

		else if(hyphenCount == 1)
		{
			for(char* c = argv[i] + 1; *c != 0; c++)
			{
				// Don't process "Cancelled" characters
				if(*c != 0x18)
					continue;

				printf("Invalid option -%c\n", *c);
				listOptions();
			}
		}
	}

	validationDone = true;
}

void OptionParser::listOptions()
{
	// Find the longest "long" name
	size_t longestName = 0;
	for(auto& opt : options)
		longestName = std::max(longestName, opt->longName ? strlen(opt->longName) : 0);

	printf("Valid options:\n");
	for(auto& opt : options)
	{
		// Calculate a gap that aligns each description
		unsigned gap = longestName - (opt->longName ? strlen(opt->longName) : 0) + 3;

		// Output each option nicely formatted
		printf(" %c%c  %s%s%*c%s\n",
			opt->shortName ? '-' : ' ', opt->shortName ? opt->shortName : ' ',
			opt->longName ? "--" : "  ", opt->longName ? opt->longName :  "",
			gap, ' ', opt->descriptionText);
	}

	std::quick_exit(1);
}
