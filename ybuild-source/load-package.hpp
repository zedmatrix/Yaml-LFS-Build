std::string checkNode(const YAML::Node& config, const std::string& node) {
    if (!config[node]) {
        return {};
    }
    return config[node].as<std::string>();
}

void loadPackage(std::filesystem::path& package) {
    m_config = YAML::LoadFile(package);

    m_prepare = checkNode(m_config, "prepare");
    m_build = checkNode(m_config, "build");
    m_check = checkNode(m_config, "check");
    m_install = checkNode(m_config, "install");
    m_final = checkNode(m_config, "final");

    m_package = m_config["package"];

    m_pkgname = checkNode(m_package, "name");
    m_pkgver = checkNode(m_package, "version");
    m_pkgrel = m_package["rel"].as<int>(1);
    m_zarchive = m_package["archive"].as<bool>(false);
    m_delete = m_package["delete"].as<bool>(false);

    m_pkgdir = std::format("{}-{}", m_pkgname, m_pkgver);
    // Test Print Out
    std::println("Package Directory: {}", m_pkgdir);
    std::println("Package Name: {} Version: {}", m_pkgname, m_pkgver);

    //TODO Test Printouts
    if (!m_prepare.empty()) std::println("Prepare: {}", m_prepare);
    if (!m_build.empty()) std::println("Build: {}", m_build);
    if (!m_check.empty()) std::println("Check: {}", m_check);
    if (!m_install.empty()) std::println("Install: {}", m_install);
    if (!m_final.empty()) std::println("Finalize: {}", m_final);

    // Source 0 is main archive to extract (Multiple Sources)
    if (m_config["sources"]) {
        m_pkgurl = m_config["sources"][0]["url"].as<std::string>();
        m_archive = getBaseName(m_pkgurl);
        m_pkgmd5 = m_config["sources"][0]["md5"].as<std::string>();
        std::println("Package: {} URL:{} MD5:{}", m_archive, m_pkgurl, m_pkgmd5);
    }

    // Multiple Patches
    if (m_config["patches"]) {
        for (const auto& patch : m_config["patches"]) {
            std::string url = patch["url"].as<std::string>();
            std::string md5 = patch["md5"].as<std::string>();
            std::println("Patch:{} URL:{} MD5:{}", getBaseName(url), url, md5);
        }
    }
}
