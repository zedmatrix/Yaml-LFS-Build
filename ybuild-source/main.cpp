#include "init.hpp"
#include "time_manager.hpp"
#include "dir_scan.hpp"
#include "dir_diff.hpp"
#include "yprint.hpp"
#include "checksum.hpp"
#include "load-package.hpp"
#include "fs-functions.hpp"
#include "database.hpp"
#include "exec.hpp"
#include "execCapture.hpp"
#include "download.hpp"
#include "source-functions.hpp"
#include "extractor.hpp"
#include "build-functions.hpp"
#include "xmlout.hpp"

int main(int argc, char* argv[]/*, char* envp[]*/) {

    if (argc < 2) {
        yprint::out(std::format("Usage: {} [package]", argv[0]));
        return 1;
    }

    TimeManager tm;
    tm.start();

    FILEPATH = argv[1];
    if (!std::filesystem::exists(FILEPATH)) return 1;

    if (fs::exists(certFile)) {
        m_addCert = certFile.string();
    }

    // Get Package Data
    loadPackage(FILEPATH);
    m_package_path = absolutePath(FILEPATH);

    fs::current_path(m_package_path);

    m_build_dir = (m_root_path / "tmp" / m_pkgdir);
    createBuildDir();

    m_log_dir = (m_root_path / "log" / m_pkgdir);
    make_dir(m_log_dir);

    m_ysrc = (m_package_path / m_source_dir);

    std::cout << "*** Main root: " << m_root_path << std::endl;
    std::cout << "*** Main ysrc: " << m_ysrc << std::endl;
    std::cout << "*** Main Build: " << m_build_dir << std::endl;
    make_dir(m_ysrc);

    std::println("{}", stars());
    loadDatabase();
    if (check_built(m_pkgname, m_pkgver)) {
        yprint::out(std::format("Package: {}-{} Is Found in Database. Skipping.", m_pkgname, m_pkgver));
        return 0;
    }

    m_DEBUG = true;
    int m_error = depend_check();
    if (m_error != 0) {
        yprint::bad("Missing Dependencies");
        return m_error;
    }

    if (!getSources()) {
        printf("Error Downloading Sources\n");
        return 1;
    }

    if (!m_gitrepo) {
        if (!extract()) {
            printf("Error Extracting Sources\n");
            return 1;
        }
    }

    std::println("{}", stars());
    //Get Size of Extracted Directory
    FileMap m_map_extract = scan_directory(m_build_dir);
    m_extract_size = directory_size_hr(m_map_extract);

    if (!applyPatches()) return 10;
    if (!prepare()) return 9;
    FileMap m_map_prepare = scan_directory(m_build_dir);

    if (!build()) return 8;
    FileMap m_map_build = scan_directory(m_build_dir);

    if (!check()) return 7;
    if (!install()) return 6;
    if (!finalize()) return 5;

    //Get Size of Finished Directory
    FileMap m_map_final = scan_directory(m_build_dir);
    m_final_size = directory_size_hr(m_map_final);

    DirDiff m_package_prepare_diff = directory_diff(m_map_extract, m_map_prepare);
    write_diff_log(m_package_prepare_diff, "prepare.log");

    DirDiff m_package_install_diff = directory_diff(m_map_prepare, m_map_build);
    write_diff_log(m_package_install_diff, "install.log");

    std::println("{}", stars());

    change_dir(m_rootPath);
    m_DEBUG = false;
    update_db();
    if (m_delete) {
        yprint::out("Removing Build Directory");
        std::uintmax_t files = fs::remove_all(m_build_dir);
        yprint::good(std::format("Removed: {} Files", files));
    } else {
        yprint::out("Build Directory - Remains");
    }
    tm.stop();

    //XML Print Summary
    double baseline = 70.0; // Binutils pass1 took 68 seconds on this machine
    print_xml_out(tm.sbu_str(baseline));

    yprint::out(std::format("Build Time: {}", tm.hms()));

    std::println("{}", stars());
    return 0;
}
