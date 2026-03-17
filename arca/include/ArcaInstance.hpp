#ifndef ARCA_INSTANCE_HPP
#define ARCA_INSTANCE_HPP

#include <string>
#include <memory>
#include <fstream>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "ArcaSettings.hpp"
#include "ArcaIO.hpp"
#include "ArcaModule.hpp"

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

        void FetchInstance();

        void Test();

        bool IsArcaInstanceAlive();

        std::filesystem::path GetApplicationPath();
        std::filesystem::path GetInstancePath();
        

    protected:
        ArcaInstance() = default;
    private:
        std::filesystem::path _instanceFolderPath;
        std::filesystem::path _instanceFilePath;
        
        bool _instanceIsReady = false;
        Arca::ApplicationMetaData _metadata;

        nlohmann::json MetadataSerilaization();
        Arca::ApplicationMetaData MetadataDeserilaization(const nlohmann::json& jsonObject);

        // Instace realated guard functions 
        ArcaInstance(const ArcaInstance&) = delete;
        ArcaInstance& operator=(const ArcaInstance&) = delete;
        // Arca Instance Builder 
        bool InstanceBuilder(const std::filesystem::path& path);     
};

#endif