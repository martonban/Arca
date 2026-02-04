#include "Arca.hpp"

int main() {
	Arca::InitArcaInstance("RenderLamp");

    // BUILDING ARCA INSTANCE
    if (Arca::FetchArcaInstanceData()) {
        Arca::BuildArcaInstance();
        std::cout << "Arca instance has been de-serialized and build!" << std::endl;
    }
    else {
        Arca::CreateArcaInstance();
        Arca::AddCreator("Márton Bán");
        Arca::CreateModule("Editor");
        Arca::CreateModule("Renderer");
        if(Arca::ReleaseArcaInstance()) {
            std::cout << "Arca Instance has been created!" << std::endl;
        }   
    }

	return 0;
}