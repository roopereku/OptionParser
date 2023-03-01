#ifndef OPTION_DETAIL_HH
#define OPTION_DETAIL_HH

#include <optional>
#include <string>
#include <queue>

class OptionParser;
class OptionDetail
{
public:
	OptionDetail(OptionParser& opts, const char* longName, char shortName) :
		opts(opts), longName(longName), shortName(shortName)
	{
	}

	void fail();
	bool exists();

	friend class OptionParser;

protected:
	virtual bool requiresValue()=0;

	std::queue <const char*> values;
	OptionParser& opts;

	const char* descriptionText = "No description";
	const char* longName;
	char shortName;
};

template <typename T>
class OptionWithValue : public OptionDetail
{
public:
	OptionWithValue(OptionParser& opts, const char* longName, char shortName) :
		OptionDetail(opts, longName, shortName)
	{
	}
	
	OptionWithValue& defaultValue(const T& value);
	OptionWithValue& description(const char* value);

	operator T();
	bool requiresValue() override { return true; }

private:
	std::optional <T> defValue;
};

template <>
OptionWithValue <const char*>::operator const char*();

template <>
OptionWithValue <std::string>::operator std::string();

class OptionWithoutValue : public OptionDetail
{
public:
	OptionWithoutValue(OptionParser& opts, const char* longName, char shortName) :
		OptionDetail(opts, longName, shortName)
	{
	}

	OptionWithoutValue& description(const char* value);

	operator bool();
	bool requiresValue() override { return false; }
};

#endif
