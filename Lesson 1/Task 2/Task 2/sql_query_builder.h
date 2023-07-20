#pragma once
#include <vector>
#include <string>
#include <map>
#include <algorithm>

struct QueryBuilder {
	std::vector <std::string> columns;
	std::vector <std::string> tables;
	std::map <std::string, std::string> selection;
};

class SqlSelectQueryBuilder {
public:
	SqlSelectQueryBuilder() = default;
	SqlSelectQueryBuilder& AddColumn(const std::string& column_name) noexcept {
		querybuilder.columns.push_back(column_name);
		return *this;
	}
	SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& cols) noexcept {
		for (const auto& el : cols) {
			querybuilder.columns.push_back(el);
		}
		return *this;
	}
	SqlSelectQueryBuilder& AddFrom(const std::string& table_name) noexcept {
		querybuilder.tables.push_back(table_name);
		return *this;
	}
	SqlSelectQueryBuilder& AddWhere(const std::string& column, const std::string& value) noexcept {
		querybuilder.selection[column] = value;
		return *this;
	}
	SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept {
		for (const auto& el : kv) {
			querybuilder.selection[el.first] = el.second;
		}
		return *this;
	}

	std::string BuildQuery() {
		std::string res = "SELECT ";
		if (querybuilder.columns.empty()) {
			res += "*";
		}
		for (int i = 0; i < querybuilder.columns.size(); i++) {
			res += querybuilder.columns[i];
			if (i < querybuilder.columns.size() - 1) {
				res += ", ";
			}
		}
		if (!querybuilder.tables.empty()) {
			res += " FROM ";
			for (int i = 0; i < querybuilder.tables.size(); i++) {
				res += querybuilder.tables[i];
				if (i < querybuilder.tables.size() - 1) {
					res += ", ";
				}
			}
		}
		if (!querybuilder.selection.empty()) {
			res += " WHERE ";
			for (const auto& elem : querybuilder.selection) {
				res += elem.first + "=" + elem.second + " AND ";
			}
			res = res.substr(0, res.size() - 5);
		}
		return res + ";";
	}
private:
	QueryBuilder querybuilder;
};