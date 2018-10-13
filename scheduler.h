#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "rapidxml.hpp"

#include "linkedlist.h"

//#include "memory.h"
#include "pcb.h"


//Here there will be the scheduler

class Sched{

    //multilevel Feedback Queue
    
    public:
    
    Sched(List<PrBkCtr>&);
    //Sched();

    //timer time1;

    List<PrBkCtr>* queue1;   //round robin //this is the ready queue
    //List<int> queue2;   //round robin
    //List<int> queue3;   //first come first serve



    int manage();
 
    void maintanance();
};




#endif