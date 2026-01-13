#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>

struct ExecuteStatus {
    int code = -1;
    std::vector<std::string> output;
};

ExecuteStatus execCapture(const std::filesystem::path& cmd) {
    ExecuteStatus execStatus;
    std::vector<std::string> env_strings = build_environment();
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

    std::string command = cmd.string();
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        execStatus.code = -1;
        return execStatus;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        execStatus.code = -1;
        return execStatus;
    }

    if (pid == 0) {
        // Child
        close(pipefd[0]);              // close read end
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);

        char* const args[] = {
            (char*)"/usr/bin/bash",
            (char*)"-c",
            (char*)command.c_str(),
            nullptr
        };
        if (execve("/usr/bin/bash", args, envp.data()) == -1) {
            perror("execve failed");
            _exit(127);
        }
    } else {
        close(pipefd[1]); // parent only reads

        std::vector<std::string> result;
        char buffer[4096];
        std::string line;

        ssize_t n;
        while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            line.append(buffer, n);

            size_t pos;
            while ((pos = line.find('\n')) != std::string::npos) {
                std::string outLine = line.substr(0, pos);

                execStatus.output.emplace_back(outLine);
                std::println(">>> {}", outLine);

                line.erase(0, pos + 1);
            }

        }
        if (!line.empty()) execStatus.output.emplace_back(line);

        close(pipefd[0]);

        int status = 0;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
            execStatus.code = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            execStatus.code = 128 + WTERMSIG(status);
        else
            execStatus.code = -1;

        return execStatus;
    }
    return execStatus;
}
