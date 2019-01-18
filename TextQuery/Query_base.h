#pragma once
#include "QueryResult.h"
#include "TextQuery.h"

#include <string>

class Query_base {
	friend class Query;
private:
	virtual QueryResult eval(const TextQuery& tx) = 0;
	virtual std::string rep() const = 0;
};