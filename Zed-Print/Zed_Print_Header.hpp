#ifndef ZED_PRINT_HEADER_HPP
#define ZED_PRINT_HEADER_HPP

#include <sys/ioctl.h>
#include <unistd.h>
#include <print>
#include <string>
#include <format>

namespace Color {
	enum class Code : uint8_t {
		BLACK = 30,
		RED = 31,
		GREEN = 32,
		YELLOW = 33,
		BLUE = 34,
		MAGENTA = 35,
		CYAN = 36,
		WHITE = 37,
		DEFAULT = 0
	};
}

inline std::string reset() { return "\033[0m"; }
inline std::string pos(uint16_t x) { return std::format("\033[{}G", x); }

template<typename T>
static std::string color(bool bold, T code) {
	static_assert(std::is_enum_v<T> || std::is_integral_v<T>, "color() requires an enum or integral type");
	uint8_t c = static_cast<uint8_t>(code);
	return "\033[" + std::string(bold ? "1;" : "") + std::to_string(c) + "m";
}
struct Label {
	std::string text;
	Color::Code color;
	Color::Code bracket = Color::Code::MAGENTA;
	uint16_t xpos = 1;
};

template<>
struct std::formatter<Label, char> : std::formatter<std::string, char> {
    using std::formatter<std::string, char>::parse;

    template<typename FormatContext>
    auto format(const Label& l, FormatContext& ctx) const {
        auto colored_text = std::format("{}{}", color(true, l.color), l.text);
        auto enclosed = std::format("{}{}[{}{}]{}",
            pos(l.xpos),
            color(true, l.bracket),
            colored_text,
            color(true, l.bracket),
            reset()
        );
        return std::formatter<std::string, char>::format(enclosed, ctx);
    }
};

template<typename... Args>
void yprintln(std::format_string<Args...> fmt, Args&&... args) {
    std::string text = std::format(fmt, std::forward<Args>(args)...);
    std::println("{}", Label{text, Color::Code::YELLOW});
}

class Zed_Print {

public:
	Zed_Print()
		: SCREEN_WIDTH(get_screen_width())
		, SCREEN_HEIGHT(get_screen_height())
	{}

	~Zed_Print() = default;

	uint16_t get_width() const { return SCREEN_WIDTH; }
	uint16_t get_height() const { return SCREEN_HEIGHT; }

	std::string clear(int x = 0) const { return std::format("{}[{}J", ESCAPE, x); }
	void clear_screen() const { std::print("{}", clear(2)); }

	std::string center() const { return std::format("{}[{};{}H", ESCAPE, MIDDLE_HEIGHT, MIDDLE_WIDTH); }
	std::string locate(uint16_t y, uint16_t x) const {
		return std::format("{}[{};{}H", ESCAPE,
			validate(y, SCREEN_HEIGHT),
			validate(x, SCREEN_WIDTH)
		);
	}
	std::string pos_x(uint16_t x) { return std::format("{}[{}G", ESCAPE, validate(x, SCREEN_WIDTH)); }

	template<typename T>
	void print(const T& value) {
		std::println("{}", value);
	}

	void warn(std::string text = "") {
		uint16_t x = (SCREEN_WIDTH - 9);
		std::println("{}{}", text, Label{m_warn, Color::Code::YELLOW, Color::Code::YELLOW, x});
	}
	void fail(std::string text = "") {
		uint16_t x = (SCREEN_WIDTH - 6);
		std::println("{}{}", text, Label{m_fail, Color::Code::RED, Color::Code::RED, x});
	}
	void pass(std::string text = "") {
		uint16_t x = (SCREEN_WIDTH - 6);
		std::println("{}{}", text, Label{m_pass, Color::Code::GREEN, Color::Code::GREEN, x});
	}
private:

	static uint16_t validate(int num, int max) {
		if (num > max) return max;
		if (num < 1) return 1;
		return num;
	}

	static uint16_t get_screen_width() {
		struct winsize w;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
		return w.ws_col;
	}

	static uint16_t get_screen_height() {
		struct winsize w;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
		return w.ws_row;
	}

	const uint16_t SCREEN_WIDTH;
	const uint16_t SCREEN_HEIGHT;
	const uint16_t MIDDLE_WIDTH = SCREEN_WIDTH / 2;
	const uint16_t MIDDLE_HEIGHT = SCREEN_HEIGHT / 2;

	static constexpr char ESCAPE = '\033';

    const std::string m_warn = "WARNING";
    const std::string m_fail = "FAIL";
    const std::string m_pass = "PASS";

};

#endif /*ZED_PRINT_HEADER_HPP*/
