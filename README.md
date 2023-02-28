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

	// Let's add a required option that expects an integer as a value
	auto& test = opts.add <int> ("test", 't').
		description("This option is required");

	// Let's make the next one optional by giving it a default value
	auto& optional = opts.add <float> ("optional")
		.defaultValue(1.23f)
		.description("This option isn't required");

	// The options can be used as if they were normal variables
	// Make sure to not add any new options after you use one for the first time
	int a = test;
	foo(optional);
}
```
