#include <iostream>
#include <cassert>
#include "sql_query_builder.h"

int main() {
    {
        SqlSelectQueryBuilder qb;
        qb.AddColumn("name").AddColumn("phone");
        qb.AddFrom("students");
        qb.AddWhere("id", "42").AddWhere("name", "John");

        const std::string expected =
            "SELECT name, phone FROM students WHERE id=42 AND name=John;";
        assert(qb.BuildQuery() == expected);
        std::cout << "[Task 1] " << qb.BuildQuery() << "\n";
    }

    {
        SqlSelectQueryBuilder qb;
        qb.AddFrom("students");
        assert(qb.BuildQuery() == "SELECT * FROM students;");
        std::cout << "[Task 1, no columns] " << qb.BuildQuery() << "\n";
    }

    // перезаписывает таблицу
    {
        SqlSelectQueryBuilder qb;
        qb.AddColumn("id");
        qb.AddFrom("a").AddFrom("b");
        assert(qb.BuildQuery() == "SELECT id FROM b;");
        std::cout << "[Task 1, AddFrom overwrite] " << qb.BuildQuery() << "\n";
    }

    // Порядок вызовов не важен
    {
        SqlSelectQueryBuilder qb;
        qb.AddWhere("id", "42").AddFrom("students").AddColumn("name");
        assert(qb.BuildQuery() == "SELECT name FROM students WHERE id=42;");
        std::cout << "[Task 1, any order] " << qb.BuildQuery() << "\n";
    }

    std::cout << "\nTask 1 checks passed.\n";
    return 0;
}