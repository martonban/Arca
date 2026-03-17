#include "ArcaInstance.hpp"

void ArcaInstance::StartArcaInstance(const std::string& applicationName) {
    _instanceFolderPath = std::filesystem::path("./");
    _instanceFilePath = _instanceFolderPath / "ArcaFiles" / (applicationName + ".json");
    
    // BUILD OR LOAD THE INSTANCE
    if(std::filesystem::exists(_instanceFilePath)) {
        _instanceIsReady = true;
    } else {
        _instanceIsReady = false;
    }
}

void ArcaInstance::AddApplicationMetadata(const Arca::ApplicationMetaData& metadataStruct) {
    _metadata = metadataStruct;
}

void ArcaInstance::Build() {
    Arca::ArcaIO::CreateFolder(_instanceFolderPath, "ArcaFiles");
    nlohmann::json serializedFile {
        {"ApplicationMetadata", MetadataSerilaization()}
    };

    std::ofstream outFile(_instanceFilePath);
    if (outFile.is_open()) {
        outFile << serializedFile.dump(4);
        outFile.close();
    } else {
        std::cerr << "Error: Could not open file for writing: " << _instanceFilePath << std::endl;
    }
}

void ArcaInstance::FetchInstance() {
    
}


std::filesystem::path ArcaInstance::GetApplicationPath() {
    return _instanceFolderPath;
}

std::filesystem::path ArcaInstance::GetInstancePath() {
    return _instanceFolderPath / "ArcaFiles";
}



bool ArcaInstance::IsArcaInstanceAlive() {
    return _instanceIsReady;
}

nlohmann::json ArcaInstance::MetadataSerilaization() {
    if(_metadata.creatorName.empty() && _metadata.instanceName.empty() && _metadata.version.empty()) {
        std::cerr << "Error: Arca Instance metadata struct is not defined!" << std::endl;
        return nlohmann::json();
    } else {
        return nlohmann::json {
            {"ApplicationName", _metadata.instanceName},
            {"Creator", _metadata.creatorName},
            {"Version", _metadata.version}
        }; 
    }

}