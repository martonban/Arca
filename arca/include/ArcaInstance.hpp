#ifndef ARCA_INSTANCE_HPP
#define ARCA_INSTANCE_HPP

#include <memory>
#include <string>
#include <cctype>
#include <map>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "ArcaModule.hpp"
#include "ArcaCommon.hpp"
#include "ArcaIO.hpp"

class ArcaInstance {
    public:
        // Insntace Realted Funtions
        static ArcaInstance& GetInstance() {
            static ArcaInstance instance;
            return instance;
        }
        void StartArcaInstance(const std::string& applicationName);

        void FetchInstance();

        void AddApplicationMetadata(const Arca::ApplicationMetaData& metadataStruct);
        void Build();

        bool AddModule(const Arca::ModuleConfig& moduleConfig);
        Arca::ModuleConfig GetModule(const std::string& moduleName);
        void ProcessModuleConfig(Arca::ModuleConfig& config);

        void Test();

        std::filesystem::path GetApplicationPath();
        std::filesystem::path GetInstanceFolder();
        std::filesystem::path GetInstancePath();

        bool IsArcaNew();
        bool IsArcaInstanceReadyToSave();
        bool IsArcaInstanceReadyToLoad();
        

    protected:
        ArcaInstance() = default;
    private:
        // State Varriabels
        Arca::ArcaInstanceStatus _arcaInstanceStatus;       // Arca Instance Lifetime indicator
        std::filesystem::path _applicationFolderPath;       // absolute path: ApplicationRoot
        std::filesystem::path _instanceFolderPath;          // absolute path: ApplicationRoot/ArcaFiles
        std::filesystem::path _instanceFilePath;            // absolute path: ApplicationRoot/ArcaFiles/AppName.json

        Arca::ApplicationMetaData _metadata;

        std::map<std::string, Arca::ModuleConfig> _moduleMap;


        void CreateFolderStruct();

        nlohmann::json MetadataSerilaization();
        nlohmann::json ModulesSerialization();

        Arca::ApplicationMetaData MetadataDeserilaization(const nlohmann::json& jsonObject);
        std::map<std::string, Arca::ModuleConfig> ModulesDeserialiazation(const nlohmann::json& jsonObject);

        // Validators
        bool IsInstaceFileValid();
        bool IsMetadataValid(const Arca::ApplicationMetaData& metaData);
        bool IsModuleConfigValid(const Arca::ModuleConfig& config);

        // Helper Functions
        std::string StringToUpperCamelCase(const std::string& input);
 

        // Instace realated guard functions 
        ArcaInstance(const ArcaInstance&) = delete;
        ArcaInstance& operator=(const ArcaInstance&) = delete;
        // Arca Instance Builder 
        bool InstanceBuilder(const std::filesystem::path& path);     
};

#endif