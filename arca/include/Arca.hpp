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

#include "ArcaInstance.hpp"
#include "ArcaSettings.hpp"

namespace Arca {
    /**
     * @brief Create the singleton Arca instance class
     * @details This function builds the path to the ArcaInstance.json file. After calling it, 
     * you should attempt to load all existing data or create a new Arca instance from scratch.
     * @param applicationName The name of the application to be used within Arca
     * @return No return value
     */
    void InitArcaInstance(const std::string& applicationName) {
        ArcaInstance::GetInstance().StartArcaInstance(applicationName);
    }

    bool IsAlive() {
        return ArcaInstance::GetInstance().IsArcaInstanceAlive();
    }


    void AddMetaData(const Arca::ApplicationMetaData& metaDataStruct) {
        ArcaInstance::GetInstance().AddApplicationMetadata(metaDataStruct);
    }

    void BuildInstance() {
        ArcaInstance::GetInstance().Build();
    }


    void FetchArcaInstance() {
        ArcaInstance::GetInstance().FetchInstance();
    }

    std::filesystem::path GetApplicationPath() {
        return ArcaInstance::GetInstance().GetApplicationPath();
    }

    std::filesystem::path GetArcaInstancePath() {
        return ArcaInstance::GetInstance().GetInstancePath();
    }
}

#endif