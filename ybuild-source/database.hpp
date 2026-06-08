#include "Zed_Print_Header.hpp"

bool loadDatabase(Zed_Print& zp) {
    if (!DATABASE || DATABASE.IsNull()) {
        DATABASE = YAML::LoadFile(m_dbfile);
        if (DATABASE) {
            zp.pass("Database Loaded");
            return true;
        }
    }
    zp.fail("Failed to Load Database");
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
	if (!yaml_iskey_valid(DATABASE, "installed")) return false;

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
	if (!yaml_iskey_valid(DATABASE, "installed")) return false;

    for (const auto& entry : DATABASE["installed"]) {
        if (entry["name"] && entry["name"].as<std::string>() == name) {
            return true;
        }
    }
    return false;
}

int depend_check(Zed_Print& zp) {
    int missing = 0;
    std::string name;
    for (const auto& category : {"required", "recommended", "optional", "runtime"}) {
    	if (!yaml_iskey_valid(m_config, category)) continue;
        for (const auto& pkg : m_config[category]) {
            name = pkg.as<std::string>();
            if (!is_installed(name)) {
            	zp.fail(std::format("{} Missing: {}",color(true, Color::Code::RED), name));
            	std::string_view cat(category);
            	if (cat != "optional" && cat != "runtime") missing ++;
            } else {
                zp.pass(name);
            }
        }
    }
    return missing;
}

int update_db() {
    // Check for duplicate
    if (package_exists(m_pkgname, m_pkgver)) {
        yprintln("Package '{}' already exists in database, skipping.", m_pkgname);
        return 0;
    }
    // Get current date/time string
    std::time_t now = std::time(nullptr);
    char buf[100];
    std::strftime(buf, sizeof(buf), "%a %b %d-%Y %Z %H:%M:%S", std::localtime(&now));

    // Create package node
    YAML::Node pkg;
    pkg["name"] = m_pkgname;
    pkg["version"] = m_pkgver;
    pkg["url"] = m_pkgurl;
    pkg["yaml"] = FILEPATH.string();
    pkg["date"] = buf;
    // Test Print out
    for (const auto& it : pkg) {
        yprintln("{} => {}", it.first.as<std::string>(), it.second.as<std::string>());
    }

    // Ensure "installed" is a sequence
	if (!yaml_iskey_valid(DATABASE, "installed")) {
        DATABASE["installed"] = YAML::Node(YAML::NodeType::Sequence);
    }

    // Append new package
    DATABASE["installed"].push_back(pkg);

    // Write to file
    std::ofstream fout(m_dbfile);
    fout << "# Package Database\n" << DATABASE;

    return 0;
}
