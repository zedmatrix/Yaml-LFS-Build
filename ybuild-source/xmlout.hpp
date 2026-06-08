#include <string>
#include <string_view>
#include "filesize.hpp"
#include "Zed_Print_Header.hpp"

std::string m_capname;
inline void init_caps() {
    m_capname = m_pkgname;
    m_capname[0] = std::toupper(m_capname[0]);
}

inline const std::string head = R"""(<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE sect1 PUBLIC "-//OASIS//DTD DocBook XML V4.5//EN"
   "http://www.oasis-open.org/docbook/xml/4.5/docbookx.dtd" [
  <!ENTITY % general-entities SYSTEM "../general.ent">
  %general-entities;
)""";
// std::string constructor repeat
inline std::string indent(int level, int width = 2) {
    return std::string(level * width, ' ');
}

inline std::string xml_entity(std::string_view name, std::string_view value) {
    return std::format("  <!ENTITY {}-{} \"{}\">", m_pkgname, name, value);
}

inline std::string xml_string(std::string text1) {
    return std::format("&{}-{};", m_pkgname, text1);
}

inline std::string replace_pkgver(std::string_view text) {
    constexpr std::string_view token = "${PKGVER}";
    const std::string version = xml_string("version");
    std::string result(text);
    std::size_t pos = result.find(token);
    while (pos != std::string::npos) {
        result.replace(pos, token.size(), version);
        pos = result.find(token, pos + version.size());
    }
    return result;
}

inline std::string xml_para(std::string text1, int level = 2) {
    return std::format("\n{}<para>{}</para>\n", indent(level), text1);
}

inline std::string xml_package(std::string text1, std::string text2) {
    //BLFS Style Package Print
    std::string tmp;
	tmp += std::format("\n{}<listitem>", indent(4));
    tmp += xml_para(std::format("{}: {}", text1, text2), 5);
    tmp += std::format("\n{}</listitem>", indent(4));
    return tmp;
}
inline std::string screen_userinput(int function, std::string_view text) {
    std::string role;
    switch (function) {
        case 1:
            role = "configure";
            break;
        case 2:
            role = "make";
            break;
        case 3:
            role = "test";
            break;
        case 4:
            role = "install";
            break;
        case 5:
            role = "post";
            break;
        default:
            role = "pre";
            break;
    }
    return std::format("\n{}<screen><userinput remap=\"{}\">{}</userinput></screen>\n", indent(4), role, text);
}

