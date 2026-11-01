#include <iostream>
#include <cassert>
#include <cstdio>    // std::remove

#include "task1_command.h"
#include "task2_observer.h"
#include "task3_chain.h"

int main() {
    // Задание 1
    // Команда 
    std::cout << "Task 1: Command \n";
    {
        const std::string path = "task1_log.txt";
        std::remove(path.c_str());   // чистим файл перед тестом

        ConsoleLogCommand console_cmd;
        FileLogCommand file_cmd(path);

        print(console_cmd, "hello console");
        print(file_cmd, "hello file");
        print(file_cmd, "second line");

        // Проверим содержимое файла
        std::ifstream in(path);
        std::string line1, line2;
        std::getline(in, line1);
        std::getline(in, line2);
        assert(line1 == "hello file");
        assert(line2 == "second line");
        std::cout << "File content OK\n";
    }

    // Задание 2
    // Наблюдатель 
    std::cout << "\n Task 2: Observer\n";
    {
        const std::string path = "task2_log.txt";
        std::remove(path.c_str());

        Observable observable;

        WarningObserver warning_obs;
        ErrorObserver error_obs(path);
        FatalErrorObserver fatal_obs(path);

        observable.addObserver(&warning_obs);
        observable.addObserver(&error_obs);
        observable.addObserver(&fatal_obs);

        observable.warning("low disk space");
        observable.error("file not found");
        observable.fatalError("segmentation fault");

        // Корректное удаление наблюдателя
        observable.removeObserver(&warning_obs);
        observable.warning("this should not be printed by warning_obs");

        // Проверим файл
        std::ifstream in(path);
        std::string line;
        std::getline(in, line);
        assert(line == "[error] file not found");
        std::getline(in, line);
        assert(line == "[fatal] segmentation fault");
        std::cout << "Observer file content OK\n";
    }

    // Задание 3
    // Цепочка ответственности
    std::cout << "\n Task 3: Chain of Responsibility\n";
    {
        const std::string path = "task3_log.txt";
        std::remove(path.c_str());

        // Порядок: fatal -> error -> warning -> unknown
        auto chain = std::make_unique<FatalErrorHandler>(
            std::make_unique<ErrorHandler>(
                path,
                std::make_unique<WarningHandler>(
                    std::make_unique<UnknownHandler>())));

        // Предупреждение — печатается в консоль
        chain->handle(LogMessage(LogMessage::Type::Warning, "test warning"));

        // Ошибка — пишется в файл
        chain->handle(LogMessage(LogMessage::Type::Error, "test error"));

        // Фатальная ошибка — исключение
        try {
            chain->handle(LogMessage(LogMessage::Type::FatalError, "boom"));
            assert(false && "must not reach");
        }
        catch (const std::exception& e) {
            std::cout << "Caught: " << e.what() << "\n";
        }

        // Неизвестное — исключение
        try {
            chain->handle(LogMessage(LogMessage::Type::Unknown, "???"));
            assert(false && "must not reach");
        }
        catch (const std::exception& e) {
            std::cout << "Caught: " << e.what() << "\n";
        }

        // Проверим файл
        std::ifstream in(path);
        std::string line;
        std::getline(in, line);
        assert(line == "[error] test error");
        std::cout << "Chain file content OK\n";
    }

    std::cout << "\nAll tasks passed.\n";
    return 0;
}