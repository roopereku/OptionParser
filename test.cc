#include <OptionParser.hh>

#include <cstdio>
#include <vector>
#include <string>
#include <memory>

int main(int argc, char** argv)
{
	OptionParser opts(argc, argv);

	auto& test = opts.add <int> ("test1");
	auto& test2 = opts.add <int> ("test2");

	while(test.remaining())
		printf("Value is %d\n", (int)test);

	while(test2.remaining())
		printf("Value2 is %d\n", (int)test2);
}
