#pragma once
#include "Query_base.h"
//#include "Query.h"
#include <string>

class WordQuery : public Query_base {
	friend class Query;
private:
	explicit WordQuery(const std::string& str) : query_word(str){}
	virtual QueryResult eval(const TextQuery & tx) override {
		if (query_word.empty())
			return QueryResult(rep());
		return tx.query(query_word);
	};
	virtual std::string rep() const override {
		return query_word;
	};

	std::string query_word;
};
