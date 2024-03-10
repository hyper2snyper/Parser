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

		if (String::str_eq(cstr, "clear"))
		{
#ifdef _WIN32
			system("cls");
#elif
			system("clear");
#endif
			continue;
		}

		Parser parser{};
		parser.parse(cstr);

		std::cout << "===========================\n\n";
		parser.print_tree();
		std::cout << "\n===========================\n\n";

		parser.evaluate();
	}
}


