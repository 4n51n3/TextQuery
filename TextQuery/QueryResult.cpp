#include "QueryResult.h"

//
//QueryResult::QueryResult()
//{
//	lines = std::make_shared<lines_data>();
//	line_numbers = std::make_shared<std::set<uint>>();
//}

QueryResult::QueryResult(std::string _sought, std::shared_ptr<lines_data> _lines,
	std::shared_ptr<std::set<uint>> _line_numbers) : sought(_sought),
	lines(_lines), line_numbers(_line_numbers)
{
}

QueryResult::~QueryResult()
{
}

void QueryResult::print() const
{
	using std::endl;
	using std::cout;
	cout << "Search for '"<< sought << '\'' << endl << "Found on (" << line_numbers->size() << ") strings" << endl ;
	if (line_numbers->empty()) {
		cout << "Nothing found." << endl;
		return;
	}
	for (auto i : *line_numbers) {
		cout << "(" << i << " line): " << *(*lines)[i - 1] << '\n';
	}
	cout << endl;
}

std::shared_ptr<lines_data> QueryResult::getData() const
{
	return lines;
}

std::shared_ptr<std::set<uint>> QueryResult::getLines() const
{
	return line_numbers;
}

std::string QueryResult::getSought() const
{
	return sought;
}
