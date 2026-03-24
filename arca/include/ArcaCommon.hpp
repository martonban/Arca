#pragma once

#include <string>
#include <filesystem>

namespace Arca {
    enum ModuleType {
        CORE_TYPE= 0,
        EXTERNAL_TYPE = 1 ,
        EXTERNAL_FREEDOM_TYPE = 2
    };

    enum ModuleStatus {
        READY_FOR_PROCESSING = 0,
        PROCESSED = 1
    };

    enum ArcaInstanceStatus {
        FIRST_LAUNCH = 0,
        ARCA_FILES_DIRECTORY_CREATED = 1,
        ARCA_INSTANCE_FILE_IS_VALID = 2,
        ARCA_IS_READY = 3
    };

    typedef struct ApplicationMetaData {
        std::string instanceName;
        std::string creatorName;
        std::string version;
    } ArcaInstanceConfig;

    typedef struct ModuleConfig {
        std::string moduleName;
        std::filesystem::path modulePath;
        ModuleType type;
        ModuleStatus status;
    } ModuleConfig;
}