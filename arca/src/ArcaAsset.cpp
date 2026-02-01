#include "ArcaAsset.hpp"
#include <fstream>

ArcaAsset::ArcaAsset(const std::filesystem::path& path) {
    mPath = path;
}

ArcaAsset::ArcaAsset(const std::filesystem::path& path, const std::string& fileName, const std::string& extension) {
    mPath = PathBuilder(path, fileName, extension);
}

std::filesystem::path ArcaAsset::GetAsset() {
    return mPath;
}

void ArcaAsset::AddSingleStreamAndDispatch(std::ostringstream& stream) {
    std::ofstream ofs(mPath);
    if (!ofs) {
        throw std::runtime_error("Failed to open file: " + mPath.string());
    }
    ofs << stream.str();
    if (ofs.fail()) {
        throw std::runtime_error("Failed to write to file: " + mPath.string());
    }
}

// PRIVATE FUNCTIONS
std::filesystem::path ArcaAsset::PathBuilder(const std::filesystem::path& path, const std::string& fileName, const std::string& extension) {
    return path / (fileName + extension);
}
