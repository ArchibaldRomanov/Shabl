#include <iostream>
#include <cassert>
#include <map>

#include "text_render.h"
#include "very_heavy_database.h"

int main() {
    // Декоратор
    {
        auto* text_block = new Paragraph(new Text());
        std::string result = text_block->render("Hello world");
        assert(result == "<p>Hello world</p>");
        std::cout << "[Paragraph] " << result << "\n";
        delete text_block;
    }

    {
        auto* text_block = new Reversed(new Text());
        std::string result = text_block->render("Hello world");
        assert(result == "dlrow olleH");
        std::cout << "[Reversed]  " << result << "\n";
        delete text_block;
    }

    {
        auto* text_block = new Link(new Text());
        // Внимание: у Link своя сигнатура — (url, data)
        std::string result = text_block->render("netology.ru", "Hello world");
        assert(result == "<a href=netology.ru>Hello world</a>");
        std::cout << "[Link]      " << result << "\n";
        delete text_block;
    }

    // Комбинирование декораторов: <p><a href=...>...</a></p>
    {
        auto* text_block = new Paragraph(new Link(new Text()));
        std::string result = text_block->render("netology.ru", "Hello world");
        std::cout << "[Paragraph+Link] " << result << "\n";
        delete text_block;
    }

    // Комбинирование: <a href=...>dlrow olleH</a>
    {
        auto* text_block = new Link(new Reversed(new Text()));
        std::string result = text_block->render("netology.ru", "Hello world");
        std::cout << "[Link+Reversed]  " << result << "\n";
        delete text_block;
    }

    // Задание 2
    // OneShotDB
    {
        auto real_db = VeryHeavyDatabase();
        auto limit_db = OneShotDB(std::addressof(real_db), 2);

        std::string r1 = limit_db.GetData("key");
        std::string r2 = limit_db.GetData("key");
        std::string r3 = limit_db.GetData("key");

        assert(r1 == "value");
        assert(r2 == "value");
        assert(r3 == "error");

        std::cout << "\n[OneShotDB] " << r1 << "\n"
            << "[OneShotDB] " << r2 << "\n"
            << "[OneShotDB] " << r3 << "\n";
    }

    // Проверка значения по умолчанию (shots = 1)
    {
        VeryHeavyDatabase real_db;
        OneShotDB limit_db(std::addressof(real_db));   // по умолчанию 1

        assert(limit_db.GetData("k") == "value");
        assert(limit_db.GetData("k") == "error");
        std::cout << "[OneShotDB default] value, error\n";
    }

    std::cout << "\nAll checks passed.\n";
    return 0;
}