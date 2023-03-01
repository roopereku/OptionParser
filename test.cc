#include <OptionParser.hh>

#include <cstdio>

int verboseLevel = 0;

void usePort(int port)
{
	if(verboseLevel > 0) printf("debug log 1\n");
	if(verboseLevel > 1) printf("debug log 2\n");

	printf("port is %d\n", port);
}

int main(int argc, char** argv)
{
	OptionParser opts(argc, argv);

	auto& verbose = opts.addSwitch('v')
		.description("Stackable verbose mode");

	auto& port = opts.add <int> ("port", 'p')
		.description("What port to listen on");

	while(verbose)
		verboseLevel++;

	usePort(port);
}
