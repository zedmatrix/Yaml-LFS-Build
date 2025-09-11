#include <iostream>
#include <cstdlib>
#include <string>
#include <filesystem>
#include <print>
#include <string>
#include <filesystem>
#include <yaml-cpp/yaml.h>

namespace fs = std::filesystem;
YAML::Node m_config, m_package;
int m_pkgrel;
bool m_zarchive, m_DEBUG;

std::string m_pkgdir, m_pkgname, m_pkgver, m_archive;
std::string m_prepare, m_build, m_check, m_install, m_final;
std::string m_pkgurl, m_pkgmd5, m_addCert;

fs::path m_build_dir, m_package_path, m_archivedir, m_ysrc;
fs::path m_source_dir {"sources"};

// Test Environment Variables
inline const fs::path m_lfs = [] {
    if (const char* env_p = std::getenv("LFS")) {
        return fs::path{env_p};
    }
    return fs::path{"/mnt/lfs"};
}();

inline const std::string m_cflags = [] {
    if (const char* env_p = std::getenv("CFLAGS")) {
        return env_p;
    }
    return "-Os -pipe -fPIC";
}();

fs::path command;
fs::path m_exec_path = fs::read_symlink("/proc/self/exe");
fs::path m_root_path = m_exec_path.parent_path();
fs::path certFile = (m_root_path / "ca-bundle.crt");    //inside chroot if no packages

bool lfs = static_cast<std::string>(m_root_path).starts_with(m_lfs.string());

std::string stars() {
    return std::string(30, '*');
}

std::string getBaseName(const std::string& str) {
    auto last_slash = str.find_last_of('/');
    if (last_slash != std::string::npos) {
        return str.substr(last_slash + 1);
    }
    return "";
}
