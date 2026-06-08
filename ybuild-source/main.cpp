#include "Zed_Print_Header.hpp"
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
        yprintln("Usage: {} [package]", argv[0]);
        return 1;
    }

    Zed_Print zp;
    TimeManager tm;
    tm.start();

    FILEPATH = argv[1];
    if (!std::filesystem::exists(FILEPATH)) return 1;

    if (fs::exists(certFile)) {
        m_addCert = certFile.string();
    }

    // Get Package Data
    if (!loadPackage(FILEPATH)) {
        zp.fail("Error Loading Package Definition");
        return 127;
    }
    m_package_path = absolutePath(FILEPATH);
    fs::current_path(m_package_path);

    if (!loadDatabase(zp)) return 127;

    if (check_built(m_pkgname, m_pkgver)) {
        m_error_text = std::format("Package: {}-{} Is Found in Database. Skipping.", m_pkgname, m_pkgver);
        zp.pass(m_error_text);
        return 0;
    }

    m_DEBUG = true;
    int m_error = depend_check(zp);
    if (m_error != 0) {
        zp.fail("Missing Dependencies");
        return m_error;
    }

    m_build_dir = (m_root_path / "tmp" / m_pkgdir);
    createBuildDir();

    m_log_dir = (m_root_path / "log" / m_pkgdir);
    make_dir(m_log_dir);

    if (m_destdir_bool) {
        m_destdir = (m_root_path / "image" / m_pkgdir);
        make_dir(m_destdir);
    }

	m_xml_dir = (m_root_path / "xml");
    make_dir(m_xml_dir);
    m_xml_file = m_pkgname + ".xml";

    m_ysrc = (m_root_path / "sources");
    make_dir(m_ysrc);

    uint16_t m_width = zp.get_width() - 3;
    yprintln("{}", stars(m_width));
    yprintln("*** Main  ROOT: {}", m_root_path.string());
    yprintln("*** Main  YPKG: {}", m_package_path.string());
    yprintln("*** Main  YSRC: {}", m_ysrc.string());
    yprintln("*** Main BUILD: {}", m_build_dir.string());


    if (!getSources()) {
        zp.fail("Error Downloading Sources");
        return 1;
    }

    if (!m_gitrepo) {
        if (!extract(zp)) {
            zp.fail("Error Extracting Sources");
            return 1;
        }
    }

    yprintln("{}", stars(m_width));
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

    yprintln("{}", stars(m_width));

    change_dir(m_rootPath);
    m_DEBUG = false;
    update_db();
    if (m_delete) {
        yprintln("Removing Build Directory");
        std::uintmax_t files = fs::remove_all(m_build_dir);
        yprintln(Color::Code::RED, "Removed: {} Files", files);
    } else {
        yprintln(Color::Code::BLUE, "Build Directory - Remains");
    }
    tm.stop();

/*      XML Print Summary
        Binutils pass1 took 2min 31secs on this machine (154)
        Binutils pass1 took 68 seconds on this machine (70)
*/
	double baseline = 154.0;
    if (!print_xml_out(tm.sbu_str(baseline), zp)) zp.fail("XML Out returned a failure");

    yprintln("Build Time: {}", tm.hms());

    //Get Size of Image Directory
    if (m_destdir_bool) {
	    FileMap m_final_destdir = scan_directory(m_destdir);
	    std::string m_final_destdir_size = directory_size_hr(m_final_destdir);
	    yprintln("Image Dir: {} Size: {}", m_destdir.string(), m_final_destdir_size);
	}

    yprintln("{}", stars(m_width));
    return 0;
}
