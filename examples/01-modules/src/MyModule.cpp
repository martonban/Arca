#include "MyModule.hpp"


MyModule::MyModule() {
    Arca::ModuleConfig config = Arca::GetModule("TestModule");
    arcaModule = std::make_shared<Arca::Module>(config);
    arcaModule->AddAccessPoint("Test", "C:\\Learn");
    arcaModule->Save();
}

void MyModule::Print() {
    std::cout << arcaModule->GetAccessPoint("Test");
}