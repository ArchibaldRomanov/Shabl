#pragma once

#include <string>
#include <memory>
#include <fstream>
#include <iostream>
#include <stdexcept>

// Сообщение о логировании
class LogMessage {
public:
    enum class Type {
        Warning,
        Error,
        FatalError,
        Unknown
    };

    LogMessage(Type type, std::string message)
        : type_(type), message_(std::move(message)) {}

    Type type() const { return type_; }
    const std::string& message() const { return message_; }

private:
    Type type_;
    std::string message_;
};

// Базовый обработчик
class LogHandler {
public:
    explicit LogHandler(std::unique_ptr<LogHandler> next = nullptr)
        : next_(std::move(next)) {}

    virtual ~LogHandler() = default;

    // Обработать сообщение (или передать дальше)
    void handle(const LogMessage& msg) {
        if (handleMessage(msg)) {
            return;
        }
        if (!next_) {
            // Дошли до конца цепочки, но сообщение не обработано —
            // выбрасываем исключение о необработанном сообщении
            throw std::runtime_error(
                "Unhandled log message: " + msg.message());
        }
        next_->handle(msg);
    }

protected:
    // Возвращает true, если сообщение обработано
    virtual bool handleMessage(const LogMessage& msg) = 0;

private:
    std::unique_ptr<LogHandler> next_;
};

// Обработчик фатальных ошибок — выбрасывает исключение
class FatalErrorHandler : public LogHandler {
public:
    using LogHandler::LogHandler;

protected:
    bool handleMessage(const LogMessage& msg) override {
        if (msg.type() != LogMessage::Type::FatalError) {
            return false;
        }
        throw std::runtime_error("Fatal error: " + msg.message());
    }
};

// Обработчик обычных ошибок — пишет в файл
class ErrorHandler : public LogHandler {
public:
    explicit ErrorHandler(std::string path,
        std::unique_ptr<LogHandler> next = nullptr)
        : LogHandler(std::move(next)), path_(std::move(path)) {}

protected:
    bool handleMessage(const LogMessage& msg) override {
        if (msg.type() != LogMessage::Type::Error) {
            return false;
        }
        std::ofstream out(path_, std::ios::app);
        if (!out) {
            throw std::runtime_error("Cannot open file: " + path_);
        }
        out << "[error] " << msg.message() << std::endl;
        return true;
    }

private:
    std::string path_;
};

// Обработчик предупреждений — печать в консоль
class WarningHandler : public LogHandler {
public:
    using LogHandler::LogHandler;

protected:
    bool handleMessage(const LogMessage& msg) override {
        if (msg.type() != LogMessage::Type::Warning) {
            return false;
        }
        std::cout << "[warning] " << msg.message() << std::endl;
        return true;
    }
};

// Обработчик неизвестных сообщений — выбрасывает исключение
class UnknownHandler : public LogHandler {
public:
    using LogHandler::LogHandler;

protected:
    bool handleMessage(const LogMessage& msg) override {
        if (msg.type() != LogMessage::Type::Unknown) {
            return false;
        }
        throw std::runtime_error(
            "Unknown log message type: " + msg.message());
    }
};