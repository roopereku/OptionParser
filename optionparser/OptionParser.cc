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
		if(hyphenCount == 2 && *this->argv[i] + 3 == 0)
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
	int key = consumeKey(opt, attachedValue);

	if(key == -1)
		return nullptr;

	if(attachedValue == nullptr)
	{
		printf("Key found but attached value nullptr\n");
		std::quick_exit(1);
	}

	/* The attached value hasn't changed if the
	 * first character is a null terminator */
	if(*attachedValue == 0)
	{
		if(!isValue(key + 1))
		{
			printf("Expected a value\n");
			return nullptr;
		}

		value = argv[key + 1];
		argv[key + 1] = nullptr;
	}

	else value = attachedValue;

	return value;
}

int OptionParser::findKey(OptionDetail& opt)
{
	for(int i = opt.offset; i < optionsEnd; i++)
	{
		if(argv[i] == nullptr)
			continue;

		size_t hyphenCount = getHyphenCount(argv[i]);
		size_t keyLength = 0;

		while(argv[i][keyLength] != '=' && argv[i][keyLength] != 0)
			keyLength++;

		keyLength -= hyphenCount;

		if(hyphenCount == 2 && opt.longName)
		{
			if(strncmp(argv[i] + 2, opt.longName, keyLength) == 0)
			{
				return i;
			}
		}

		if(hyphenCount == 1 && opt.shortName)
		{
			printf("Short options not implemented\n");
			std::quick_exit(1);
		}
	}

	return -1;
}

int OptionParser::consumeKey(OptionDetail& opt, const char*& attachedValue)
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
		if(equals)
			attachedValue = equals + 1;

		argv[opt.offset] = nullptr;
		return updateOffset(opt);
	}

	if(hyphenCount == 1 && opt.shortName)
	{
	}

	return updateOffset(opt);
}

int OptionParser::updateOffset(OptionDetail& opt)
{
	int ret = opt.offset;
	opt.offset = findKey(opt);

	return ret;
}

void OptionParser::validateOptions()
{
	validationDone = true;
}
