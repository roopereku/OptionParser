#include "../OptionParser.hh"

int main(int argc, char** argv)
{
	OptionParser opt(argc, argv);

	auto test = opt.describe("test", '1', "Test option", false);
	auto test2 = opt.describe("test2", '2', "Test option 2", true);
	auto test3 = opt.describe("test3", '3', "Test option 2", true);

	if(opt.undescribed())
		return 1;

	int a = 0;

	while(opt.find(test2, a))
		printf("found test %d\n", a);

	while(opt.find(test))
		printf("test\n");

	opt.list();
}
