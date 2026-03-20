#include "Arca.hpp"
#include "MyModule.hpp"

int main() {

	Arca::InitArcaInstance("ArcaModule");
    
    if(!Arca::IsExists()) {
        // Base Arca
        Arca::ApplicationMetaData metaData {"ArcaModule", "Márton Bán", "v.0.0.1"};
        Arca::AddMetaData(metaData);
   
        Arca::BuildInstance();
    }

    Arca::Test();

    MyModule module {};
    

    std::string input;
    std::cin >> input;
}