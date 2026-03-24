#include "ArcaInstance.hpp"

void ArcaInstance::StartArcaInstance(const std::string& applicationName) {
    std::string tmpName = StringToUpperCamelCase(applicationName);
    _applicationFolderPath = std::filesystem::absolute(std::filesystem::path("./"));
    _instanceFolderPath = std::filesystem::absolute(_applicationFolderPath / "ArcaFiles");
    _instanceFilePath = std::filesystem::absolute(_instanceFolderPath / (tmpName + ".json"));

    _arcaInstanceStatus = Arca::FIRST_LAUNCH;
    
    if(Arca::ArcaIO::IsFolderExists(_instanceFolderPath)) {
        _arcaInstanceStatus = Arca::ARCA_FILES_DIRECTORY_CREATED;
    }
    
    if (IsInstaceFileValid()) {
        _arcaInstanceStatus = Arca::ARCA_INSTANCE_FILE_IS_VALID;
    }
    
    switch (_arcaInstanceStatus) {
    case Arca::FIRST_LAUNCH:
        CreateFolderStruct();
        break;
    case Arca::ARCA_FILES_DIRECTORY_CREATED:
        std::cerr << "Error: ArcaInstance file is currupted!" << std::endl;
        break;
    case Arca::ARCA_INSTANCE_FILE_IS_VALID:
        FetchInstance();
        break;
    default:
        break;
    }
}

void ArcaInstance::FetchInstance() {
    if(IsArcaInstanceReadyToLoad()) {
        std::ifstream rawFile(_instanceFilePath);
        if(!rawFile.is_open()) {
            std::cerr << "Error: ArcaInstance is not exists!" << std::endl; 
        }

        nlohmann::json json;
        rawFile >> json;

        _metadata = MetadataDeserilaization(json);
        _moduleMap = ModulesDeserialiazation(json);

        // TO-DO VALIDATION SATGE


        _arcaInstanceStatus = Arca::ARCA_IS_READY;
    } else {
        std::cerr << "Error: Arca Instance is not ready to load" << std::endl;
    }
}

void ArcaInstance::AddApplicationMetadata(const Arca::ApplicationMetaData& metadataStruct) {
    if(IsArcaInstanceReadyToSave()) {
        if(IsMetadataValid(metadataStruct)) {
            _metadata = metadataStruct;
        } else {
            std::cerr << "Error: Application Metadata is invalid. Instance will going to be created. Expect some issues!" << std::endl;
        } 
    } else {
        std::cerr << "Error: Please use 'InitArcaInstance(std::string name)' function before calling this function' " << std::endl;
    }  
}

void ArcaInstance::Build() {
    nlohmann::json serializedFile {
        {"ApplicationMetadata", MetadataSerilaization()},
        {"InstanceModules", ModulesSerialization()}
    };

    std::ofstream outFile(_instanceFilePath);
    if (outFile.is_open()) {
        outFile << serializedFile.dump(4);
        outFile.close();
    } else {
        std::cerr << "Error: Could not open file for writing: " << _instanceFilePath << std::endl;
    }
}

bool ArcaInstance::AddModule(const Arca::ModuleConfig& moduleConfig) {
    if(IsModuleConfigValid(moduleConfig)) {
        std::string name = moduleConfig.moduleName;
        if(_moduleMap.find(name) != _moduleMap.end()) {
            return false;
        } else {
            _moduleMap[name] = moduleConfig;
            return true;
        }
    } else {
        std::cerr << "Error: " << std::endl;
        return false;
    }
    
}

Arca::ModuleConfig ArcaInstance::GetModule(const std::string& moduleName) {
    if(_moduleMap.find(moduleName) != _moduleMap.end()) {
        return _moduleMap[moduleName];
    } else {
        return Arca::ModuleConfig {};
    }
}

void ArcaInstance::ProcessModuleConfig(Arca::ModuleConfig& config) {
    std::filesystem::path tmpPath = config.modulePath;
    std::string tmpName = StringToUpperCamelCase(config.moduleName);
    switch (config.type)
    {
    case Arca::CORE_TYPE:
        if(Arca::ArcaIO::CreateFolder(std::filesystem::absolute(GetApplicationPath() / "ArcaFiles"), tmpName)) {
            config.modulePath = GetApplicationPath() / "ArcaFiles" / tmpName / (tmpName + ".json");
            config.status = Arca::PROCESSED;
        } else {
            std::cerr << "ERROR: " << std::endl;
        }
        break;
    case Arca::EXTERNAL_TYPE:
        if(Arca::ArcaIO::CreateFolder(tmpPath, tmpName)) {
            config.modulePath = tmpPath / tmpName/ (tmpName + ".json");
            config.status  = Arca::PROCESSED;

        } else {
            std::cerr << "ERROR: " << std::endl;
        }
        break;
    case Arca::EXTERNAL_FREEDOM_TYPE:
        if(Arca::ArcaIO::IsFolderExists(tmpPath)) {
            config.modulePath = tmpPath / (tmpName + ".json");
            config.status = Arca::PROCESSED;
        } else {
            std::cerr << "ERROR: " << std::endl;
        }
    default:
        std::cerr << "ERROR: " << std::endl;
        break;
    }
}

