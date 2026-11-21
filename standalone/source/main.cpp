#include <xnetwork/greeter.h>
#include <xnetwork/version.h>

// cxxopts < 3.3 uses uint8_t/intmax_t without including <cstdint>, which breaks with newer
// libstdc++; include it first so a system-provided cxxopts keeps working too.
#include <cstdint>
#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

auto main(int argc, char** argv) -> int {
    const std::unordered_map<std::string, xnetwork::LanguageCode> languages{
        {"en", xnetwork::LanguageCode::EN},
        {"de", xnetwork::LanguageCode::DE},
        {"es", xnetwork::LanguageCode::ES},
        {"fr", xnetwork::LanguageCode::FR},
    };

    cxxopts::Options options(*argv, "A program to welcome the world!");

    std::string language;
    std::string name;

    // clang-format off
  options.add_options()
    ("h,help", "Show help")
    ("v,version", "Print the current version number")
    ("n,name", "Name to greet", cxxopts::value(name)->default_value("World"))
    ("l,lang", "Language code to use", cxxopts::value(language)->default_value("en"))
  ;
    // clang-format on

    auto result = options.parse(argc, argv);

    if (result["help"].as<bool>()) {
        std::cout << options.help() << '\n';
        return 0;
    }

    if (result["version"].as<bool>()) {
        std::cout << "XNetwork, version " << XNETWORK_VERSION << '\n';
        return 0;
    }

    auto langIt = languages.find(language);
    if (langIt == languages.end()) {
        std::cerr << "unknown language code: " << language << '\n';
        return 1;
    }

    return 0;
}