//	TODO possible specialized XML Print out via template
static bool print_xml_out(std::string baseline, Zed_Print& zp) {
	init_caps();
    std::filesystem::path filepath = m_ysrc / m_archive;
    std::string tar_size = compute_filesize(filepath);
    std::string sha256 = compute_sha256(filepath);
    std::string md5 = compute_md5(filepath);
    std::string m_xmlout;
    m_xmlout = std::format("{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n{}\n]>",
                         head,
                         xml_entity("version   ", m_pkgver),
                         xml_entity("size      ", tar_size),
                         xml_entity("http-url  ", m_pkgurl),
                         xml_entity("sha256sum ", sha256),
                         xml_entity("md5sum    ", md5),
                         xml_entity("extracted ", m_extract_size),
                         xml_entity("buildsize ", m_final_size),
                         xml_entity("buildtime ", baseline));

	if (XML_PRINT == "SCREEN") {
		yprint::out(m_xmlout);
		return true;
	}

	m_xmlout += std::format("\n <sect1 id=\"{}\" xreflabel=\"{}-{}\">", m_pkgname, m_capname, xml_string("version"));
    m_xmlout += std::format("\n    <?dbhtml filename=\"{}.html\"?>\n", m_pkgname);
    m_xmlout += std::format("\n{}<title>{}-{}</title>\n", indent(3), m_capname, xml_string("version"));
    m_xmlout += std::format("\n{}<indexterm zone=\"{}\">", indent(3), m_pkgname);
    m_xmlout += std::format("\n{}    <primary sortas=\"a-{}\">{}</primary>", indent(3), m_capname, m_capname);
    m_xmlout += std::format("\n{}</indexterm>\n", indent(3));

    m_xmlout += std::format("\n{}<sect2 role=\"package\">\n{}<title/>\n", indent(2), indent(3));
    m_xmlout += xml_para(m_pkgdesc, 3);
	//BLFS Style Package Print
    m_xmlout += std::format("\n{}<bridgehead renderas=\"sect3\">Package Information</bridgehead>\n", indent(3));
    m_xmlout += std::format("\n{}<itemizedlist spacing=\"compact\">", indent(3));
    m_xmlout += xml_package("Download HTTP",std::format("<ulink url=\"{}\"/>",xml_string("http-url")));
    m_xmlout += xml_package("Download SHA256 Sum", xml_string("sha256sum"));
    m_xmlout += xml_package("Download Size", xml_string("size"));
    m_xmlout += xml_package("Estimated Disk Space", xml_string("buildsize"));
    m_xmlout += xml_package("Estimated Build Time", xml_string("buildtime"));
    m_xmlout += std::format("\n{}</itemizedlist>", indent(3));
    // Dependencies
    auto yaml_as_string_or_list = [](const YAML::Node& node) -> std::string {
        if (node.IsScalar())
            return node.as<std::string>();
        if (node.IsSequence()) {
            std::string result;
            for (const auto& item : node) {
                if (!result.empty()) result += " ";
                result += item.as<std::string>();
            }
            return result;
        }
        return {};
    };
    if (yaml_iskey_valid(m_config, "required")) {
        std::string required = yaml_as_string_or_list(m_config["required"]);
        m_xmlout += std::format("{}<bridgehead renderas=\"sect3\">{} Dependencies</bridgehead>", indent(3), m_capname);
        m_xmlout += std::format("{}<bridgehead renderas=\"sect4\">Required</bridgehead>", indent(4));
        m_xmlout += xml_para(required, 5);
    }
    //Patch or Document
    m_xmlout += std::format("\n{}</sect2>\n", indent(2));

    m_xmlout += std::format("\n{}<sect2 role=\"installation\">", indent(3));
    m_xmlout += std::format("\n{}  <title>Installation of {}</title>\n", indent(3),m_capname);
    m_xmlout += xml_para(std::format("Prepare {} for compilation:",m_capname, 3));
    // Insert patches
    if (m_patch_bool) {
        std::string url, file;
        std::string patch_text = "patch -Np1 -i ../";
        for (const auto& patches : m_patches) {
            url = patches["url"].as<std::string>();
            file = getBaseName(url);
            m_xmlout += xml_para("Patch a vulnerability, issue:", 3);
            m_xmlout += screen_userinput(0, patch_text + file);
        }
    }
    // Configure
	if (!m_prepare.empty()) {
		m_xmlout += xml_para("Now Configure the Package, issue:", 3);
		m_xmlout += screen_userinput(1, replace_pkgver(m_prepare));
	}
    if (!m_build.empty()) {
        m_xmlout += xml_para("To Compile the Package, issue:", 3);
        m_xmlout += screen_userinput(2, m_build);
    }
    if (!m_check.empty()) {
        m_xmlout += xml_para("To Test the Package, issue:", 3);
        m_xmlout += screen_userinput(3, m_check);
    }
    if (!m_install.empty()) {
        m_xmlout += xml_para("Now Install the Package, issue:", 3);
        m_xmlout += screen_userinput(4, replace_pkgver(m_install));
    }
    if (!m_final.empty()) m_xmlout += screen_userinput(5, m_final);
    m_xmlout += std::format("\n{}</sect2>\n", indent(3));
    // Configuration


    // Content
	m_xmlout += std::format("\n{}<sect2 role=\"content\">", indent(3));
    m_xmlout += std::format("\n{}<title>Contents of {}</title>\n", indent(4), m_capname);
    m_xmlout += std::format("\n{}<segmentedlist>", indent(4));
    m_xmlout += std::format("\n{}<segtitle>Installed programs</segtitle>", indent(5));
    m_xmlout += std::format("\n{}<segtitle>Installed libraries</segtitle>", indent(5));
    m_xmlout += std::format("\n{}<segtitle>Installed directories</segtitle>\n", indent(5));
    m_xmlout += std::format("\n{}<seglistitem>", indent(4));
    m_xmlout += std::format("\n{}<seg>PROGRAMS</seg>", indent(5));
    m_xmlout += std::format("\n{}<seg>LIBRARIES</seg>", indent(5));
    m_xmlout += std::format("\n{}<seg>DIRECTORIES</seg>", indent(5));
    m_xmlout += std::format("\n{}</seglistitem>", indent(4));
    m_xmlout += std::format("\n{}</segmentedlist>", indent(4));
    m_xmlout += std::format("\n{}</sect2>\n", indent(3));

    // Close Section
    m_xmlout += std::format("\n{}</sect1>\n", indent(1));

    std::ofstream f(m_xml_dir / m_xml_file);
    if (!f) throw std::runtime_error("cannot open: " + m_xml_file);
    f << m_xmlout;

    if (file_exists(m_xml_dir / m_xml_file)) {
        zp.pass(m_xml_file);
    } else {
        zp.fail(m_xml_file);
        return false;
    }
    return true;
}
