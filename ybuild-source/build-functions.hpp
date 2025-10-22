bool applyPatches() {
    if (!change_dir(m_build_dir)) {
        yprint::bad("Error Changing to Build Directory");
        return false;
    }

    std::string url, md5, file;
    for (const auto& patch : m_config["patches"]) {
        url = patch["url"].as<std::string>();
        md5 = patch["md5"].as<std::string>();
        std::println("Applying Patch:{} URL:{} MD5:{}", getBaseName(url), url, md5);
        file = getBaseName(url);
        fs::path filePath = m_ysrc / file;
        command = std::format("patch -Np1 -i {}", filePath.string());

        yprint::out(std::format("Executing: {}", command.c_str()));
        bool ret = execute(command);
        if (!ret) {
            yprint::bad(std::format("Error Applying Patch: {}", file));
            return false;
        }
    }
    return true;
}

bool prepare() {
    if (m_prepare.empty()) return true;
    change_dir(m_rootPath);
    if (!change_dir(m_build_dir)) {
        yprint::bad("Error Changing to Build Directory");
        return false;
    }

    yprint::out(std::format("Prepare Executing: {}", m_prepare));
    bool ret = execute(m_prepare);
    if (!ret) yprint::bad("Error Preparing Package");
    return ret;
}

bool build() {
    if (m_build.empty()) return true;
    change_dir(m_rootPath);
    if (!change_dir(m_build_dir)) {
        yprint::bad("Error Changing to Build Directory");
        return false;
    }

    yprint::out(std::format("Build Executing: {}", m_build));
    bool ret = execute(m_build);
    if (!ret) yprint::bad("Error Building Package\n");
    return ret;
}

bool check() {
    if (m_check.empty()) return true;
    change_dir(m_rootPath);
    if (!change_dir(m_build_dir)) {
        yprint::bad("Error Changing to Build Directory");
        return false;
    }

    yprint::out(std::format("Check Executing: {}", m_check));
    bool ret = execute(m_check);
    if (!ret) yprint::bad("Error Checking Package");
    return ret;
}

bool install() {
    if (m_install.empty()) return true;
    change_dir(m_rootPath);
    if (!change_dir(m_build_dir)) {
        yprint::bad("Error Changing to Build Directory");
        return false;
    }

    yprint::out(std::format("Install Executing: {}", m_install));
    bool ret = execute(m_install);
    if (!ret) yprint::bad("Error Installing Package");
    return ret;
}
bool finalize() {
    if (m_final.empty()) return true;
    change_dir(m_rootPath);
    if (!change_dir(m_build_dir)) {
        yprint::bad("Error Changing to Build Directory");
        return false;
    }

    yprint::out(std::format("Finalize Executing: {}", m_final));
    bool ret = execute(m_final);
    if (!ret) yprint::bad("Error Finalizing Package\n");
    return ret;
}
