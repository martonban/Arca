#include "ArcaInstance.hpp"

void ArcaInstance::StartArcaInstance(const std::string& applicationName) {
    _applicationFolderPath = std::filesystem::path("./");
    _instanceFilePath = _applicationFolderPath / "ArcaFiles" / (applicationName + ".json");
    
    // BUILD OR LOAD THE INSTANCE
    if(std::filesystem::exists(_instanceFilePath)) {
        _instanceIsReady = true;
        FetchInstance();
    } else {
        if (!Arca::ArcaIO::CreateFolder(_applicationFolderPath, "ArcaFiles")) {
            std::cerr << "Error: If you see this message, something went really really bad" << std::endl;
        }
        _instanceIsReady = false;
    }
}

void ArcaInstance::AddApplicationMetadata(const Arca::ApplicationMetaData& metadataStruct) {
    _metadata = metadataStruct;
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
        _instanceIsReady = true;
    } else {
        std::cerr << "Error: Could not open file for writing: " << _instanceFilePath << std::endl;
    }
}

bool ArcaInstance::AddModule(const Arca::ModuleConfig& moduleConfig) {
    std::string name = moduleConfig.moduleName;
    if(_moduleMap.find(name) != _moduleMap.end()) {
        return false;
    } else {
        _moduleMap[name] = moduleConfig;
        return true;
    }
}

Arca::ModuleConfig ArcaInstance::GetModule(const std::string& moduleName) {
    if(IsArcaInstanceAlive()) {
        if(_moduleMap.find(moduleName) != _moduleMap.end()) {
            return _moduleMap[moduleName];
        } else {
            return Arca::ModuleConfig {};
        }
    } else {
        return Arca::ModuleConfig {};
    }
}

void ArcaInstance::ProcessModuleConfig(Arca::ModuleConfig& config) {
    std::filesystem::path tmpPath = config.modulePath;
    switch (config.type)
    {
    case Arca::CORE_TYPE:
        if(Arca::ArcaIO::CreateFolder(std::filesystem::absolute(GetApplicationPath() / "ArcaFiles"), config.moduleName)) {
            config.modulePath = GetApplicationPath() / "ArcaFiles" / config.moduleName / (config.moduleName + ".json");
            config.status = Arca::PROCESSED;
        } else {
            std::cerr << "ERROR: " << std::endl;
        }
        break;
    case Arca::EXTERNAL_TYPE:
        if(Arca::ArcaIO::CreateFolder(tmpPath, config.moduleName)) {
            config.modulePath = tmpPath / config.moduleName / (config.moduleName + ".json");
            config.status  = Arca::PROCESSED;

        } else {
            std::cerr << "ERROR: " << std::endl;
        }
        break;
    case Arca::EXTERNAL_FREEDOM_TYPE:
        if(Arca::ArcaIO::IsFolderExists(tmpPath)) {
            config.modulePath = tmpPath / (config.moduleName + ".json");
            config.status = Arca::PROCESSED;
        } else {
            std::cerr << "ERROR: " << std::endl;
        }
    default:
        std::cerr << "ERROR: " << std::endl;
        break;
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
    _moduleMap = ModulesDeserialiazation(json);
}

std::filesystem::path ArcaInstance::GetApplicationPath() {
    return _applicationFolderPath;
}

std::filesystem::path ArcaInstance::GetInstancePath() {
    return _applicationFolderPath / "ArcaFiles";
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