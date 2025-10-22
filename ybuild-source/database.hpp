
bool loadDatabase() {
    if (!DATABASE || DATABASE.IsNull()) {
        DATABASE = YAML::LoadFile(m_dbfile);
        if (DATABASE) {
            yprint::good("Database Loaded");
            return true;
        } else {
            yprint::bad("Failed to Load Database");
        }
    }
    return false;
}

bool check_built(const std::string& name, const std::string& ver) {
    if (!DATABASE["installed"] || !DATABASE["installed"].IsSequence()) return false;
    for (const auto& entry : DATABASE["installed"]) {
        if (entry["name"] && entry["name"].as<std::string>() == name) {
            if (entry["version"] && entry["version"].as<std::string>() == ver) {
                return true;
            }
        }
    }
    return false;
}

bool package_exists(const std::string& name, const std::string& ver) {
    // Load existing database if present
    if (!DATABASE || DATABASE.IsNull()) {
        loadDatabase();
    }

    if (!DATABASE["installed"]) return false;

    for (const auto& pkg : DATABASE["installed"]) {
        if (pkg["name"] && pkg["name"].as<std::string>() == name) {
            if (pkg["version"] && pkg["version"].as<std::string>() == ver) {
                return true;
            }
        }
    }
    return false;
}

bool is_installed(const std::string& name) {
    if (!DATABASE["installed"] || !DATABASE["installed"].IsSequence()) return false;
    for (const auto& entry : DATABASE["installed"]) {
        if (entry["name"] && entry["name"].as<std::string>() == name) {
            return true;
        }
    }
    return false;
}

int depend_check() {
    int missing = 0;
    for (const auto& category : {"required", "recommended", "optional"}) {
        if (!m_config[category] || !m_config[category].IsSequence()) continue;
        for (const auto& pkg : m_config[category]) {
            std::string name = pkg.as<std::string>();
            if (!is_installed(name)) {
                yprint::missing(name);
                missing += (category != std::string("optional"));
            } else {
                yprint::found(name);
            }
        }
    }
    return missing;
}

int update_db() {
    // Check for duplicate
    if (package_exists(m_pkgname, m_pkgver)) {
        std::println("Package '{}' already exists in database, skipping.", m_pkgname);
        return 0;
    }
    // Get current date/time string
    std::time_t now = std::time(nullptr);
    char buf[100];
    std::strftime(buf, sizeof(buf), "%d-%a-%Y %Z %H:%M:%S", std::localtime(&now));

    // Create package node
    YAML::Node pkg;
    pkg["name"] = m_pkgname;
    pkg["version"] = m_pkgver;
    pkg["url"] = m_pkgurl;
    pkg["yaml"] = FILEPATH.string();
    pkg["date"] = buf;
    // Test Print out
    for (const auto& it : pkg) {
        std::println("{} => {}",
                     it.first.as<std::string>(),
                     it.second.as<std::string>()
        );
    }

    // Ensure "installed" is a sequence
    if (!DATABASE["installed"]) {
        DATABASE["installed"] = YAML::Node(YAML::NodeType::Sequence);
    }

    // Append new package
    DATABASE["installed"].push_back(pkg);

    // Write to file
    std::ofstream fout(m_dbfile);
    fout << "# Package Database\n" << DATABASE;

    return 0;
}
