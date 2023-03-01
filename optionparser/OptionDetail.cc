#include "OptionDetail.hh"
#include "OptionParser.hh"

void OptionDetail::fail()
{
	if(longName) printf("Missing option --%s\n", longName);
	else if(shortName) printf("Missing option -%c\n", shortName);

	std::quick_exit(1);
}

bool OptionDetail::exists()
{
	return !values.empty();
}

OptionWithoutValue& OptionWithoutValue::description(const char* value)
{
	descriptionText = value;
	return *this;
}

OptionWithoutValue::operator bool()
{
	opts.validateArguments();
	bool ret = exists();
	values.pop();

	return ret;
}

template <>
OptionWithValue <const char*>::operator const char*()
{
	opts.validateArguments();

	// If there are no remaining values, return the default value or error out
	if(values.empty())
		return defValue ? defValue.value() : (fail(), "");

	const char* value = values.front();
	return values.pop(), value;

}

template <>
OptionWithValue <std::string>::operator std::string()
{
	opts.validateArguments();

	// If there are no remaining values, return the default value or error out
	if(values.empty())
		return defValue ? defValue.value() : (fail(), std::string());

	std::string value = std::string(values.front());
	return values.pop(), value;
}
