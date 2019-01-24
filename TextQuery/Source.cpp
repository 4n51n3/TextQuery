#pragma once
#include "TextQuery.h"
#include <iostream>
#include <cctype>
#include "Query.h"
#include <algorithm>
#include <regex>
#include <vector>

using std::regex;
using std::vector;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::exception;

//  \"[^&~|]+\"
template<typename T>
T pop(vector<T>& v) {
	if (v.empty()) {
		throw exception("Access to empty variable");
	}
	T tmp(std::move(v.back()));
	v.pop_back();
	return tmp;
}

inline string& cleanString(string& str) {
	str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
	return str;
}

inline void checkString(const string& str) {
	if (str.empty())
		throw std::exception("The string was empty");
}

inline void compute(vector<Query>& operandStack, vector<char>& operatorStack) {
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

Query parse(string str) {


	checkString(cleanString(str));
	vector<Query> operandStack;
	vector<char> operatorStack;
	const static regex operand("\"[^&~|]+\"");
	const static string operators("()&|~");
	for (int i = 0; i < str.size(); ++i) {
		char ch = str[i];
		if (ch == ' ')
			continue;
		if (operators.find(ch) != string::npos) {
			if (ch == ')') {
				/// compute Query
				compute(operandStack, operatorStack);
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

			if (!operatorStack.empty() && operatorStack.back() == '~') {
				pop(operatorStack);
				operandStack.push_back(~Query(substr));
			}
			else {
				operandStack.push_back(Query(substr));
			}
			continue;
		}
		throw std::exception("Unrecognized character");
	}
	/// compute if not enough
	while (!operatorStack.empty())
	{
		compute(operandStack, operatorStack);
	}
	return operandStack.empty() ? Query("") : operandStack.back();
}

int main() {
	TextQuery tq("file.txt");
	while (true) {
		string str;
		cout << "Input query: ";
		std::getline(std::cin, str);
		if (str[0] == 'q' || str[0] == 'Q')
			return 0;
		try {
			parse(str).eval(tq).print();
		}
		catch (std::exception &ex) {
			std::cerr << ex.what() << std::endl;
		}
	}
	Query("text") & Query("text");
	return -1;
}