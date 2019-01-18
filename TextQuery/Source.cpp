#pragma once
#include "TextQuery.h"
#include <iostream>
#include <cctype>
#include "Query.h"

int main() {
	TextQuery tq("file.txt");
	//std::string sought("");
	//while (std::tolower(sought[0]) != 'q') {
	//	std::cout << "Enter string: ";
	//	std::getline(std::cin, sought);
	//	tq.query(sought).print();
	//}
	Query q = Query("Proin") & Query("lorem") & Query("sit") ;
	q.eval(tq).print();

	int i;
	std::cin >> i;
	return 0;
}