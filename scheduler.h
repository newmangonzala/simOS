#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "rapidxml.hpp"

#include "linkedlist.h"

#include "pcb.h"


//Process scheduler
//multilevel Feedback Queue
class Sched{

    
    public:
    
    Sched(List<PrBkCtr*>& ,mem&);
    //Sched();

    int qtime = 20; //20 milliseconds

    List<PrBkCtr*>* queue1;   //round robin //this is the ready queue
    //List<int> queue2;   //round robin
    //List<int> queue3;   //first come first serve

    mem* M1;

    //move pcb running to back of the queue 
    void updateQ();

    //FIX or DELETE
    int manage();

    //runs the processes in READY
    void running();
    void running2();
    void fork(PrBkCtr*);
};




#endif