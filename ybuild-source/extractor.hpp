#include <magic.h>
#include <vector>
#include <algorithm>
#include "Zed_Print_Header.hpp"

std::vector<std::string> MIMETYPES{"Unknown", "application/x-xz", "application/gzip",
                                    "application/x-bzip2", "application/zstd", "application/zip"};

// include from main.cpp
int filetype(std::filesystem::path& file) {

    magic_t magic = magic_open(MAGIC_MIME_TYPE);
    if (!magic || magic_load(magic, nullptr) != 0) {
        if (magic) magic_close(magic);
        return 0;
    }

    const char* type = magic_file(magic, file.c_str());
    std::string mime(type);
    yprintln("Detected type: {} ", type);

    magic_close(magic);

    auto it = std::find(MIMETYPES.begin(), MIMETYPES.end(), mime);
    return (it != MIMETYPES.end()) ? std::distance(MIMETYPES.begin(), it) : 0;
}

bool extract(Zed_Print& zp) {

    m_archivedir = m_ysrc / fs::path(m_archive);
    m_filetype = filetype(m_archivedir);

    if (m_filetype > 0 && m_filetype < 5) {
        yprintln("Looking For {}", tar_bin.string());
        if (file_exists(tar_bin)) {
            zp.pass(std::format("Found {}", tar_bin.string()));
        } else {
            zp.fail(std::format("Not Found {}", tar_bin.string()));
            return false;
        }

        command = std::format("tar -xf {} -C {} --strip-components={}",
                              m_archivedir.string(), m_build_dir.string(), m_pkgrel);

    } else if (m_filetype == 5) {
        yprintln("Looking For {}", unzip_bin.string());
        if (file_exists(unzip_bin)) {
            zp.pass(std::format("Found {}", unzip_bin.string()));
        } else {
            zp.fail(std::format("Not Found {}", unzip_bin.string()));
            return false;
        }

        command = std::format("bsdunzip -d {} {}", m_build_dir.string(), m_archivedir.string());

    } else {
        zp.fail("Invalid Archive File Type Detected!");
        return false;
    }

    yprintln("Executing: {}", command.c_str());
    return execute(command);
}
