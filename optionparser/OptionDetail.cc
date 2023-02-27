#include "OptionDetail.hh"
#include "OptionParser.hh"

void OptionDetail::fail()
{
	printf("Fail\n");
	std::quick_exit(1);
}

bool OptionDetail::remaining()
{
	return offset != -1;
}
