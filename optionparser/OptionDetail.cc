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
	return offset != -1;
}

OptionWithoutValue& OptionWithoutValue::description(const char* value)
{
	descriptionText = value;
	return *this;
}

OptionWithoutValue::operator bool()
{
	return opts.find(*this);
}
