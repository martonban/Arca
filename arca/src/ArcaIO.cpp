#include "ArcaIO.hpp"

bool ArcaIO::IsFileExists(const std::filesystem::path& fullFilePath) {
    return std::filesystem::exists(fullFilePath) && std::filesystem::is_regular_file(fullFilePath);
}

bool ArcaIO::CreateFolder(const std::filesystem::path& path, const std::string& name) {
    std::filesystem::path fullPath = path / name;
    try {
        if (std::filesystem::exists(fullPath)) {
            std::cerr << "Warning: Directory already exists at path: " << fullPath << std::endl;
            return true;
        }
    
        if (!std::filesystem::create_directory(fullPath)) {
            std::cerr << "Error: Failed to create directory at path: " << fullPath << std::endl;
            return false;
        }
        
        return true;
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return false;
    }
}


bool ArcaIO::IsFolderExists(const std::filesystem::path& path) {
    try {
        return std::filesystem::exists(path) && std::filesystem::is_directory(path);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return false;
    }
}