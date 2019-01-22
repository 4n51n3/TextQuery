#pragma once
#include "TextQuery.h"
#include <iostream>
#include <cctype>
#include "Query.h"

#include <regex>
#include <vector>

using std::regex;
using std::vector;


//  \"[^&~|]+\"
template<typename T>
T pop(vector<T>& v) {
	T tmp = v.back();
	v.pop_back();
	return tmp;
}


/// bug: "lorem" & ("leo"|"ipsum")
/// deleting spaces can solve the bug
Query parse(const string& str) {
	vector<Query> operandStack;
	vector<char> operatorStack;
	regex operand("\"[^&~|]+\"");
	string operators("()&|~");
	for (int i = 0; i < str.size(); ++i) {
		char ch = str[i];
		if (ch == ' ')
			continue;
		if (operators.find(ch) != string::npos) {
			if (ch == ')') {
				/// compute Query
				char op = pop(operatorStack);
				auto left = pop(operandStack);
				if (op == '~') {
					operandStack.push_back(~left);
				}
				else {
					auto right = pop(operandStack);
					if (op == '&')
						operandStack.push_back(left & right);
					else if (op == '|')
						operandStack.push_back(left | right);
				}
				operatorStack.pop_back();
				continue;
			}
			operatorStack.push_back(ch);
			continue;
		}
		if (ch == '"') {
			string substr;
			auto startIndex = ++i;
			for (; i < str.size(); ++i) {
				char future = str[i + 1];
				char current = str[i];
				if (current == '"') {
					if (operators.find(future) != string::npos || future == ' ' || future == '\0') {
						substr = str.substr(startIndex, i - startIndex);
						break;
					}
				}
			}
			if (substr.empty())
				throw std::exception("Could not parse operand.");

			if (operatorStack.back() == '~') {
				pop(operatorStack);
				operandStack.push_back(~Query(substr));
			}
			else {
				operandStack.push_back(Query(substr));
			}
			
		}
	}
	/// compute if not enough
	while (!operatorStack.empty())
	{
		char op = pop(operatorStack);
		auto left = pop(operandStack);
		if (op == '~') {
			operandStack.push_back(~left);
		}
		else {
			auto right = pop(operandStack);
			if (op == '&')
				operandStack.push_back(left & right);
			else if (op == '|')
				operandStack.push_back(left | right);
		}
	}
	return operandStack.back();
}

int main() {
	TextQuery tq("file.txt");
	while (true) {
		string str;
		//std::cin >> str; /// bad. interrupts after space
		std::getline(std::cin, str);
		if (str[0] == 'q' || str[0] == 'Q')
			return 0;

		parse(str).eval(tq).print();//parse("\"ipsum\" & \"lorem\"");
	}
	return -1;
}