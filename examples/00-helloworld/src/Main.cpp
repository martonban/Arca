#include "Arca.hpp"

int main() {

	Arca::InitArcaInstance("HelloArca");
    
    if(Arca::IsArcaNew()) {
        Arca::ApplicationMetaData metaData {"HelloArca", "Márton Bán", "v.0.0.1"};
        Arca::AddMetaData(metaData);
        Arca::Build();
    }

    Arca::Test();

    std::string input;
    std::cin >> input;

	return 0;
}