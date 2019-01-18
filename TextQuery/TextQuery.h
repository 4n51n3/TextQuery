#pragma once
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <set>
#include "QueryResult.h"


class TextQuery
{
	typedef unsigned int uint;
	typedef std::vector<std::shared_ptr<std::string>> lines_data;

	std::shared_ptr<lines_data> lines;
	std::map<std::string, std::set<uint>> words;
public:
	explicit TextQuery(std::string path);
	~TextQuery();
	QueryResult query(std::string str) const;
};

