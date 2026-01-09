#pragma once

#include <string>

namespace xnetwork {

enum class LanguageCode {
    EN,
    DE,
    ES,
    FR
};

class Greeter {
public:
    explicit Greeter(std::string name) : name_(std::move(name)) {}

    std::string greet(LanguageCode lang) const {
        switch (lang) {
            case LanguageCode::DE:
                return "Hallo " + name_ + "!";
            case LanguageCode::ES:
                return "¡Hola " + name_ + "!";
            case LanguageCode::FR:
                return "Bonjour " + name_ + "!";
            default:
                return "Hello " + name_ + "!";
        }
    }

private:
    std::string name_;
};

}  // namespace xnetwork