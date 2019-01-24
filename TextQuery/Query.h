#pragma once
#include "Query_base.h"
#include "WordQuery.h"
#include <numeric>
#include <iterator>
#include <memory>
#include <string>
#include <iostream>
#include <algorithm>
#include <set>

using std::set;
using std::map;
using std::string;
using std::shared_ptr;


class Query : public Query_base {
	friend Query operator&(const Query&, const Query&);
	friend Query operator|(const Query&, const Query&);
	friend Query operator~(const Query&);
public:
	explicit Query(const std::string& str) : p(new WordQuery(str)) {}
	Query() = delete;
	Query(const Query& q) : p(q.p) {}

	virtual QueryResult eval(const TextQuery & tx) override {
		return p->eval(tx);
	};
	virtual std::string rep() const override {
		return p->rep();
	};


private:
	explicit Query(std::shared_ptr<Query_base> pointer) : p(pointer) {}
	std::shared_ptr<Query_base> p;
};


std::ostream& operator<<(std::ostream &os, const Query& q) {
	return os << q.rep();
}


class BinaryQuery_base : public Query_base {
	//virtual QueryResult eval(const TextQuery& tx) = 0;
protected:
	Query lhs, rhs;
	std::string op;

	BinaryQuery_base(const Query& q1, const Query& q2, std::string _op) :
		lhs(q1), rhs(q2), op(_op) {}
	std::string rep() const override {
		return "(" + lhs.rep() + " "
			+ op + " " + rhs.rep() + ")";
	}

};

class AndQuery : public BinaryQuery_base {
	friend Query operator&(const Query&, const Query&);
private:
	AndQuery(const Query& q1, const Query& q2) : BinaryQuery_base(q1, q2, "&") {}

	QueryResult eval(const TextQuery & tx) override {
		QueryResult left = lhs.eval(tx);
		QueryResult right = rhs.eval(tx);

		if (left.isEmpty() || right.isEmpty())
			return QueryResult(BinaryQuery_base::rep(), left.getData(), std::make_shared<set<uint>>());

		std::shared_ptr<std::set<unsigned int>> intersect = std::make_shared<std::set<unsigned int>>();

		std::set_intersection(left.getLines()->begin(), left.getLines()->end(),
			right.getLines()->begin(), right.getLines()->end(),
			std::inserter(*intersect, intersect->begin())
		);

		return QueryResult(BinaryQuery_base::rep(), left.getData(), intersect);
	};
};

class OrQuery : public BinaryQuery_base {
	friend Query operator|(const Query&, const Query&);
private:
	OrQuery(const Query& q1, const Query& q2) : BinaryQuery_base(q1, q2, "|") {}
	virtual QueryResult eval(const TextQuery & tx) override {
		QueryResult left = lhs.eval(tx);
		QueryResult right = rhs.eval(tx);
		if (left.isEmpty())
			return QueryResult(rep(), right.getData(), right.getLines());
		if (right.isEmpty())
			return QueryResult(rep(), left.getData(), left.getLines());
		shared_ptr<set<unsigned int>> union_ = std::make_shared<set<unsigned int>>();

		std::set_union(left.getLines()->begin(), left.getLines()->end(),
			right.getLines()->begin(), right.getLines()->end(),
			std::inserter(*union_, union_->begin()));
		return QueryResult(BinaryQuery_base::rep(), left.getData(), union_);
	};
};

class NotQuery : public Query_base {
	friend Query operator~(const Query&);
private:
	NotQuery(const Query& _q) : q(_q){}
	virtual QueryResult eval(const TextQuery&tx) override {
		QueryResult result = q.eval(tx);
		shared_ptr<set<uint>> set_computed = std::make_shared<set<uint>>();
		auto set_lines = result.getLines();
		auto max_size = result.getData()->size();

		for (size_t i = 1; i <= max_size; ++i)
			set_computed->insert(i);
		if (result.isEmpty()) {
			return QueryResult(rep(), result.getData(), set_computed);
		}

		///
		//std::iota(set_computed->begin(), set_computed->end(), 1);
		///
		for (auto i : *set_lines)
			set_computed->erase(i);
		return QueryResult(rep(), result.getData(), set_computed);
	}

	virtual std::string rep() const override {
		return "~(" + q.rep() + ')';
	}

	Query q;
};

inline Query operator&(const Query& q1, const Query& q2) {
	return Query(std::shared_ptr<Query_base>(new AndQuery(q1, q2)));
}
inline Query operator|(const Query& q1, const Query& q2) {
	return Query(std::shared_ptr<Query_base>(new OrQuery(q1, q2)));
}
inline Query operator~(const Query& q1) {
	return Query(std::shared_ptr<Query_base>(new NotQuery(q1)));
}
