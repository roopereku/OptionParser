# OptionParser

OptionParser is an easy-to-use and lightweight CLI option parser library.

## Usage
Below is an example of how you can add OptionParser to your CMake project
```c
cmake_minimum_required(VERSION 3.0)
project(test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_subdirectory(OptionParser)

add_executable(test main.cc)
target_link_libraries(test OptionParser)
```

## Example
```cpp
#include <OptionParser.hh>

void foo(float val) {}

int main(int argc, char** argv)
{
	OptionParser opts(argc, argv);

	// Let's add a required option that expects an integer as a value.
	// You can make an option be required by not supplying a default value.
	auto& test = opts.add <int> ("test", 't').
		description("This option is required");

	// Let's make the next one optional by giving it a default value
	auto& optional = opts.add <float> ("optional")
		.defaultValue(1.23f)
		.description("This option isn't required");

	// Because test has the type int, it can be used like an int.
	// Make sure to not add any new options after you use one for the first time
	// because argument validation takes place at that point.
	int a = test;
	foo(optional);

	// You can use the exists() function to check if there are more
	// occurences of the given option. Let's print the rest of the values
	while(test.exists())
		printf("test has value %d\n", (int)test);

	// You can also get the arguments that are not related to any options
	const char* argument;
	while((argument = opts.nextArgument()) != nullptr)
		printf("Non-option '%s'\n", argument);
}
```
