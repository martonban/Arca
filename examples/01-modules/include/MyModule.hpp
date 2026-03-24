#ifndef MY_MODULE_HPP
#define MY_MODULE_HPP

#include "Arca.hpp"
#include <memory>
#include <string>
#include <filesystem>
#include <vector>
#include <utility>

class MyModule {
    public:
        MyModule();
        void Print();
    private:
        std::shared_ptr<Arca::Module> arcaModule; 
};


#endif