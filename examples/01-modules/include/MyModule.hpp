#ifndef MY_MODULE_HPP
#define MY_MODULE_HPP

#include "Arca.hpp"
#include <memory>

class MyModule {
    public:
        MyModule();
    
    private:
        std::shared_ptr<Arca::Module> arcaModule; 
};


#endif