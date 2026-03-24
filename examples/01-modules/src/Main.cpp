#include "Arca.hpp"
#include "MyModule.hpp"
#include <iostream>

int main() {

	Arca::InitArcaInstance("Arca Module");
    
    if(Arca::IsArcaNew()) {
        // Base Arca
        Arca::ApplicationMetaData metaData {"ArcaModule", "Márton Bán", "v.0.0.1"};
        Arca::AddMetaData(metaData);

        Arca::ModuleConfig coreModuleConfig {
            "TestModule",
            std::filesystem::path {},
            Arca::CORE_TYPE,
            Arca::READY_FOR_PROCESSING
        };

        Arca::ProcessModuleConfig(coreModuleConfig);
        Arca::AddModule(coreModuleConfig);
        Arca::Build();
    }

    Arca::Test();

    MyModule module {};
    module.Print();

    std::string input;
    std::cin >> input;
}