#include <curl/curl.h>
#include <iomanip>

int download(const std::string& view, const std::filesystem::path& filePath) {
    std::string url(view);
    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize curl.\n");
        return -1;
    }
    FILE *fp = fopen(filePath.string().c_str(), "wb");
    if (!fp) {
        perror("fopen");
        curl_easy_cleanup(curl);
        return -1;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
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
    }

    fclose(fp);
    curl_easy_cleanup(curl);
    return static_cast<int>(res);
}
