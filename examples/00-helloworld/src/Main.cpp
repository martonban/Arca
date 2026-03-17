#include "Arca.hpp"

int main() {

	Arca::InitArcaInstance("HelloArca");
    
    if(!Arca::IsExists()) {
        Arca::ApplicationMetaData metaData {"HelloArca", "Márton Bán", "v.0.0.1"};
        Arca::AddMetaData(metaData);
        Arca::BuildInstance();
    }

    Arca::Test();

    std::string input;
    std::cin >> input;

	return 0;
}