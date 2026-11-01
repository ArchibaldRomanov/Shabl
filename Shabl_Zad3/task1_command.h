#pragma once

#include <string>
#include <fstream>
#include <iostream>

// Базовый класс команды
class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

// Команда: печать в консоль
class ConsoleLogCommand : public LogCommand {
public:
    void print(const std::string& message) override {
        std::cout << message << std::endl;
    }
};

// Команда: печать в файл
class FileLogCommand : public LogCommand {
public:
    explicit FileLogCommand(std::string path) : path_(std::move(path)) {}

    void print(const std::string& message) override {
        std::ofstream out(path_, std::ios::app);
        if (!out) {
            throw std::runtime_error("Cannot open file: " + path_);
        }
        out << message << std::endl;
    }

private:
    std::string path_;
};

// Функция, выполняющая команду
inline void print(LogCommand& cmd, const std::string& message) {
    cmd.print(message);
}