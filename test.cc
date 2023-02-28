#include <OptionParser.hh>

#include <cstdio>

class SocketTCP
{
public:
	SocketTCP(const char* host, unsigned port)
	{
		printf("tcp socket at %s:%u\n", host, port);
	}
};

int main(int argc, char** argv)
{
	OptionParser opts(argc, argv);

	auto& useTcp = opts.addSwitch("tcp", 't')
		.description("Use to enable TCP");

	auto& host = opts.add <const char*> ("host")
		.defaultValue("localhost")
		.description("Which address to host on");

	auto& port = opts.add <unsigned> ('p')
		.defaultValue(80)
		.description("What port to host on");

	if(useTcp)
	{
		SocketTCP socket(host, port);
	}
}
