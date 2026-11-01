#pragma once

#include <map>
#include <string>
#include <vector>
#include <sstream>

class SqlSelectQueryBuilder {
public:
    SqlSelectQueryBuilder() = default;
    ~SqlSelectQueryBuilder() = default;

    SqlSelectQueryBuilder& AddColumn(const std::string& column) noexcept {
        columns_.push_back(column);
        return *this;
    }

    SqlSelectQueryBuilder& AddFrom(const std::string& table) noexcept {
        table_ = table;
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::string& key, const std::string& value) noexcept {
        where_.emplace_back(key, value);
        return *this;
    }


    // Добавить сразу несколько полей в SELECT
    SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& columns) noexcept {
        for (const auto& c : columns) {
            columns_.push_back(c);
        }
        return *this;
    }

    // Добавить сразу несколько условий WHERE
    SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept {
        for (const auto& [k, v] : kv) {
            where_.emplace_back(k, v);
        }
        return *this;
    }

    // Построить запрос
    std::string BuildQuery() const {
        std::ostringstream query;
        query << "SELECT ";

        if (columns_.empty()) {
            query << "*";
        }
        else {
            for (std::size_t i = 0; i < columns_.size(); ++i) {
                if (i) query << ", ";
                query << columns_[i];
            }
        }

        query << " FROM " << table_;

        if (!where_.empty()) {
            query << " WHERE ";
            for (std::size_t i = 0; i < where_.size(); ++i) {
                if (i) query << " AND ";
                query << where_[i].first << "=" << where_[i].second;
            }
        }

        query << ";";
        return query.str();
    }

private:
    std::vector<std::string> columns_;
    std::string table_;
    std::vector<std::pair<std::string, std::string>> where_;
};