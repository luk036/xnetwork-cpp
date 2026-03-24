#pragma once

#include <string>

namespace xnetwork {

/**
 * @brief Language codes supported for greetings
 * 
 * Specifies the available language options for generating
 * localized greeting messages.
 */
enum class LanguageCode {
    EN,  ///< English
    DE,  ///< German
    ES,  ///< Spanish
    FR   ///< French
};

/**
 * @brief A class that generates localized greeting messages
 * 
 * This class provides functionality to generate greeting messages
 * in multiple languages based on the provided language code.
 */
class Greeter {
public:
    /**
     * @brief Construct a new Greeter object
     * 
     * @param name The name to include in the greeting message
     */
    explicit Greeter(std::string name) : name_(std::move(name)) {}

    /**
     * @brief Generate a greeting message in the specified language
     * 
     * @param lang The language code for the greeting
     * @return std::string A localized greeting message containing the name
     */
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
    std::string name_;  ///< The name to include in greeting messages
};

}  // namespace xnetwork