#include "Arca.hpp"

int main() {

	Arca::InitArcaInstance("Hello Arca");
    
    if(!Arca::IsAlive()) {
        Arca::ApplicationMetaData metaData {"Hello Arca", "Márton Bán", "v.0.0.1"};
        Arca::AddMetaData(metaData);
        Arca::BuildInstance();
    } else {
        Arca::FetchArcaInstance();
        Arca::Test();
    }

    std::string input;
    std::cin >> input;

	return 0;
}