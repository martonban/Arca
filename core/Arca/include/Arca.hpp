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

//-----------------------------------------------------------------------------
//                                  ARCA Structs
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//                                      ARCA
//-----------------------------------------------------------------------------
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

    /**
     * @brief This function will going to fetch all data from the instance serialzation file.
     * @details With this function the libary will deserialize all the data.
     * @warning Modules and other instances will not be live. This function just fetch the relative paths to those, you have to build them spereatly 
     * @return A bool value based on the file existance.
    */
    inline bool FetchArcaInstanceData() {
        return ArcaInstance::GetInstance().InstanceDeserialize();
    }
   
    /**
     * @brief This function will trigger all the Arca components based on the ArcaInstance file
     * @param 
     * @return
    */
    inline bool BuildArcaInstance() {
        ArcaInstance::GetInstance().BuildArcaInstance();
        return true;
    }

    /**
     * @brief This function will create the folder structure for ArcaFiles
     * @param 
     * @return void
    */
    inline void CreateArcaInstance() {
        return ArcaInstance::GetInstance().CreateArcaInstance();
    }
    
    /**
     * @brief Add extra metadata for the application
     * @param creatorName an std::string for the creators
     * @return void
    */
    inline void AddCreator(const std::string& creatorName) {
        ArcaInstance::GetInstance().AddCreator(creatorName);
    }

    /**
     * @brief Once all data has been received, the Arca Instance saves its state and generates the metadata files.
     * @param 
     * @return
    */
    inline bool ReleaseArcaInstance() {
        return ArcaInstance::GetInstance().InstanceSerialize();
    }


    inline void ArcaTest() {
        ArcaInstance::GetInstance().ArcaTest();
    }

    inline std::filesystem::path GetArcaInstanceFolderPath() {
        return ArcaInstance::GetInstance().GetArcaInstanceFolderPath();
    }

    //-----------------------------------------------------------------------------
    //                                  ARCA Module
    //----------------------------------------------------------------------------- 
    /**
     * @brief Create an Arca Module for the deveoper's modules
     * @param name Name of the module
     * @return True if exists, False if not
    */
    inline void CreateModule(const std::string& name) {
        return ArcaInstance::GetInstance().CreateModule(name);
    }

    /**
     * @brief Create an Arca Module - For user generated projects
     * @param name Name of the module
     * @param fullFilePath an std::filesystem::path absolute  
     * @return 
    */
    inline void CreateModule(const std::filesystem::path& fullFilePath, const std::string& name) {
        return ArcaInstance::GetInstance().CreateModule(fullFilePath, name);
    }

    inline std::shared_ptr<ArcaModule> GetArcaModule(const std::string& key) {
        return ArcaInstance::GetInstance().GetModule(key);
    }
    
    //-----------------------------------------------------------------------------
    //                               SIMPLE I/O UTILS  
    //-----------------------------------------------------------------------------    
    /**
     * @brief Check if this file exist on your system or not
     * @param fullFilePath an std::string for the file path 
     * @return True if exists, False if not
    */
    inline bool IsFileExists(const std::filesystem::path& fullFilePath) {
        return ArcaInstance::GetInstance().IsFileExists(fullFilePath);
    }

    /**
     * @brief This function will create a directory on your system
     * @param path Absolute path for the new folder
     * @param folderName Name of the new folder name
     * @return True if it gets created. False if it does not
    */
    inline bool CreateFolder(const std::filesystem::path& path, const std::string& folderName) {
        return ArcaInstance::GetInstance().CreateFolder(path, folderName);
    }
}

//-----------------------------------------------------------------------------
//                                 ARCA Scene Describer
//-----------------------------------------------------------------------------




//-----------------------------------------------------------------------------
//                                    ARCA I/O
//-----------------------------------------------------------------------------



#endif