#ifndef ARCA_CONTAINER_HPP
#define ARCA_CONTAINER_HPP

#include <map>
#include <any>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>

#include <nlohmann/json.hpp>

class ArcaContainer {
    public:
        ArcaContainer(const std::filesystem::path& serilaizedFilePath);
        ArcaContainer(const std::filesystem::path& path, const std::string& fileName);
        
        bool Dispatch();
        bool Unpack();

        std::filesystem::path GetPath();
        std::string GetName();
        

        void AddPair(const std::string& key, const std::any& value);

        template<typename T>
        T GetValue(const std::string& key) {
            auto i = mContainer[key];
            return std::any_cast<T>(i);
        }

    private:
        std::string mName;
        std::filesystem::path mPath;
        std::map<std::string, std::any> mContainer;

        bool Load(const nlohmann::json& fs);
        nlohmann::json Save();
};

#endif