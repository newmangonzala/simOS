
#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <list>
#include <unordered_map>

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
    void loadApp(List<std::string>::node*);
    vector<List<mem::instrucion>> qOfPr;
    vector<List<std::string>> memOfProcesses;



    
    short int** mainMem = new short int*[numFrames];   //128 pages of 16kb
    short int** TLB = new short int*[maxNumPages];

    queue<short int> freeFrames;

    short int currentFrameIndex;

    void mmu();

    //tuple<int, PrBkCtr*> processTable;

};



class pageTable{

    public: 

    vector<int*> entries;
    //int** entries = new int*[256];
    
    pageTable(int priority){
        for(int i = 0 ; i < priority*2; i++){
            int * tmp = new int[2];
            tmp[0] = 0; //initialize valid bits to 0
            entries.push_back(tmp);
        }

    }
    

    
      
    
};

#endif