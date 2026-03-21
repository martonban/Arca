#include "MyModule.hpp"

MyModule::MyModule() {
    Arca::ModuleConfig moduleConfig = Arca::GetModule("TestModule");
    arcaModule = std::make_shared<Arca::Module>(moduleConfig);
}