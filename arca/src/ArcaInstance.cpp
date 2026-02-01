#include "ArcaInstance.hpp"

void ArcaInstance::StartArcaInstance(const std::string& applicationName) {
    mApplicationName = applicationName;
    mInstanceFolderPath = std::filesystem::path("./");
    mInstanceFilePath = mInstanceFolderPath / "ArcaFiles" / (applicationName + ".json");
}

void ArcaInstance::CreateArcaInstance() {
    // Directory has been created
    if(CreateFolder(mInstanceFolderPath, "ArcaFiles")) {
        std::cout << "Arca Instance directory has been created" << std::endl;
    } else {
        std::cout << "Arca Instance directory cannot able to create" << std::endl;
    }
}

void ArcaInstance::AddCreator(const std::string& creatorName) {
    mApplicationCreator = creatorName;
}

void ArcaInstance::BuildArcaInstance() {
    for(const auto& path : mModulePathContainer) {
        ArcaModule module {path};
        auto m = std::make_shared<ArcaModule>(module);
        std::string key = m->GetName();
        mModuleMap[key] = m;
    }
}

bool ArcaInstance::InstanceSerialize() {
    nlohmann::json instanceJson = Save();
    std::ofstream file(mInstanceFilePath);
    if (!file.is_open()) {
        return false;
    }
    file << instanceJson.dump(4);
    file.close();
    return true;
}

nlohmann::json ArcaInstance::Save() {
    nlohmann::json json;
    json["ApplicationName"] = mApplicationName;
    json["ApplicationCreator"] = mApplicationCreator;
    
    // Serialize module path container
    nlohmann::json modulePaths = nlohmann::json::array();
    for (const auto& path : mModulePathContainer) {
        modulePaths.push_back(path.string());
    }
    json["ModulePaths"] = modulePaths;

    return json;
}

bool ArcaInstance::InstanceDeserialize() {
    std::ifstream file(mInstanceFilePath);
    if(file.is_open()) {
        nlohmann::json instanceJson;
        file >> instanceJson;
        file.close();
        
        if(Load(instanceJson)) {
            return true;
        } else {
            std::cerr << "Error::ArcaInstance.cpp: mModulePathContainer issue is accured during the deserialization" << std::endl;
            return false;
        }
    
    } else {
        std::clog<< "Warnirng::ArcaInstance.cpp: " << mInstanceFilePath << " is not exists, please create Instance from scratch, or give a valid file" <<  std::endl;
        return false;
    }

}

bool ArcaInstance::Load(const nlohmann::json& json) {
    if(const auto it = json.find("ApplicationName"); it != json.end() && it->is_string()) {
        mApplicationName = it->get<std::string>();
    }

    if(const auto it = json.find("ApplicationCreator"); it != json.end() && it->is_string()) {
        mApplicationCreator = it->get<std::string>();
    }

    if (json.contains("ModulePaths")) {
        mModulePathContainer.clear(); // Clear existing paths
        for (const auto& path : json["ModulePaths"]) {
            if (path.is_string()) {
                try {
                    mModulePathContainer.emplace_back(std::filesystem::path(path.get<std::string>()));
                } catch(const std::exception& e) {
                    std::cerr << "Error::ArcaInstance" << e.what() << std::endl;
                }
                
            }
        }
    }
    return true;
}

void ArcaInstance::ArcaTest() {
    std::cout << mApplicationName << std::endl;
    std::cout << mApplicationCreator << std::endl;
    for(const auto& path : mModulePathContainer) {
        std::cout << path << std::endl;
    }
}

inline std::filesystem::path GetArcaInstanceFolderPath() {
    return ArcaInstance::GetInstance().GetArcaInstanceFolderPath();
}

void ArcaInstance::CreateModule(ArcaModule& module) {
    std::string key = module.GetName();
    mModuleMap[key] = std::make_shared<ArcaModule>(module);
}

void ArcaInstance::CreateModule(const std::string& moduleName) {
    // Create Module Instance 
    ArcaModule module { mInstanceFilePath.parent_path(), moduleName };
    mModulePathContainer.push_back(module.GetFilePath());
    mModuleMap[moduleName] = std::make_shared<ArcaModule>(module);
}

void ArcaInstance::CreateModule(const std::filesystem::path& fullFilePath, const std::string& name) {
    ArcaModule module { fullFilePath, name };
    mModulePathContainer.push_back(module.GetFilePath());
    mModuleMap[name] = std::make_shared<ArcaModule>(module);
}



std::shared_ptr<ArcaModule> ArcaInstance::GetModule(const std::string& name) {
    return mModuleMap[name];
}


bool ArcaInstance::IsFileExists(const std::filesystem::path& fullFilePath) {
    return mArcaIO.IsFileExists(fullFilePath);
}

bool ArcaInstance::CreateFolder(const std::filesystem::path& path, const std::string& folderName) {
    return mArcaIO.CreateFolder(path, folderName);
}

bool ArcaInstance::InstanceBuilder(const std::filesystem::path& path) {
    return true;
}
