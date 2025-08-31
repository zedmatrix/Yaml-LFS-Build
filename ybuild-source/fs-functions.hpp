#include <filesystem>

namespace fs = std::filesystem;
fs::path m_execPath, m_rootPath, m_currentpath;

void current_path() {
    m_currentpath = fs::current_path();
    printf("Current path: %s\n", m_currentpath.c_str());
}

fs::path absolutePath(const std::filesystem::path& file) {
    return fs::absolute(file).parent_path();
}

bool make_dir(const std::filesystem::path& dir) {
    if (!fs::create_directories(dir)) {
        printf("Directory Exists: %s\n", dir.c_str());
        return false;
    }
    printf("Created Directory: %s\n", dir.c_str());
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
            std::cout << "*** Target directory does not exist: " << target << "\n";
            return false;
        }

        if (fs::current_path() != target) {
            std::cout << "Changing to: " << target << "\n";
            fs::current_path(target);
        }

        return true;
    } catch (const fs::filesystem_error& e) {
        std::cout << "*** Error changing directory: " << e.what() << "\n";
        return false;
    }
}

void createBuildDir(const std::filesystem::path& path) {
    m_build_dir = (path / "tmp" / m_pkgdir);
    if (!make_dir(m_build_dir)) {
        const std::uintmax_t n{fs::remove_all(m_build_dir)};
        printf("remove_all(): %ju files or directories\n", static_cast<uintmax_t>(n));
        make_dir(m_build_dir);
    } else {
        make_dir(m_build_dir);
    }
}
