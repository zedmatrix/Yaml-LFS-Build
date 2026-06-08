#include <filesystem>
#include <string>

namespace fs = std::filesystem;

std::string add_commas(double value, int precision = 0) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << value;
    std::string str = oss.str();

    // find decimal point and work backwards inserting commas
    size_t dot = str.find('.');
    int insert_pos = (dot != std::string::npos ? dot : str.size()) - 3;

    while (insert_pos > 0) {
        str.insert(insert_pos, ",");
        insert_pos -= 3;
    }

    return str;
}

std::string compute_filesize(std::filesystem::path filepath) {
    fs::directory_entry entry(filepath);

    if (!entry.exists() || !entry.is_regular_file())
        return "0 KB";

    double size = static_cast<double>(entry.file_size()) / 1024.0;

    return add_commas(size) + " KB";
}
