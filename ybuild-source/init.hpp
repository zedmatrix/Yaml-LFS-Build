#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <print>
#include <string>
#include <filesystem>
#include <yaml-cpp/yaml.h>
#include <thread>
#include <chrono>

namespace fs = std::filesystem;
YAML::Node m_config, m_package;
YAML::Node m_sources, m_patches;
YAML::Node DATABASE;
int m_pkgrel = 1;
int m_filetype = 0;

bool m_zarchive = false;
bool m_delete = false;
bool m_DEBUG = false;
bool m_gitrepo = false;
bool lfs = false;
bool m_patch_bool = false;
bool m_source_bool = false;
bool m_testsuite;
bool m_destdir_bool = false;

std::string m_pkgdir, m_pkgname, m_pkgver, m_archive, m_pkgdesc;
std::string m_prepare, m_build, m_check, m_install, m_final;
std::string m_pkgurl, m_pkgsha256, m_pkgmd5, m_addCert, m_pkg_cflags;
std::string m_extract_size, m_final_size, m_error_text, m_xml_file;

fs::path m_build_dir, m_log_dir, m_package_path, m_archivedir, m_ysrc, m_xml_dir, m_destdir;

fs::path ant_bin = "/opt/ant/bin";
fs::path jdk_bin = "/opt/jdk/bin";
fs::path rustc_bin = "/opt/rustc/bin";
fs::path kf6_bin = "/opt/kf6/bin";
fs::path qt6_bin = "/opt/qt6/bin";

fs::path tar_bin   = "/usr/bin/tar";
fs::path unzip_bin = "/usr/bin/bsdunzip";

// Test Environment Variables
inline const fs::path m_lfs = [] {
    if (const char* env_p = std::getenv("LFS")) {
    	lfs = true;
        return fs::path{env_p};
    }
    return fs::path{""};
}();
/*
inline const bool m_testsuite = [] {
    if (const char* env_p = std::getenv("YCHECK")) {
        return std::string(env_p) == "YES";
    }
    return false;
}();
*/
//x86_64-ybuild-linux-gnu
inline const std::string m_lfstgt = [] {
    if (const char* env_p = std::getenv("LFS_TGT")) {
        return env_p;
    }
    return "";
}();
//i686-ybuild-linux-gnu
inline const std::string m_lfstgt32 = [] {
    if (const char* env_p = std::getenv("LFS_TGT32")) {
        return env_p;
    }
    return "";
}();

//CFLAGS="-O2 -g -fPIC";
inline const std::string m_cflags = [] {
    if (const char* env_p = std::getenv("CFLAGS")) {
        return env_p;
    }
    return "";
}();
//LDFLAGS=""
inline const std::string m_ldflags = [] {
    if (const char* env_p = std::getenv("LDFLAGS")) {
        return env_p;
    }
    return "";
}();
//LC_ALL=C.UTF-8;
inline const std::string m_locale = [] {
    if (/*const char* env_p = */std::getenv("LANG")) {
        return "C.UTF-8";   //env_p;
    }
    return "POSIX";
}();
//XML_PRINT
inline const std::string XML_PRINT = [] {
	const char* val = std::getenv("XML_PRINT");
	if (val && std::strcmp(val, "FILE") == 0) {
		return "FILE";
	}
	return "SCREEN";
}();

fs::path command;
fs::path m_exec_path = fs::read_symlink("/proc/self/exe");
fs::path m_root_path = m_exec_path.parent_path();
fs::path FILEPATH;
fs::path m_dbfile = m_root_path / "ydatabase.yaml";
fs::path certFile = (m_root_path / "ca-bundle.crt");    //inside chroot if no packages

// bool lfs = static_cast<std::string>(m_root_path).starts_with(m_lfs.string());

std::string getBaseName(const std::string& str) {
    auto last_slash = str.find_last_of('/');
    if (last_slash != std::string::npos) {
        return str.substr(last_slash + 1);
    }
    return "";
}

void time_delay(int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}
