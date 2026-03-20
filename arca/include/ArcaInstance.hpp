#ifndef ARCA_INSTANCE_HPP
#define ARCA_INSTANCE_HPP

#include <memory>
#include <string>
#include <map>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "ArcaModule.hpp"
#include "ArcaSettings.hpp"
#include "ArcaIO.hpp"

class ArcaInstance {
    public:
        // Insntace Realted Funtions
        static ArcaInstance& GetInstance() {
            static ArcaInstance instance;
            return instance;
        }
        void StartArcaInstance(const std::string& applicationName);

        void AddApplicationMetadata(const Arca::ApplicationMetaData& metadataStruct);
        void Build();

        bool AddModule(const Arca::ModuleConfig& moduleConfig);
        Arca::ModuleConfig GetModule(const std::string& moduleName);


        void FetchInstance();

        void Test();

        bool IsArcaInstanceAlive();

        std::filesystem::path GetApplicationPath();
        std::filesystem::path GetInstancePath();
        

    protected:
        ArcaInstance() = default;
    private:
        bool _instanceIsReady = false;
        std::filesystem::path _applicationFolderPath;    // absolute path: ApplicationRoot
        std::filesystem::path _instanceFilePath;        // absolute path: ApplicationRoot/ArcaFiles/AppName.json

        Arca::ApplicationMetaData _metadata;

        std::map<std::string, Arca::ModuleConfig> _moduleMap;

        nlohmann::json MetadataSerilaization();
        nlohmann::json ModulesSerialization();

        Arca::ApplicationMetaData MetadataDeserilaization(const nlohmann::json& jsonObject);
        std::map<std::string, Arca::ModuleConfig> ModulesDeserialiazation(const nlohmann::json& jsonObject);

        // Instace realated guard functions 
        ArcaInstance(const ArcaInstance&) = delete;
        ArcaInstance& operator=(const ArcaInstance&) = delete;
        // Arca Instance Builder 
        bool InstanceBuilder(const std::filesystem::path& path);     
};

#endif