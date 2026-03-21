#pragma once

#include <string>
#include <filesystem>

namespace Arca {
    enum ModuleType {
        CORE_TYPE,
        EXTERNAL_TYPE,
        EXTERNAL_FREEDOM_TYPE
    };

    enum ModuleStatus {
        READY_FOR_PROCESSING,
        PROCESSED
    };

    typedef struct ApplicationMetaData {
        std::string instanceName;
        std::string creatorName;
        std::string version;
    } ArcaInstanceConfig;

    typedef struct ModuleConfig {
        std::string moduleName;
        std::filesystem::path moduelPath;
        ModuleType type;
        ModuleStatus status;
    } ModuleConfig;
}