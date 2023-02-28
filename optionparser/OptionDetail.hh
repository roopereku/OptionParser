#ifndef OPTION_DETAIL_HH
#define OPTION_DETAIL_HH

#include <optional>

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
	OptionParser& opts;
	int offset = 0;

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

private:
	std::optional <T> defValue;
};

class OptionWithoutValue : public OptionDetail
{
public:
	OptionWithoutValue(OptionParser& opts, const char* longName, char shortName) :
		OptionDetail(opts, longName, shortName)
	{
	}

	OptionWithoutValue& description(const char* value);

	operator bool();
};

#endif
