#ifndef MEMORY_H
#define MEMORY_H


#include <iostream>
#include <string>
#include <vector>
#include <deque>
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

    struct createMailbox{
        deque<string> messages;
        int id;
    };
    
    unordered_map<int, deque<string>*> mailboxes;

};

#endif