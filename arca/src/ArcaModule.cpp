#include "ArcaModule.hpp"
#include "Arca.hpp"

Arca::Module::Module(const Arca::ModuleConfig& config) {
    switch (config.status) {
    case READY_FOR_PROCESSING:
        ProcessModuleConfig(config);
        break;
    case PROCESSED:
        if(!Arca::ArcaIO::IsFileExists(config.moduelPath)) {
            Load(config);
            Save();
        } else {
            Load(config);
        }
        
        break;
    default:
        break;
    }
}

void Arca::Module::Save() {
    nlohmann::json serializedFile {
        {"ModuleMetaData", MetadataSerilaization()},
        {"AccesPionts", AccessPointSerilaization()}
    };

     std::ofstream outFile(_path);
    if (outFile.is_open()) {
        outFile << serializedFile.dump(4);
        outFile.close();
    } else {
        std::cerr << "Error: Could not open file for writing: " << _path << std::endl;
    }
}

void Arca::Module::Load(const Arca::ModuleConfig& config) {
    _path = config.moduelPath;
    _status = config.status;
    _name = config.moduleName;
    if(config.status == PROCESSED) {
        std::ifstream rawFile(config.moduelPath);
        if(!rawFile.is_open()) {
            std::cerr << "Error: Arca Module is not exist!" << std::endl;
        }

        nlohmann::json jsonObject;
        rawFile >> jsonObject;

        MetaDataDeserilaiazation(jsonObject);
        _accessPoints = DeserializeAccessPoints(jsonObject);
    }
}


bool Arca::Module::AddAccessPoint(const std::string& key, const std::filesystem::path& path) {
    if(_status == PROCESSED) {
       if(_accessPoints.find(key) != _accessPoints.end()) {
            return false;
        } else {
            _accessPoints[key] = path;
            return true;
        }
    } else {
        return false;
    }
}


std::filesystem::path Arca::Module::GetAccessPoint(const std::string& key) {
    if(_status == PROCESSED) {
        if(_accessPoints.find(key) != _accessPoints.end()) {
            return _accessPoints[key];
        } else {
            return std::filesystem::path {};
        }
    } else {
        return std::filesystem::path {};
    }
}


bool Arca::Module::DeleteAccessPoint(const std::string& key) {
    if(_status == PROCESSED) {
        if(_accessPoints.find(key) != _accessPoints.end()) {
            _accessPoints.erase(key);
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

std::vector<std::pair<std::string, std::filesystem::path>> Arca::Module::GetAllAccessPoint() {
    std::vector<std::pair<std::string, std::filesystem::path>> result;
    if(_status == PROCESSED && _accessPoints.size() != 0) {
        for(auto const& x : _accessPoints) {
            result.push_back({x.first, x.second});
        }

    } else {
        std::cerr << "Error: Module is not ready for this operation or access point map is empty!" << std::endl; 
    }

    return result;
}

void Arca::Module::ProcessModuleConfig(const Arca::ModuleConfig& config) {
    _name = config.moduleName;

    switch (config.type)
    {
    case CORE_TYPE:
        if(Arca::ArcaIO::CreateFolder(Arca::GetArcaInstancePath(), _name)) {
            _path = Arca::GetApplicationPath() / "ArcaFiles" / (_name + ".json");
            _status = PROCESSED;
             Save();
        } else {
            std::cerr << "ERROR: " << std::endl;
        }
        break;
    case EXTERNAL_TYPE:
        if(Arca::ArcaIO::CreateFolder(config.moduelPath, _name)) {
            _path = config.moduelPath / _name / (_name + ".json");
            _status = PROCESSED;
             Save();
        } else {
            std::cerr << "ERROR: " << std::endl;
        }
        break;
    case EXTERNAL_FREEDOM_TYPE:
        if(Arca::ArcaIO::IsFolderExists(config.moduelPath)) {
            _path = config.moduelPath / (_name + ".json");
            _status = PROCESSED;
             Save();
        } else {
            std::cerr << "ERROR: " << std::endl;
        }
    default:
        std::cerr << "ERROR: " << std::endl;
        break;
    }
}

nlohmann::json Arca::Module::MetadataSerilaization() {
        if(_status == PROCESSED) {
        return nlohmann::json {
            {"ModuleName", _name},
            {"Type", _type}
        }; 

    } else {
        std::cerr << "Error: Arca Module is not ready for serilaization!" << std::endl;
        return nlohmann::json();
    }
}

nlohmann::json Arca::Module::AccessPointSerilaization() {
    if(_status == PROCESSED) {
        nlohmann::json result = nlohmann::json::object();
        for (const auto& [key, path] : _accessPoints) {
            result[key] = path.string();
        }
        return result;
    } else {
        std::cerr << "Error: Arca Module is not ready for serilaization!" << std::endl;
        return nlohmann::json();
    }
}

void Arca::Module::MetaDataDeserilaiazation(nlohmann::json deserializedJson) {
    if(deserializedJson.contains("ModuleMetaData")) {
        nlohmann::json j = deserializedJson["ModuleMetaData"];
        _name = j["ModuleName"];
        _type = j["Type"];
    } else {
        std::cerr << "Error: Module metadata is null" << std::endl;
    }
}


std::map<std::string, std::filesystem::path> Arca::Module::DeserializeAccessPoints(nlohmann::json deserializedJson) {
    std::map<std::string, std::filesystem::path> result;
    if(deserializedJson.contains("AccesPionts") && deserializedJson["AccesPionts"].is_object()) {
        for (const auto& [key, value] : deserializedJson["AccesPionts"].items()) {
            if (value.is_string()) {
                result[key] = std::filesystem::path(value.get<std::string>());
            }
        }
    } else {
        std::cerr << "Error: Access points are missing or invalid" << std::endl;
    }

    return result;
}