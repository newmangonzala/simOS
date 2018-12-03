
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

class PrBkCtr;

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
    void linker();     //this is the loader

  
    vector<List<mem::instrucion>> qOfPr;
    vector<List<std::string>> listOfProcesses;
    vector<List<std::string>> listOfProcessesChilds;
    vector<List<std::string>> listOfProcessesChilds2;



    
    short int** mainMem = new short int*[numFrames];   //128 pages of 16kb
    PrBkCtr* frameTable[numFrames];
    short int** TLB = new short int*[maxNumPages];


    queue<short int> freeFrames;

    short int currentFrameIndex;



    void mmu(PrBkCtr*, vector<int>);
    vector<int> findPages(string);
    bool lookUpTLB(int, int);
    bool loopUpPageTlb(PrBkCtr*, int);
    void pageSwapper(PrBkCtr* , int);
    void resetEntryTLB(vector<int>);
    void releaseFrames(PrBkCtr* , int);

    //tuple<int, PrBkCtr*> processTable;


    void createHD();

};





#endif