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

bool OptionParser::find(OptionWithoutValue& opt)
{
	bool longName;
	const char* attachedValue = nullptr;
	int key = consumeKey(opt, attachedValue, longName);

	// Switch options are not supposed to take values
	if(attachedValue)
	{
		if(longName) printf("Option --%s doesn't take a value\n", opt.longName);
		else printf("Option -%c doesn't take a value\n", opt.shortName);

		std::quick_exit(1);
	}

	// Was the option found?
	return key != -1;
}

bool OptionParser::getValue(OptionWithValue <const char*>& opt, const char*& value)
{
	const char* ret = consumeValue(opt);
	if(ret == nullptr) return false;

	value = ret;
	return true;
}

bool OptionParser::getValue(OptionWithValue <std::string>& opt, std::string& value)
{
	const char* ret = consumeValue(opt);
	if(ret == nullptr) return false;

	value = std::string(ret);
	return true;
}

const char* OptionParser::consumeValue(OptionDetail& opt)
{
	const char* value;
	const char* attachedValue = "";

	bool longName;
	int key = consumeKey(opt, attachedValue, longName);

	if(key == -1)
		return nullptr;

	/* consumeKey can null attachedValue if a found option is
	 * accessed with it's short name, and there's another
	 * short name right after it. This means that the found
	 * option doesn't have a value */
	if(attachedValue == nullptr)
	{
		if(longName) printf("Option --%s requires a value\n", opt.longName);
		else printf("Option -%c requires a value\n", opt.shortName);

		std::quick_exit(1);
	}

	/* The attached value hasn't changed if the
	 * first character is a null terminator */
	if(*attachedValue == 0)
	{
		// Is the next argument not a valid value
		if(!isValue(key + 1))
		{
			if(longName) printf("Option --%s requires a value\n", opt.longName);
			else printf("Option -%c requires a value\n", opt.shortName);

			std::quick_exit(1);
		}

		// Save and consume the next argument
		value = argv[key + 1];
		argv[key + 1] = nullptr;
	}

	// Use the attached value as the result value
	else value = attachedValue;

	return value;
}

int OptionParser::findKey(OptionDetail& opt)
{
	for(int i = opt.offset; i < optionsEnd; i++)
	{
		// Don't process consumed arguments
		if(argv[i] == nullptr)
			continue;

		size_t hyphenCount = getHyphenCount(argv[i]);
		size_t keyLength = 0;

		// Find the length of the argument or the length up until '='
		while(argv[i][keyLength] != '=' && argv[i][keyLength] != 0)
			keyLength++;

		// Remove the hyphens from the key length
		keyLength -= hyphenCount;

		// Is the argument a long option?
		if(hyphenCount == 2 && opt.longName)
		{
			// Does the argument match the option that we're looking for
			if(strncmp(argv[i] + 2, opt.longName, keyLength) == 0)
				return i;
		}

		// Is the argument a short option
		if(hyphenCount == 1 && opt.shortName)
		{
			// Does one of the short options match what we're looking for
			for(char* c = argv[i] + 1; *c != 0; c++)
			{
				if(*c == opt.shortName)
					return i;
			}
		}
	}

	return -1;
}

int OptionParser::consumeKey(OptionDetail& opt, const char*& attachedValue, bool& longName)
{
	if(!validationDone)
		validateOptions();

	if(opt.offset == -1)
		return -1;

	size_t hyphenCount = getHyphenCount(argv[opt.offset]);

	char* equals = strchr(argv[opt.offset], '=');
	if(equals) *equals = 0;

	if(hyphenCount == 2)
	{
		// If '=', whatever's after it is the attached value
		if(equals) attachedValue = equals + 1;

		longName = true;
		argv[opt.offset] = nullptr;
		return updateOffset(opt);
	}

	else if(hyphenCount == 1)
	{
		longName = false;

		// Loop through each character after the single hyphen
		for(char* c = argv[opt.offset] + 1; *c != 0; c++)
		{
			if(*c == opt.shortName)
			{
				if(equals)
				{
					// Is this not the last character?
					if(*(c + 1) != 0)
					{
						/* Only the option accessed by the last character
						 * can have an attached value */
						attachedValue = nullptr;
						*equals = '=';
					}

					// This is the last character so save the attached value
					else attachedValue = equals + 1;
				}

				// "Cancel" the current character
				*c = 0x18;
				return updateOffset(opt);
			}
		}
	}

	// Revert the '=' symbol if we've not returned yet
	*equals = '=';

	return -1;
}

OptionWithoutValue& OptionParser::addSwitch(const char* longName, char shortName)
{
	options.emplace_back(std::make_shared <OptionWithoutValue> (*this, longName, shortName));
	options.back()->offset = findKey(*options.back());
	return static_cast <OptionWithoutValue&> (*options.back());
}

OptionWithoutValue& OptionParser::addSwitch(char shortName)
{
	return addSwitch(nullptr, shortName);
}

int OptionParser::updateOffset(OptionDetail& opt)
{
	int ret = opt.offset;
	opt.offset = findKey(opt);

	return ret;
}

void OptionParser::validateOptions()
{
	listOptions();
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
}
