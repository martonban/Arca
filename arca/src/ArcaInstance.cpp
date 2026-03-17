#include "ArcaInstance.hpp"

void ArcaInstance::StartArcaInstance(const std::string& applicationName) {
    _instanceFolderPath = std::filesystem::path("./");
    _instanceFilePath = _instanceFolderPath / "ArcaFiles" / (applicationName + ".json");
    
    // BUILD OR LOAD THE INSTANCE
    if(std::filesystem::exists(_instanceFilePath)) {
        _instanceIsReady = true;
        FetchInstance();
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
    std::ifstream rawFile(_instanceFilePath);
    if(!rawFile.is_open()) {
        std::cerr << "Error: ArcaInstance is not exists!" << std::endl; 
    }

    nlohmann::json json;
    rawFile >> json;

    _metadata = MetadataDeserilaization(json);

}


std::filesystem::path ArcaInstance::GetApplicationPath() {
    return _instanceFolderPath;
}

std::filesystem::path ArcaInstance::GetInstancePath() {
    return _instanceFolderPath / "ArcaFiles";
}

void ArcaInstance::Test() {
    std::cout << "Arca Test" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << _metadata.instanceName << std::endl;
    std::cout << _metadata.creatorName << std::endl;
    std::cout << _metadata.version << std::endl;
    std::cout << "--------------------------------------" << std::endl;
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

Arca::ApplicationMetaData ArcaInstance::MetadataDeserilaization(const nlohmann::json& jsonObject) {
    if(jsonObject.contains("ApplicationMetadata")) {
        nlohmann::json j = jsonObject["ApplicationMetadata"]; 
        return Arca::ApplicationMetaData {
            j["ApplicationName"].get<std::string>(),
            j["Creator"].get<std::string>(),
            j["Version"].get<std::string>() 
        };
    } else {
        std::cerr << "Error: Metadata is null at the Arca Instance file" << std::endl;
        return Arca::ApplicationMetaData {};
    }
   
}