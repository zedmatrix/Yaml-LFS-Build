#include <filesystem>

namespace fs = std::filesystem;
fs::path m_execPath, m_rootPath, m_currentpath;

bool file_exists(const std::filesystem::path& file) {
    return fs::exists(file) && fs::is_regular_file(file);
}

bool dir_exists(const std::filesystem::path& dir) {
    return fs::exists(dir) && fs::is_directory(dir);
}

void current_path() {
    m_currentpath = fs::current_path();
    yprint::out(std::format("Current path: {}", m_currentpath.string()));
}

fs::path absolutePath(const std::filesystem::path& file) {
    return fs::absolute(file).parent_path();
}

bool make_dir(const std::filesystem::path& dir) {
    if (!fs::create_directories(dir)) {
        yprint::good(std::format("Directory Exists: {}", dir.string()));
        return false;
    }
    yprint::out(std::format("Created Directory: {}", dir.string()));
    return true;
}
bool change_dir(const std::filesystem::path& dir) {
    if (dir.empty()) {
        current_path();
        return false;
    }
    try {
        fs::path target = fs::absolute(dir);

        if (!fs::exists(target)) {
            yprint::bad(std::format("*** Target directory does not exist: {}", target.string()));
            return false;
        }

        if (fs::current_path() != target) {
            yprint::out(std::format("Changing to: {}", target.string()));
            fs::current_path(target);
        }

        return true;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "*** Error changing directory: " << e.what() << "\n";
        return false;
    }
}

void createBuildDir() {
    if (!make_dir(m_build_dir)) {
        const std::uintmax_t n{fs::remove_all(m_build_dir)};
        yprint::out(std::format("remove_all(): {} files or directories", static_cast<uintmax_t>(n)));
        make_dir(m_build_dir);
    } else {
        make_dir(m_build_dir);
    }
}
