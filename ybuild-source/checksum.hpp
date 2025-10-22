#include <openssl/evp.h>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

std::string compute_digest(const std::filesystem::path& filePath, const std::string& algoName) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file)
        return {};

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    const EVP_MD* md = EVP_get_digestbyname(algoName.c_str());
    if (!md) {
        EVP_MD_CTX_free(ctx);
        return {};
    }

    EVP_DigestInit_ex(ctx, md, nullptr);

    char buffer[8192];
    while (file.read(buffer, sizeof(buffer)))
        EVP_DigestUpdate(ctx, buffer, file.gcount());
    if (file.gcount() > 0)
        EVP_DigestUpdate(ctx, buffer, file.gcount());

    std::vector<unsigned char> hash(EVP_MD_size(md));
    unsigned int len = 0;
    EVP_DigestFinal_ex(ctx, hash.data(), &len);
    EVP_MD_CTX_free(ctx);

    std::ostringstream oss;
    for (unsigned i = 0; i < len; ++i)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return oss.str();
}

inline std::string compute_sha256(const std::filesystem::path& path) {
    return compute_digest(path, "SHA256");
}

inline std::string compute_md5(const std::filesystem::path& path) {
    return compute_digest(path, "MD5");
}
