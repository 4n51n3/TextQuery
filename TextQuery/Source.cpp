#include "TextQuery.h"
#include <iostream>
#include <cctype>

int main() {
	TextQuery tq("file.txt");
	std::string sought("");
	while (std::tolower(sought[0]) != 'q') {
		std::cout << "Enter string: ";
		std::getline(std::cin, sought);
		tq.query(sought).print();
	}

	return 0;
}