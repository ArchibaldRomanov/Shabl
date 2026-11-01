#pragma once

#include <string>
#include <algorithm>   // std::reverse
#include <iostream>

// Базовый компонент — простой текст
class Text {
public:
    virtual ~Text() = default;
    virtual std::string render(const std::string& data) const {
        return data;
    }
};

// Базовый декоратор — хранит указатель на оборачиваемый компонент
class TextDecorator : public Text {
public:
    explicit TextDecorator(Text* component) : component_(component) {}
    ~TextDecorator() override { delete component_; }

    std::string render(const std::string& data) const override {
        return component_->render(data);
    }

protected:
    Text* component_;
};

// <p>...</p>
class Paragraph : public TextDecorator {
public:
    explicit Paragraph(Text* component) : TextDecorator(component) {}

    std::string render(const std::string& data) const override {
        return "<p>" + TextDecorator::render(data) + "</p>";
    }
};

// Разворот строки
class Reversed : public TextDecorator {
public:
    explicit Reversed(Text* component) : TextDecorator(component) {}

    std::string render(const std::string& data) const override {
        std::string reversed = TextDecorator::render(data);
        std::reverse(reversed.begin(), reversed.end());
        return reversed;
    }
};

// <a href=url>...</a>
// ВАЖНО: сигнатура render() отличается от оригинального метода
class Link : public TextDecorator {
public:
    explicit Link(Text* component) : TextDecorator(component) {}

    std::string render(const std::string& url,
                       const std::string& data) const {
        return "<a href=" + url + ">" + TextDecorator::render(data) + "</a>";
    }

    // Базовая версия — без url, чтобы не нарушать полиморфизм
    std::string render(const std::string& data) const override {
        return TextDecorator::render(data);
    }
};