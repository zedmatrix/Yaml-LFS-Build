bool yaml_iskey_valid(const YAML::Node& yaml_node, std::string key) {
    return yaml_node.IsMap() && yaml_node[key].IsDefined();
}

std::string checkNode(const YAML::Node& config, const std::string& node) {
    if (!config[node]) {
        return {};
    }
    return config[node].as<std::string>();
}

bool loadPackage(std::filesystem::path& package) {
    m_config = YAML::LoadFile(package);

    m_prepare = checkNode(m_config, "prepare");
    m_build = checkNode(m_config, "build");
    m_check = checkNode(m_config, "check");
    m_install = checkNode(m_config, "install");
    m_final = checkNode(m_config, "final");
    m_pkgdesc = checkNode(m_config, "description");
    if (m_pkgdesc.empty()) m_pkgdesc = "description";

    if (yaml_iskey_valid(m_config, "package")) {
        m_package = m_config["package"];
    } else {
        return false;
    }

    m_pkgname = checkNode(m_package, "name");
    m_pkgver = checkNode(m_package, "version");
    m_pkgrel = m_package["rel"].as<int>(1);
    m_zarchive = m_package["archive"].as<bool>(false);
    m_delete = m_package["delete"].as<bool>(false);
    m_pkg_cflags = checkNode(m_package, "cflags");
    m_testsuite = m_package["ycheck"].as<bool>(false);
    m_destdir_bool = m_package["destdir"].as<bool>(false);

    m_pkgdir = std::format("{}-{}", m_pkgname, m_pkgver);
    // Test Print Out
    yprintln("Package Directory: {}", m_pkgdir);
    yprintln("Package Name: {} Version: {}", m_pkgname, m_pkgver);

    //TODO Test Printouts
    if (!m_prepare.empty()) std::println("Prepare: {}", m_prepare);
    if (!m_build.empty()) std::println("Build: {}", m_build);
    if (!m_check.empty()) std::println("Check: {}", m_check);
    if (!m_install.empty()) std::println("Install: {}", m_install);
    if (!m_final.empty()) std::println("Finalize: {}", m_final);

	// Mulitiple Sources vailidate Source 0 and setup m_sources Node
    m_sources = m_config["sources"];
    if (!checkNode(m_sources[0], "url").empty()) {
        const auto& src = m_sources[0];
        m_pkgurl = src["url"].as<std::string>();
        m_archive = getBaseName(m_pkgurl);

        if (src["sha256"]) {
            m_pkgsha256 = src["sha256"].as<std::string>();
        }
        if (src["md5"]) {
            m_pkgmd5 = src["md5"].as<std::string>();
        }
        else {
            m_pkgsha256.clear();
            m_pkgmd5.clear();
            yprint::warn("Missing Checksums on Main Package");
        }

        yprintln("Archive:{} URL:{} \nChecksums: SHA256SUM:{} \nMD5SUM: {}", m_archive, m_pkgurl, m_pkgsha256 ,m_pkgmd5);
        m_source_bool = true;
    } else {
        yprintln(Color::Code::RED, "Missing or Invalid Main Package");
        return false;
    }

    // Multiple Patches (View Only and Setup m_patches Node)
    if (yaml_iskey_valid(m_config, "patches")) {
        m_patches = m_config["patches"];
        std::string url, md5, sha256;
        for (const auto& patch : m_patches) {
            url = patch["url"].as<std::string>();
            sha256 = patch["sha256"].as<std::string>();
            md5 = patch["md5"].as<std::string>();
            yprintln("Patch:{} URL:{} \n SHA256:{} MD5:{}", getBaseName(url), url, sha256, md5);
        }
        m_patch_bool = true;
    }
    return true;
}
