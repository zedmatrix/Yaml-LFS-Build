#include <curl/curl.h>
#include <iomanip>
#include <openssl/evp.h>
#include <functional>
EVP_MD_CTX* mdctx;

size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    size_t total = size * nmemb;
    FILE* fp = static_cast<FILE*>(userdata);
    size_t written = fwrite(ptr, size, nmemb, fp);

    EVP_DigestUpdate(mdctx, ptr, total);
    return written;
}

std::string download(const std::string& view, const std::filesystem::path& filePath/*, bool sha256 = false*/) {
    std::string url(view);
    std::string result = "";
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize curl.\n");
        return "";
    }
    mdctx = EVP_MD_CTX_new();
    // const EVP_MD* digest = sha256 ? EVP_sha256() : EVP_md5();
    const EVP_MD* digest = EVP_md5();
    EVP_DigestInit_ex(mdctx, digest, NULL);

    FILE *fp = fopen(filePath.string().c_str(), "wb");
    if (!fp) {
        perror("fopen");
        curl_easy_cleanup(curl);
        return "";
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/8.7.1");
    if (!m_addCert.empty()) {
        curl_easy_setopt(curl, CURLOPT_CAINFO, m_addCert.c_str());
    }

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::println("Download failed: {}", curl_easy_strerror(res));
        std::filesystem::remove(filePath);
        result = "";
    } else {
        EVP_DigestFinal_ex(mdctx, hash, &hash_len);
        EVP_MD_CTX_free(mdctx);
        std::ostringstream oss;
        for (unsigned int i = 0; i < hash_len; ++i) {
            oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
        }
        // Debuging Test
        std::println("Download successful: {} \nChecksum: {}", filePath.string(), oss.str());
        result = oss.str();
    }

    fclose(fp);
    curl_easy_cleanup(curl);
    return result;
}
