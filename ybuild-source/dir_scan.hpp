#pragma once
#include <unordered_map>
#include <filesystem>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <sstream>
#include <iostream>

using FileMap = std::unordered_map<std::string, uintmax_t>;

struct HumanReadable {
    std::uintmax_t size{};

    template<typename Os>
    friend Os& operator<<(Os& os, HumanReadable hr)
    {
        static constexpr char suffixes[] = "BKMG";

        int i = 0;
        double mantissa = static_cast<double>(hr.size);

        while (mantissa >= 1024.0 && i < 1) {
            mantissa /= 1024.0;
            ++i;
        }
        os << std::ceil(mantissa * 10.0) / 10.0 << suffixes[i];
        //if (i > 0) os << "B (" << hr.size << ')';

        return os;
    }
};

inline FileMap scan_directory(const std::filesystem::path& dir) {
    FileMap result;
    namespace fs = std::filesystem;

    try {
        for (auto const& entry :
            fs::recursive_directory_iterator(dir, fs::directory_options::skip_permission_denied))
        {
            if (entry.is_symlink()) continue;

            if (entry.is_regular_file()) {
                auto rel = fs::relative(entry.path(), dir);
                result[rel.string()] = entry.file_size();
            }
        }

    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << "\n";
    }

    return result;
}

inline uintmax_t directory_size(const FileMap& map) {
    uintmax_t result = 0;
    for(const auto& [file, size] : map) {
        result += size;
    }
    return result;
}

inline std::string directory_size_hr(const FileMap& map) {
    std::ostringstream oss;
    oss << HumanReadable{directory_size(map)};
    return oss.str();
}
