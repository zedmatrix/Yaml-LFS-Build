#pragma once
#include <string>
#include <format>
#include <chrono>
#include <cmath>
/*
 * Usage create an instance  : TimeManager tm;
 *       to set a start time : tm.start();
 *        to stop the timer  : tm.stop();
 *
 *         example baseline  : used 70s my biniutils timed {configure..make}
 *         to print sbu time : std::println("{}", tm.sbu_str(double baseline_seconds));
 *
 *      to print simple time : std::println("{}", tm.hms());
 *  to return seconds lapsed : double seconds = tm.seconds();
 *
 */

class TimeManager {
public:
    using clock = std::chrono::steady_clock;

    void start() { m_start = clock::now(); }
    void stop()  { m_stop  = clock::now(); }

    double seconds() const {
        return std::chrono::duration<double>(m_stop - m_start).count();
    }

    std::string hms() const {
        return to_hms(seconds());
    }

    // SBU calculator
    double sbu(double baseline_seconds) const {
        if (baseline_seconds <= 0.0)
            return 0.0;
        return seconds() / baseline_seconds;
    }

    std::string sbu_str(double baseline_seconds) const {
        double s = sbu(baseline_seconds);
        if (s < 0.5) return std::string("less than 0.5 SBU");
        if (s < 1.0) return std::string("less than 1.0 SBU");
        return std::format("{:.2f} SBU", s);
    }

private:
    clock::time_point m_start{};
    clock::time_point m_stop{};

    // helper for H/M/S output
    static std::string to_hms(double sec) {
        int t = static_cast<int>(sec);
        int h = t / 3600;
        int m = (t / 60) % 60;
        int s = t % 60;

        std::string out;
        if (h) out += std::format("{}h ", h);
        if (m) out += std::format("{}m ", m);
        out += std::format("{}s", s);

        return out;
    }
};
