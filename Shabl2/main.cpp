#include <iostream>
#include <cassert>
#include <map>
#include <vector>
#include "sql_query_builder.h"

int main() {
    {
        SqlSelectQueryBuilder qb;
        qb.AddColumn("name").AddColumn("phone");
        qb.AddFrom("students");
        qb.AddWhere("id", "42").AddWhere("name", "John");

        assert(qb.BuildQuery() ==
            "SELECT name, phone FROM students WHERE id=42 AND name=John;");
        std::cout << "[Task 1] " << qb.BuildQuery() << "\n";
    }

    // несколько полей
    {
        SqlSelectQueryBuilder qb;
        qb.AddColumns({ "name", "phone" });
        qb.AddFrom("students");

        assert(qb.BuildQuery() == "SELECT name, phone FROM students;");
        std::cout << "[Task 2, AddColumns] " << qb.BuildQuery() << "\n";
    }

    // несколько условий
    {
        SqlSelectQueryBuilder qb;
        qb.AddColumns({ "name", "phone" });
        qb.AddFrom("students");
        qb.AddWhere({ {"id", "42"}, {"name", "John"} });

        // std::map сортирует ключи: id < name → id=42 AND name=John
        assert(qb.BuildQuery() ==
            "SELECT name, phone FROM students WHERE id=42 AND name=John;");
        std::cout << "[Task 2, AddWhere(map)] " << qb.BuildQuery() << "\n";
    }

    // Комбинация одиночных и множественных вызовов
    {
        SqlSelectQueryBuilder qb;
        qb.AddColumn("id").AddColumns({ "name", "age" });
        qb.AddFrom("users");
        qb.AddWhere("active", "1").AddWhere({ {"role", "admin"} });

        assert(qb.BuildQuery() ==
            "SELECT id, name, age FROM users WHERE active=1 AND role=admin;");
        std::cout << "[Task 2, mixed] " << qb.BuildQuery() << "\n";
    }

    // SELECT *
    {
        SqlSelectQueryBuilder qb;
        qb.AddFrom("students");
        qb.AddWhere({ {"id", "42"} });

        assert(qb.BuildQuery() == "SELECT * FROM students WHERE id=42;");
        std::cout << "[Task 2, no columns] " << qb.BuildQuery() << "\n";
    }

    std::cout << "\nTasks 1 & 2 checks passed.\n";
    return 0;
}