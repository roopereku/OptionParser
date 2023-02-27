#ifndef OPTION_PARSER_TCC
#define OPTION_PARSER_TCC

#include <sstream>

template <typename T>
OptionWithValue <T>& OptionParser::add(const char* longName, char shortName)
{
	options.emplace_back(std::make_shared <OptionWithValue <T>> (*this, longName, shortName));
	options.back()->offset = findKey(*options.back());
	return static_cast <OptionWithValue <T>&> (*options.back());
}

template <typename T>
OptionWithValue <T>& OptionParser::add(char shortName)
{
	return add <T> (nullptr, shortName);
}

template <typename T>
bool OptionParser::getValue(OptionWithValue <T>& opt, T& value)
{
	const char* ret = consumeValue(opt);
	if(ret == nullptr) return false;

	// TODO Check if stringstream fails
	std::stringstream ss(ret);
	ss >> value;

	return true;
}

template <typename T>
OptionWithValue <T>::operator T()
{
	T value;
	return opts.getValue(*this, value)
		? value : (!defValue ? (fail(), value) : defValue.value());
}

#endif
