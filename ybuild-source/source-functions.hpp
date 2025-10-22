
bool gitRepo(const std::string& url) {
    if (!url.starts_with("git+")) {
        return false; // not a git repo
    }
    std::string gitUrl = url.substr(4); // strip "git+"

    // ensure build dir is clean
    if (fs::exists(m_build_dir)) {
        std::println("Removing old build dir: {}", m_build_dir.string());
        fs::remove_all(m_build_dir);
    }

    std::println("Cloning git repo: {}", gitUrl);
    if (!execute(("git clone " + gitUrl + " " + m_build_dir.string()).c_str())) {
        std::println("Failed to clone repo: {}", gitUrl);
        return false;
    }
    return true;
}

std::string extension(const std::string& str) {
    if (str.ends_with(".tar.gz")) return ".tar.gz";
    if (str.ends_with(".tar.xz")) return ".tar.xz";
    if (str.ends_with(".tar.bz2")) return ".tar.bz2";

    auto pos = str.find_last_of('.');
    return (pos != std::string::npos) ? str.substr(pos) : "";

}

bool getSources() {
    if (!m_config["sources"]) {
        yprint::bad("No Defined Sources!");
        return false;
    }
    bool success = true;
    bool rename;
    yprint::out(std::format("From getSources: {}", m_ysrc.string()));
    make_dir(m_ysrc);
    std::string url, md5, sha256, file, ext;
    fs::path filepath;
    int idx = 0;
    for (const auto& src : m_config["sources"]) {
        url = src["url"].as<std::string>();
        rename = src["rename"].as<bool>(false);
        m_gitrepo = false;
        if (url.starts_with("git+")) {
            m_gitrepo = gitRepo(url);
            if (!m_gitrepo) {
                success = false;
                break;
            }

        } else {
            md5 = src["md5"].as<std::string>();
            sha256 = src["sha256"].as<std::string>();
            file = getBaseName(url);
            if (idx == 0 && rename) {
                ext = extension(file);
                m_archive = (m_pkgdir + ext);
                filepath = m_ysrc / m_archive;
            } else {
                filepath = m_ysrc / file;
            }

            if (!fs::exists(filepath)) {
                yprint::out(std::format("Downloading: {}", (idx == 0 ? m_archive : file)));
                if (download(url, filepath) != 0) {
                    yprint::bad(std::format("Failed To Download: {}", (idx == 0 ? m_archive : file)));
                    success = false;
                    break;
                };
            } else {
                yprint::found(filepath);
            }
        }

        if (!sha256.empty() && sha256 != "null") {
            std::string actual = compute_sha256(filepath);
            if (actual == sha256) {
                yprint::good("✔ SHA256 verified");
            } else {
                yprint::bad(std::format("✖ SHA256 mismatch! - Expected: {}\nGot: {}", sha256, actual));
            }
        } else if (!md5.empty() && md5 != "null") {
            std::string actual = compute_md5(filepath);
            if (actual == md5) {
                yprint::good("✔ MD5 verified");
            } else {
                yprint::bad(std::format("✖ MD5 mismatch - Expected: {}\nGot: {}", md5, actual));
            }
        } else {
            yprint::warn("Skipping checksum verification");
        }

        idx++;
    }

    time_delay(1);
    if (!m_config["patches"]) {
        return true;
    }
    for (const auto& src : m_config["patches"]) {
        url = src["url"].as<std::string>();
        md5 = src["md5"].as<std::string>();
        file = getBaseName(url);
        fs::path filepath = m_ysrc / file;

        if (!fs::exists(filepath)) {
            std::println("Downloading: {}", file);
            if (download(url, filepath) != 0) {
                std::println("Failed To Download: {}", file);
                success = false;
                break;
            };
        }
        if (!md5.empty()) {
            std::string actual = compute_md5(filepath);
            if (actual == md5)
                yprint::good("✔ MD5 verified");
            else
                yprint::bad(std::format("✖ MD5 mismatch - Expected: {}\nGot: {}", md5, actual));
        } else {
            yprint::warn("Skipping checksum verification");
        }
    }
    time_delay(1);
    return success;
}
