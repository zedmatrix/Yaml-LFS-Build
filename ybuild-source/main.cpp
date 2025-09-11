#include "init.hpp"
#include "yaml-in.hpp"
#include "fs-functions.hpp"
#include "exec.hpp"
#include "download.hpp"
#include "source-functions.hpp"

bool extract() {
    m_archivedir = m_ysrc / fs::path(m_archive);
    command = std::format("tar -xf {} -C {} --strip-components={}",
                          m_archivedir.string(), m_build_dir.string(), m_pkgrel);
    printf("*** Executing: %s\n", command.c_str());
    return execute(command);
}
bool applyPatches() {
    if (!change_dir(m_build_dir)) return false;

    std::string url, md5, file;
    for (const auto& patch : m_config["patches"]) {
        url = patch["url"].as<std::string>();
        md5 = patch["md5"].as<std::string>();
        std::println("Applying Patch:{} URL:{} MD5:{}", getBaseName(url), url, md5);
        file = getBaseName(url);
        fs::path filePath = m_ysrc / file;
        command = std::format("patch -Np1 -i {}", filePath.string());
        printf("*** Executing: %s\n", command.c_str());
        if (!execute(command)) return false;
    }
    return true;
}

bool prepare() {
    if (m_prepare.empty()) return true;
    if (!change_dir(m_build_dir)) return false;
    printf("*** Prepare Executing: %s\n", m_prepare.c_str());
    return execute(m_prepare);
}

bool build() {
    if (m_build.empty()) return true;
    if (!change_dir(m_build_dir)) return false;
    printf("*** Build Executing: %s\n", m_build.c_str());
    return execute(m_build);
}
bool install() {
    if (m_install.empty()) return true;
    if (!change_dir(m_build_dir)) return false;

    printf("*** Install Executing: %s\n", m_install.c_str());
    return execute(m_install);
}
bool finalize() {
    if (m_final.empty()) return true;
    if (!change_dir(m_build_dir)) return false;

    printf("*** Finalize Executing: %s\n", m_final.c_str());
    return execute(m_final);
}

int main(int argc, char* argv[]/*, char* envp[]*/) {

    if (argc < 2) {
        printf("Usage: %s [package]\n", argv[0]);
        return 1;
    }

    fs::path filepath = argv[1];
    if (!std::filesystem::exists(filepath)) return 1;

    if (fs::exists(certFile)) {
        m_addCert = certFile.string();
    }

    // Get Package Data
    loadPackage(filepath);

    m_package_path = absolutePath(filepath);

    fs::current_path(m_package_path);

    m_build_dir = (m_root_path / "tmp" / m_pkgdir);
    createBuildDir();

    m_ysrc = (m_package_path / m_source_dir);

    std::cout << "*** Main root: " << m_root_path << std::endl;
    std::cout << "*** Main ysrc: " << m_ysrc << std::endl;
    std::cout << "*** Main Build: " << m_build_dir << std::endl;
    make_dir(m_ysrc);

    std::println("{}", stars());
    m_DEBUG = true;
    if (!getSources()) {
        printf("Error Downloading Sources\n");
        return 1;
    }

    if (!extract()) {
        printf("Error Extracting Sources\n");
        return 1;
    }
    if (!applyPatches()) {
        printf("Error Applying Patches\n");
        return 1;
    }

    std::println("{}", stars());
    change_dir(m_rootPath);
    if (!prepare()) {
        printf("Error Preparing Package\n");
        return 1;
    }

    change_dir(m_rootPath);
    if (!build()) {
        printf("Error Building Package\n");
        return 1;
    }

    change_dir(m_rootPath);
    if (!install()) {
        printf("Error Installing Package\n");
        return 1;
    }
    change_dir(m_rootPath);
    if (!finalize()) {
        printf("Error Finalizing Package\n");
        return 1;
    }

    std::println("{}", stars());
    change_dir(m_rootPath);

    m_DEBUG = false;
    command = "env";
    execute(command);

    std::println("{}", stars());
    return 0;
}
