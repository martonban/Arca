#ifndef ARCA_MODULE_HPP
#define ARCA_MODULE_HPP

#include <filesystem>
#include <string>
#include <map>
#include <nlohmann/json.hpp>
#include <fstream>


#include "ArcaIO.hpp"
#include "ArcaContainer.hpp"
#include "ArcaAsset.hpp"

class ArcaModule {
    public:
        ArcaModule(const std::filesystem::path& path, const std::string& name);
        ArcaModule(const std::filesystem::path& serilizedModuleFilePath);
        
        // Serialiazation
        bool Serialize();
        bool Deserialize();

        std::filesystem::path GetPath();
        std::filesystem::path GetFilePath();
        std::string GetName();

        void AddAsset(const std::string& key, const std::filesystem::path& path);
        std::filesystem::path GetAsset(const std::string& key);

        bool AddContainer(ArcaContainer& container);
        bool CreateNewContainer(std::string& name);
        std::shared_ptr<ArcaContainer> GetContainer(const std::string& key);
        

    private:
        std::string mModuleName;
        std::filesystem::path mPath;
        bool hasConfigFolder = false;

        std::vector<std::filesystem::path> mContainerVector;
        std::map<std::string, std::shared_ptr<ArcaContainer>> mContainerMap;

        std::map<std::string, std::filesystem::path> mAssetMap;
        
        ArcaIO io;

        std::filesystem::path PathBuilder(const std::filesystem::path& path, const std::string& name);
        bool Load(const nlohmann::json& fs);
        nlohmann::json Save();
};


#endif