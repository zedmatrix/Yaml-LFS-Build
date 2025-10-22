#include <print>
#include <string>

namespace yprint {

    inline const std::string color_red = "\033[1;31m";
    inline const std::string color_green = "\033[1;32m";
    inline const std::string color_yellow = "\033[1;33m";
    inline const std::string color_white = "\033[1;37m";
    inline const std::string color_reset = "\033[0m";

    void out(const std::string& str) {
        std::println("{}{}{}", color_white, str, color_reset);
    }

    void warn(const std::string& str) {
        std::println("{}{}{}", color_yellow, str, color_reset);
    }

    void good(const std::string& str) {
        std::println("{}{}{}", color_green, str, color_reset);
    }

    void bad(const std::string& str) {
        std::println("{}{}{}", color_red, str, color_reset);
    }

    void missing(const std::string& str) {
        std::println("{}Package:{} Missing{}", color_red, str, color_reset);
    }

    void found(const std::string& str) {
        std::println("{}Package:{} Found{}", color_green, str, color_reset);
    }

}
