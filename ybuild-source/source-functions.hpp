bool getSources() {
    if (!m_config["sources"]) {
        return false;
    }
    bool success = true;
    std::cout << "From getSources" << m_ysrc << std::endl;
    make_dir(m_ysrc);
    std::string url, md5, file;
    for (const auto& src : m_config["sources"]) {
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
        //TODO check sha256sum check of file
    }

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
        //TODO check sha256sum check of file
    }
    return success;
}
