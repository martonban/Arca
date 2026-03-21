//------------------------------------------------------------------------------------------------
//                                              Arca
//                                       Márton Bán (C) 2025
//	
//  Arca is a application manager libary for C++ applications. 
//------------------------------------------------------------------------------------------------

#ifndef ARCA_HPP
#define ARCA_HPP

#include <string>
#include <fstream>
#include <filesystem>
#include <memory> 

#include "ArcaModule.hpp"
#include "ArcaInstance.hpp"
#include "ArcaCommon.hpp"

namespace Arca {
    /**
     * @brief Create the singleton Arca instance class
     * @details This function builds the path to the ArcaInstance.json file. After calling it, 
     * you should attempt to load all existing data or create a new Arca instance from scratch.
     * @param applicationName The name of the application to be used within Arca
     * @return No return value
     */
    inline void InitArcaInstance(const std::string& applicationName) {
        ArcaInstance::GetInstance().StartArcaInstance(applicationName);
    }

    inline bool IsExists() {
        return ArcaInstance::GetInstance().IsArcaInstanceAlive();
    }

    inline void AddMetaData(const Arca::ApplicationMetaData& metaDataStruct) {
        ArcaInstance::GetInstance().AddApplicationMetadata(metaDataStruct);
    }

    inline void BuildInstance() {
        ArcaInstance::GetInstance().Build();
    }

    inline void FetchArcaInstance() {
        ArcaInstance::GetInstance().FetchInstance();
    }

    inline bool AddModule(const Arca::ModuleConfig& moduleConfig) {
        return ArcaInstance::GetInstance().AddModule(moduleConfig);
    }

    inline ModuleConfig GetModule(const std::string& moduleName) {
        return ArcaInstance::GetInstance().GetModule(moduleName);
    }

    inline void ProcessModuleConfig(Arca::ModuleConfig& config) {
        ArcaInstance::GetInstance().ProcessModuleConfig(config);
    }

    inline void Test() {
        ArcaInstance::GetInstance().Test();
    }

    inline std::filesystem::path GetApplicationPath() {
        return ArcaInstance::GetInstance().GetApplicationPath();
    }

    inline std::filesystem::path GetArcaInstancePath() {
        return ArcaInstance::GetInstance().GetInstancePath();
    }
}

#endif