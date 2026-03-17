#pragma once

#include <string>

namespace Arca {
    typedef struct ApplicationMetaData {
        std::string instanceName;
        std::string creatorName;
        std::string version;
    } ArcaInstanceConfig;
}