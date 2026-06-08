bool applyPatches() {
    if (!change_dir(m_build_dir)) {
        yprint::bad("Error Changing to Build Directory");
        return false;
    }
    if (yaml_iskey_valid(m_patches, "url")) return true;

    std::string url, file;
    for (const auto& patch : m_patches) {
        url = patch["url"].as<std::string>();
        file = getBaseName(url);
        std::println("Applying Patch:{} URL:{}", file, url);

        fs::path filePath = m_ysrc / file;
        command = std::format("patch -Np1 -i {}", filePath.string());
        yprint::out(std::format("Executing: {}", command.c_str()));

        if (!execute(command)) {
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

	ExecuteStatus execStatus;
    yprint::out(std::format("Prepare Executing: {}", m_prepare));
    execStatus = execCapture(m_prepare);

    std::filesystem::path logfile = m_log_dir / "package-prepare.log";
    std::ofstream out(logfile);
    for (auto& line : execStatus.output) {
        out << line << '\n';
    }
    if (execStatus.code != 0) {
        yprint::bad("Error Preparing Package");
        return false;
    }
    return true;
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
	if (!m_testsuite) return true;
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

    ExecuteStatus execStatus;

    yprint::out(std::format("Install Executing: {}", m_install));
    execStatus = execCapture(m_install);

    std::filesystem::path logfile = m_log_dir / "package-install.log";
    std::ofstream out(logfile);
    for (auto& line : execStatus.output) {
        out << line << '\n';
    }

    if (execStatus.code != 0) {
        yprint::bad("Error Installing Package");
        return false;
    }
    return true;
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
