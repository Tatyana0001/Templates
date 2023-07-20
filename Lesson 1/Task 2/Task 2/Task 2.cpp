#include <iostream>
#include "sql_query_builder.h"

int main() {
    SqlSelectQueryBuilder query_builder;
    query_builder.AddColumn("name").AddColumn("phone");
    std::vector<std::string> c = { "email", "address" };
    query_builder.AddColumns(c);
    query_builder.AddFrom("students");
    query_builder.AddWhere("id", "42").AddWhere("name", "John");
    std::map<std::string, std::string> t = { { "phone","111111" }, { "email", "http.st" } };
    query_builder.AddWhere(t);
    std::cout << query_builder.BuildQuery() << "\n";
    return 0;
}