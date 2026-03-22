#ifndef ARCA_MODULE_HPP
#define ARCA_MODULE_HPP

#include <filesystem>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <memory>

#include "ArcaIO.hpp"
#include "ArcaCommon.hpp"

namespace Arca {
    class Module {
        public: 
            Module(const Arca::ModuleConfig& config);

            void Save();
            void Load(const Arca::ModuleConfig& config);
            void LoadFromConfig(const Arca::ModuleConfig& config);

            bool AddAccessPoint(const std::string& key, const std::filesystem::path& path);
            std::filesystem::path GetAccessPoint(const std::string& key);
            bool DeleteAccessPoint(const std::string& key);
            std::vector<std::pair<std::string, std::filesystem::path>> GetAllAccessPoint();

            
        private:
            std::string _name;
            std::filesystem::path _path;
            Arca::ModuleType _type;
            Arca::ModuleStatus _status;

            std::map<std::string, std::filesystem::path> _accessPoints;

            void ProcessModuleConfig(const Arca::ModuleConfig& config);

            nlohmann::json MetadataSerilaization();
            nlohmann::json AccessPointSerilaization();

            void MetaDataDeserilaiazation(nlohmann::json deserializedJson);
            std::map<std::string, std::filesystem::path> DeserializeAccessPoints(nlohmann::json deserializedJson);
    };
}

#endif