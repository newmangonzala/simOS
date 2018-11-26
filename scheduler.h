#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <iostream>
#include <string>
#include <sstream> 
#include <queue>
#include <exception>
#include <vector>
#include <fstream>

#include "rapidxml.hpp"

#include "linkedlist.h"

#include "pcb.h"


enum QState{Queue1 , Queue2 , Queue3};

//Process scheduler
//multilevel Feedback Queue
class Sched{

    
    public:
    
    //Sched(List<PrBkCtr*>& ,mem&);
    Sched(DoublyList<PrBkCtr*>& ,DoublyList<PrBkCtr*>& , DoublyList<PrBkCtr*>& ,mem&, ipc&);
    //Sched();

    int qtime = 20; //20 milliseconds
    QState servingQ;

    DoublyList<PrBkCtr*>* queue1;   //round robin //this is the ready queue
    DoublyList<PrBkCtr*>* queue2;   //round robin
    DoublyList<PrBkCtr*>* queue3;   //first come first serve

    mem* M1;
    ipc* MB; //mailboxes

    //move pcb running to back of the queue 
    void updateQ(DoublyList<PrBkCtr *>*);
    void updateQ2();


    void terminatePr(DoublyList<PrBkCtr*>::node*);

    //runs the processes in READY
    void running();
    void runRR(DoublyList<PrBkCtr *>*);
    void runFIFO(DoublyList<PrBkCtr *>*);
    void running2();
    void fork(PrBkCtr*);
    void yield(PrBkCtr*);
    bool checkChilds(PrBkCtr*);
    vector<int> findPages(string);
    string updateTime(string, int);
    
    void mmu(PrBkCtr*, vector<int>);
};




#endif