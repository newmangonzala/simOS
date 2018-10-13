#ifndef MEMORY_H
#define MEMORY_H


#include <iostream>
#include <string>
#include <vector>

#include "pcb.h"
#include "rapidxml.hpp"

using namespace std;

//2048 MB of memory
//2048MB = 512mwords

class mem{

    private:
        //
    public:

    mem(int);
    struct instrucion{
        std::string type;
        int time;

    };

    int numProcess;
    vector<mem::instrucion> loadApps(); //this is the loader
    vector<vector<mem::instrucion> > memory1;
};

#endif