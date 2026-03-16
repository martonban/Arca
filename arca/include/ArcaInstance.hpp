#ifndef ARCA_INSTANCE_HPP
#define ARCA_INSTANCE_HPP

#include <string>
#include <memory>
#include <filesystem>
#include <nlohmann/json.hpp>

#include "ArcaConfigs.hpp"
#include "ArcaModule.hpp"

class ArcaInstance {
    public:
        // Insntace Realted Funtions
        static ArcaInstance& GetInstance() {
            static ArcaInstance instance;
            return instance;
        }

        void StartArcaInstance(const std::string& applicationName);

    protected:
        ArcaInstance() = default;
    private:
        std::filesystem::path _instanceFolderPath;
        std::filesystem::path _instanceFilePath;
        
        bool _instanceIsReady = false;
        ArcaInstanceConfig _config;


        // Instace realated guard functions 
        ArcaInstance(const ArcaInstance&) = delete;
        ArcaInstance& operator=(const ArcaInstance&) = delete;
        // Arca Instance Builder 
        bool InstanceBuilder(const std::filesystem::path& path);     
};

#endif