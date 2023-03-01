#ifndef OPTION_PARSER_TCC
#define OPTION_PARSER_TCC

#include <sstream>

template <typename T>
OptionWithValue <T>& OptionParser::add(const char* longName, char shortName)
{
	// TODO Check if the given names already exist
	options.emplace_back(std::make_shared <OptionWithValue <T>> (*this, longName, shortName));
	fillValues(*options.back());
	return static_cast <OptionWithValue <T>&> (*options.back());
}

template <typename T>
OptionWithValue <T>& OptionParser::add(char shortName)
{
	return add <T> (nullptr, shortName);
}

template <typename T>
OptionWithValue <T>::operator T()
{
	opts.validateArguments();
	T value;

	// If there are no remaining values, return the default value or error out
	if(values.empty())
		return defValue ? defValue.value() : (fail(), value);

	// Convert the next value to the given format
	std::stringstream ss(values.front());
	ss >> value;

	return values.pop(), value;
}

template <typename T>
OptionWithValue <T>& OptionWithValue <T>::description(const char* value)
{
	descriptionText = value;
	return *this;
}

template <typename T>
OptionWithValue <T>& OptionWithValue <T>::defaultValue(const T& value)
{
	defValue.emplace(value);
	return *this;
}

#endif
