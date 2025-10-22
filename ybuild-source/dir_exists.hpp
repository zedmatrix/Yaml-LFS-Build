#include <filesystem>

namespace fs = std::filesystem;

bool dir_exists(const std::filesystem::path& dir) {
    return fs::exists(dir) && fs::is_directory(dir);
}

fs::path ant_bin = "/opt/ant/bin";
fs::path jdk_bin = "/opt/jdk/bin";
fs::path rustc_bin = "/opt/rustc/bin";
fs::path kf6_bin = "/opt/kf6/bin";
fs::path qt6_bin = "/opt/qt6/bin";

std::string exec_path;

auto append_if_exists = [&](const fs::path& p) {
    if (dir_exists(p)) {
        if (!exec_path.empty())
            exec_path += ":";  // use ";" on Windows
            exec_path += p.string();
    }
};

append_if_exists(ant_bin);
append_if_exists(jdk_bin);
append_if_exists(rustc_bin);
append_if_exists(kf6_bin);
append_if_exists(qt6_bin);
