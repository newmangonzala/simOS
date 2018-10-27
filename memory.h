#ifndef MEMORY_H
#define MEMORY_H


#include <iostream>
#include <string>
#include <vector>

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
    void loadApps();     //this is the loader
    void loadPCBs();
    vector<List<mem::instrucion>> qOfPr;
    vector<std::string> memOfProcesses;

};

#endif