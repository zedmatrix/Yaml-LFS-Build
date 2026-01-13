struct DirDiff {
    FileMap added;
    FileMap removed;
    FileMap modified;
};

DirDiff directory_diff(const FileMap& oldMap, const FileMap& newMap)
{
    DirDiff diff;

    for (const auto& [path, size] : oldMap) {
        auto it = newMap.find(path);
        if (it == newMap.end()) {
            diff.removed.emplace(path, size);
        } else if (it->second != size) {
            diff.modified.emplace(path, it->second);
        }
    }

    for (const auto& [path, size] : newMap) {
        if (!oldMap.contains(path)) {
            diff.added.emplace(path, size);
        }
    }

    return diff;
}

void write_diff_log(const DirDiff& diff, const std::filesystem::path& logfile) {

    std::ofstream out(m_log_dir / logfile);

    out << "[added]\n";
    for (const auto& [path, size] : diff.added)
        out << path << " " << size << "\n";

    out << "\n[modified]\n";
    for (const auto& [path, size] : diff.modified)
        out << path << " " << size << "\n";

    out << "\n[removed]\n";
    for (const auto& [path, size] : diff.removed)
        out << path << " " << size << "\n";

}
