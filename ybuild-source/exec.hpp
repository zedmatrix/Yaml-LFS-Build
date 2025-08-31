#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>

bool execute(const std::filesystem::path& command) {
    std::vector<std::string> env_strings;
    env_strings.push_back("YSRC=" + m_ysrc.string());
    env_strings.push_back("PKGDIR="  + m_pkgdir);
    env_strings.push_back("PKGNAME=" + m_pkgname);
    env_strings.push_back("PKGVER="  + m_pkgver);
    env_strings.push_back("PKGURL=" + m_pkgurl);
    env_strings.push_back("PKGMD5="  + m_pkgmd5);
    env_strings.push_back("LC_ALL=POSIX");

    bool lfs = static_cast<std::string>(m_rootPath).starts_with("/mnt/lfs");
    if (lfs) {
        env_strings.push_back("LFS=/mnt/lfs");
        env_strings.push_back("LFS_TGT=x86_64-ybuild-linux-gnu");
        env_strings.push_back("CONFIG_SITE=" + (m_lfs / "usr/share/config.site").string());
        env_strings.push_back("PATH=" + (m_lfs / "tools/bin:/usr/bin:/bin:/sbin").string());
    } else {
        env_strings.push_back("PATH=/usr/bin:/usr/sbin");
        env_strings.push_back("MAKEFLAGS=-j4");
        env_strings.push_back("NINJAJOBS=4");
    }

    std::vector<char*> envp;
    for (auto& s : env_strings) {
        envp.push_back(const_cast<char*>(s.c_str()));
        printf("*** Setting ENV: %s \n",s.c_str());
    }
    envp.push_back(nullptr);

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