void ArcaInstance::Test() {
    std::cout << "Arca Test" << std::endl;
    std::cout << "--------------------------------------" << std::endl;
    std::cout << _metadata.instanceName << std::endl;
    std::cout << _metadata.creatorName << std::endl;
    std::cout << _metadata.version << std::endl;
    std::cout << "--------------------------------------" << std::endl;
}

std::filesystem::path ArcaInstance::GetApplicationPath() {
    return _applicationFolderPath;
}

std::filesystem::path ArcaInstance::GetInstanceFolder() {
    return _instanceFolderPath;
}

std::filesystem::path ArcaInstance::GetInstancePath() {
    return _instanceFilePath;
}

bool ArcaInstance::IsArcaNew() {
    if(_arcaInstanceStatus <= Arca::ARCA_FILES_DIRECTORY_CREATED) {
        return true;
    } else {
        return false;
    }
}

bool ArcaInstance::IsArcaInstanceReadyToSave() {
    if(_arcaInstanceStatus >= 1 ) {
        return true;
    } else {
        return false;
    }
}

bool ArcaInstance::IsArcaInstanceReadyToLoad() {
    if(_arcaInstanceStatus >= 1) {
        return true;
    } else {
        return false;
    }
}

void ArcaInstance::CreateFolderStruct() {
    if(Arca::ArcaIO::CreateFolder(_applicationFolderPath, "ArcaFiles")) {
        _arcaInstanceStatus = Arca::ARCA_FILES_DIRECTORY_CREATED;
    } else {
        std::cout << "Error: If this happens something went really really wrong xd" << std::endl;
    }
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

nlohmann::json ArcaInstance::ModulesSerialization() {
    nlohmann::json result = nlohmann::json::object();

    for (const auto& [moduleName, moduleConfig] : _moduleMap) {
        result[moduleName] = {
            {"ModuleName", moduleConfig.moduleName},
            {"ModulePath", moduleConfig.modulePath.string()},
            {"Type", moduleConfig.type},
            {"Status", moduleConfig.status}
        };
    }
    return result;
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

std::map<std::string, Arca::ModuleConfig> ArcaInstance::ModulesDeserialiazation(const nlohmann::json& jsonObject) {
    std::map<std::string, Arca::ModuleConfig> result;

    if (!jsonObject.contains("InstanceModules") || !jsonObject["InstanceModules"].is_object()) {
        std::cerr << "Error: InstanceModules is missing or invalid" << std::endl;
        return result;
    }

    for (const auto& [moduleKey, moduleJson] : jsonObject["InstanceModules"].items()) {
        if (!moduleJson.is_object()) {
            continue;
        }

        if (!moduleJson.contains("ModulePath") || !moduleJson.contains("Type") || !moduleJson.contains("Status")) {
            continue;
        }

        Arca::ModuleConfig config;
        config.moduleName = moduleJson.value("ModuleName", moduleKey);
        config.modulePath = std::filesystem::path(moduleJson["ModulePath"].get<std::string>());
        config.type = static_cast<Arca::ModuleType>(moduleJson["Type"].get<int>());
        config.status = static_cast<Arca::ModuleStatus>(moduleJson["Status"].get<int>());

        result[moduleKey] = config;
    }

    return result;
}

bool ArcaInstance::IsInstaceFileValid() {
    std::ifstream rawFile(_instanceFilePath);
    if(!rawFile.is_open()) {
        return false;
    }
    
    nlohmann::json json;
    try {
        rawFile >> json;
    } catch (const nlohmann::json::parse_error&) {
        return false;
    }

    if(!json.is_object()) {
        return false;
    }

    return json.contains("ApplicationMetadata") && json.contains("InstanceModules");
}



bool ArcaInstance::IsMetadataValid(const Arca::ApplicationMetaData& metaData) {
    if(metaData.creatorName.empty() || metaData.creatorName.empty() || metaData.version.empty() ) {
        std::cerr << "Error: Application metadata is missing requierd values! (like application name, creator's name etc.)" << std::endl; 
        return false;
    } else {
        return true;
    }
}

bool ArcaInstance::IsModuleConfigValid(const Arca::ModuleConfig& config) {
    if(config.moduleName.empty()) {
        return false;
    }

    if(!Arca::ArcaIO::IsFolderExists(config.modulePath.parent_path())) {
        return false;
    }

    if(config.status != Arca::READY_FOR_PROCESSING && config.status != Arca::PROCESSED) {
        return false;
    }

    if(config.type != Arca::CORE_TYPE && config.type != Arca::EXTERNAL_FREEDOM_TYPE && config.type != Arca::EXTERNAL_TYPE) {
        return false;  
    }

    return true;
}

std::string ArcaInstance::StringToUpperCamelCase(const std::string& input) {
    std::string newString;
    bool nextIsUppercase = true;    // First is always be uppercase 
    for(auto it = input.begin(); it != input.end(); ++it ) {
        if(*it == ' ' || *it == '-' || *it == '_') {
            nextIsUppercase = true;
        } else {
            newString += nextIsUppercase ? (char)std::toupper((unsigned char)*it) : *it;
            nextIsUppercase = false;
        }
    }
    return newString;
}