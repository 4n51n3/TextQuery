#include "QueryResult.h"



QueryResult::QueryResult(std::string _sought, std::shared_ptr<lines_data> _lines,
	const std::set<uint>& _line_numbers) :
	sought(_sought), lines(_lines), line_numbers(_line_numbers)
{
}

QueryResult::~QueryResult()
{
}

void QueryResult::print() const
{
	
	std::cout << "Search for '"<< sought << "'" << std::endl;
	if (line_numbers.empty()) {
		std::cout << "Nothing found." << std::endl;
		return;
	}
	for (auto i : line_numbers) {
		std::cout << "(" << i << " line): " << *(*lines)[i - 1] << std::endl;
	}
}