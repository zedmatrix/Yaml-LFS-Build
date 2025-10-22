#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>

bool execute(const std::filesystem::path& command) {
    std::string exec_path = "/usr/bin:/usr/sbin";
    auto append_if_exists = [&](const fs::path& p) {
        if (dir_exists(p)) {
            if (!exec_path.empty()) exec_path += ":";
            exec_path += p.string();
        }
    };

    std::vector<std::string> env_strings;
    env_strings.push_back("YSRC=" + m_ysrc.string());
    env_strings.push_back("ybuild_root_path=" + m_root_path.string());
    env_strings.push_back("package_path=" + m_package_path.string());
    env_strings.push_back("PKGDIR=" + m_pkgdir);
    env_strings.push_back("PKGNAME=" + m_pkgname);
    env_strings.push_back("PKGVER=" + m_pkgver);
    if (!m_cflags.empty()) {
        env_strings.push_back("CFLAGS=" + m_cflags);
        env_strings.push_back("CXXFLAGS=" + m_cflags);
    }

    if (lfs) {
        env_strings.push_back("LC_ALL=POSIX");
        env_strings.push_back("LFS=" + m_lfs.string());
        env_strings.push_back("LFS_TGT=" + m_lfstgt);
        env_strings.push_back("CONFIG_SITE=" + (m_lfs / "usr/share/config.site").string());
        env_strings.push_back("PATH=" + (m_lfs / "tools/bin:/usr/bin:/bin:/sbin").string());
    } else {
        append_if_exists(ant_bin);
        append_if_exists(jdk_bin);
        append_if_exists(rustc_bin);
        append_if_exists(kf6_bin);
        append_if_exists(qt6_bin);
        env_strings.push_back("LC_ALL=" + m_locale);
        env_strings.push_back("PATH=" + exec_path);
        env_strings.push_back("MAKEFLAGS=-j4");
        env_strings.push_back("NINJAJOBS=4");
    }

    std::vector<char*> envp;

    for (auto& s : env_strings) {
        envp.push_back(const_cast<char*>(s.c_str()));
    }

    // terminate with nullptr
    envp.push_back(nullptr);

    // Test printout (optional)
    if (m_DEBUG) {
        for (auto& s : env_strings) {
            printf("*** Setting ENV: %s \n", s.c_str());
        }
    }

    char* const args[] = {
        (char*)"/usr/bin/bash",
        (char*)"-c",
        (char*)command.c_str(),
        nullptr
    };

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return false;
    }

    if (pid == 0) {
        if (execve("/usr/bin/bash", args, envp.data()) == -1) {
            perror("execve failed");
        }
    } else {
        int status = 0;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            return false;
        }
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }

    return false; // should never reach here
}
