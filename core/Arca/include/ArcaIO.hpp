//-----------------------------------------------------------------------------
//                              RenderLamp - ArcaIO
//
//  This class is responsible for file handleing.
//-----------------------------------------------------------------------------

#ifndef ARCA_IO_HPP
#define ARCA_IO_HPP

#include <iostream>
#include <fstream>
#include <filesystem>


#include <string>
#include <map>



class ArcaIO {
    public:
        // General Functions
        bool IsFileExists(const std::filesystem::path& fullFilePath);
        bool CreateFolder(const std::filesystem::path& path, const std::string& name);
        bool IsFolderExists(const std::filesystem::path& path);
};

#endif