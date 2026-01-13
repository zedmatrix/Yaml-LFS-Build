#include <string>
#include <string_view>

inline std::string xml_entity(std::string_view name, std::string_view value) {
    return std::format("<ENTITY {}-{} \"{}\" >", m_pkgname, name, value);
}

static void print_xml_out(std::string baseline) {
    std::string m_xmlout;
    m_xmlout = std::format("{}\n{}\n{}\n{}",
                         xml_entity("version  ", m_pkgver),
                         xml_entity("size     ", m_extract_size),
                         xml_entity("buildsize", m_final_size),
                         xml_entity("time     ", baseline));
    yprint::out(m_xmlout);
}
