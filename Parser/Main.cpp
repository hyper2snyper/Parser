#include <iostream>
#include "String.h"
#include "Parser.h"

int main(int argc, char** argv)
{
	char cstr[200]{};

	while (true)
	{
		std::cin.getline(cstr, 200);
		if (cstr[0] == 'q') break;

		Parser parser{};
		parser.parse(cstr);

		parser.evaluate();
	}
}


