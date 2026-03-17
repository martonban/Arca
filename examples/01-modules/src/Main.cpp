#include "Arca.hpp"

int main() {

	Arca::InitArcaInstance("RenderLamp");
    
    if(!Arca::IsExists()) {
        Arca::ApplicationMetaData metaData {"RenderLamp", "Márton Bán", "v.0.0.1"};
        Arca::AddMetaData(metaData);
        Arca::BuildInstance();
    } else {
        Arca::FetchArcaInstance();
    }

    std::string input;
    std::cin >> input;

	return 0;
}