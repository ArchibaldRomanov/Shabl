#pragma once

#include <string>
#include <cstddef>
#include <iostream>

// Реальный «тяжёлый» объект (как на лекции)
class VeryHeavyDatabase {
public:
    virtual ~VeryHeavyDatabase() = default;

    virtual std::string GetData(const std::string& key) const {
        return "value";   // имитация: всегда возвращает "value"
    }
};

// Кэширующий прокси (как на лекции) — оставлен для примера
class CacheProxyDB : public VeryHeavyDatabase {
public:
    explicit CacheProxyDB(VeryHeavyDatabase* real_object)
        : real_object_(real_object) {}

    std::string GetData(const std::string& key) const override {
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            std::cout << "[cache hit] ";
            return it->second;
        }
        std::cout << "[cache miss] ";
        std::string value = real_object_->GetData(key);
        cache_[key] = value;
        return value;
    }

private:
    VeryHeavyDatabase* real_object_;
    mutable std::map<std::string, std::string> cache_;
};

// Задание 2: OneShotDB — возвращает данные из реального объекта
// не больше `shots` раз, после чего всегда возвращает "error".
class OneShotDB : public VeryHeavyDatabase {
public:
    explicit OneShotDB(VeryHeavyDatabase* real_object, std::size_t shots = 1)
        : real_object_(real_object), shots_left_(shots) {}

    std::string GetData(const std::string& key) const override {
        if (shots_left_ == 0) {
            return "error";
        }
        --shots_left_;
        return real_object_->GetData(key);
    }

private:
    VeryHeavyDatabase* real_object_;
    mutable std::size_t shots_left_;
};