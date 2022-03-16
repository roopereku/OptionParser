# Option parser

This is an easy to use command-line-option parser.

# How does this differ from the million other versions?

Most command-line-option parsers expect you to declare valid options
and call a function like `parse()`. This way has some limits because
you likely need to have all of this in one place.

This option parser does `parse()` before the user can even declare
valid options. The advantage of this way is that you can create an
object for `OptionParser`, pass it anywhere and call

- `OptionParser::describe` to describe a new option
- `OptionParser::find` to find the given option and immediately get a value if one has been passed with it

When everything has been described, you can call `OptionParser::undescribed()` which
will return true if the user passed options that haven't been described.

# Example code
How to use this like any other option parser
```cpp
int main(int argc, char** argv)
{
	OptionParser opt(argcm argv);
	
	//	Can be called with --test or -t. Requires a value
	auto test = opt.describe("test", 't', "Description", true);

	//	If --test is never used, value will remain 10
	int value = 10;

	//	If --test 5 was passed, value will now be 5
	if(!opt.find(test, value))
		return 1;
}
```

The same as before but with the options declared and used in their respective places
```cpp
class A
{
public:
	A(OptionParser& opt)
	{
		//	If the option was passed, the value will be saved immediately
		auto optA = opt.describe("value-for-A::value", true);
		if(!opt.find(optA, value)) return;
	}
	
private:
	int value;
}

int main(int argc, char** argv)
{
	OptionParser opts(argc, argv);
	A a(opts);
}
```
When you do this with a bunch of different classes, Suddenly everything becomes way more organized

## Handling multiple instances of the same option
Let's say that you want to have an option called `--add` which adds integers to an array
and use it like `--add 1 --add 2 --add 3 --add 4 --add 5`
```cpp
int main(int argc, char** argv)
{
	OptionParser opts(argc, argv);

	auto add = opts.describe("add", "Adds a value", true);
	std::vector <int> values;
	int temp;
	
	//	While there remains instances of "--add", get their values
	while(opts.find(add, temp))
		values.push_back(temp);
}
```

## Handling invalid options
Once you are done describing valid options, you should call `OptionParser::undescribed()` to check for invalid options

```cpp
int main(int argc, char** argv)
{
	OptionParser opts(argc, argv);

	auto o1 = opts.describe("Option1", '1', "Option 1", true);
	auto o2 = opts.describe("Option2", '2', "Option 2", false);

	//	Quit if the user passed options that were never declared
	if(opts.undescribed())
		return 1;
}
```

# NOTES
Because of the way the `OptionParser` constructor works, the list of non-options might not be complete until
everything is described. This should be rather simple to fix, but for the time being you should **NOT** try
to access said values before you describe everything and call `OptionParser::undescribed()`.

`OptionParser` also relies on `argv` not being modified so it's a good idea to not mess with it. It also modifies
`argv` to some extent so you shouldn't rely on argv being completely original.
