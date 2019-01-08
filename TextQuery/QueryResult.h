#pragma once
#include <vector>
#include <memory>
#include <set>
#include <iostream>
#include <string>
typedef unsigned int uint;
typedef std::vector<std::shared_ptr<std::string>> lines_data;
class QueryResult
{
	std::shared_ptr<lines_data> lines;
	std::set<uint> line_numbers;
	std::string sought;
public:
	QueryResult() = default;
	QueryResult(std::string _sought, std::shared_ptr<lines_data> _lines,const std::set<uint>& _line_numbers);
	~QueryResult();
	void print() const;
};

