#pragma once

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <algorithm>

// Интерфейс наблюдателя
class Observer {
public:
    virtual ~Observer() = default;
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

// Наблюдаемый класс
class Observable {
public:
    // Добавляем не владеющий указатель на наблюдателя
    void addObserver(Observer* observer) {
        observers_.push_back(observer);
    }

    // Удаление наблюдателя (нужно вызывать перед разрушением наблюдателя)
    void removeObserver(Observer* observer) {
        observers_.erase(
            std::remove(observers_.begin(), observers_.end(), observer),
            observers_.end());
    }

    void warning(const std::string& message) const {
        for (auto* obs : observers_) {
            obs->onWarning(message);
        }
    }

    void error(const std::string& message) const {
        for (auto* obs : observers_) {
            obs->onError(message);
        }
    }

    void fatalError(const std::string& message) const {
        for (auto* obs : observers_) {
            obs->onFatalError(message);
        }
    }

private:
    std::vector<Observer*> observers_;
};

//Иерархия конкретных наблюдателей

// Реагирует на предупреждения — печать в консоль
class WarningObserver : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << "[warning] " << message << std::endl;
    }
};

// Реагирует на ошибки — печать в файл
class ErrorObserver : public Observer {
public:
    explicit ErrorObserver(std::string path) : path_(std::move(path)) {}

    void onError(const std::string& message) override {
        std::ofstream out(path_, std::ios::app);
        if (!out) {
            throw std::runtime_error("Cannot open file: " + path_);
        }
        out << "[error] " << message << std::endl;
    }

private:
    std::string path_;
};

// Реагирует на фатальные ошибки — печать в консоль и в файл
class FatalErrorObserver : public Observer {
public:
    explicit FatalErrorObserver(std::string path) : path_(std::move(path)) {}

    void onFatalError(const std::string& message) override {
        std::cout << "[fatal] " << message << std::endl;
        std::ofstream out(path_, std::ios::app);
        if (!out) {
            throw std::runtime_error("Cannot open file: " + path_);
        }
        out << "[fatal] " << message << std::endl;
    }

private:
    std::string path_;
};