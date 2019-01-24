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
	std::shared_ptr<std::set<uint>> line_numbers;
	std::string sought;
public:

	explicit QueryResult(const std::string& _sought) : lines(std::make_shared<lines_data>()), 
		line_numbers(std::make_shared<std::set<uint>>()), sought(_sought)
	{
	}

	explicit QueryResult(const std::string&& _sought) : lines(std::make_shared<lines_data>()),
		line_numbers(std::make_shared<std::set<uint>>()), sought(std::move(_sought))
	{
	}

	QueryResult(std::string _sought, std::shared_ptr<lines_data> _lines,
		std::shared_ptr<std::set<uint>> _line_numbers);
	~QueryResult();
	void print() const;
	std::shared_ptr<lines_data> getData() const;
	std::shared_ptr<std::set<uint>> getLines() const;
	std::string getSought() const;
	bool isEmpty() const {
		return line_numbers->empty();
	}
};

