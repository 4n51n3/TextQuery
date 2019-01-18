#pragma once
#include "TextQuery.h"



TextQuery::TextQuery(std::string path)
{
	lines = std::make_shared<lines_data>();
	std::ifstream stream(path);
	if (!stream.is_open())
		throw std::runtime_error("File cannot be read!");
	std::string line;
	do {
		std::getline(stream, line);
		lines->push_back(std::make_shared<std::string>(std::move(line)));
	} while (!stream.eof());
	
	// make map
	for (int i = 0; i < lines->size(); ++i) {
		std::istringstream iss(*(*lines)[i]);
		for (std::string word; iss >> word;)
			words[word].insert(i+1); // if set isnt created, creates it? idk
								// but working now
	}
}


TextQuery::~TextQuery()
{
}

QueryResult TextQuery::query(std::string str) const
{
	try
	{
		std::shared_ptr<std::set<uint>> line_numbers(
			new std::set<uint>(std::move(words.at(str))));
		return QueryResult(str, lines, line_numbers);
	}
	catch (...) {
		return QueryResult(str, lines, std::make_shared<std::set<uint>>());
	}
}

