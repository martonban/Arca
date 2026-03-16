#include "ArcaInstance.hpp"

void ArcaInstance::StartArcaInstance(const std::string& applicationName) {
    _instanceFolderPath = std::filesystem::path("./");
    _instanceFilePath = _instanceFolderPath / "ArcaFiles" / (applicationName + ".json");
    
    // BUILD OR LOAD THE INSTANCE
    if(std::filesystem::exists(_instanceFilePath)) {

    } else {
        _instanceIsReady = false;
    }
}

