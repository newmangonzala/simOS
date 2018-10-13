#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "linkedlist.h"
#include "timer.h"
#include "memory.h"
#include "pcb.h"

#include "rapidxml.hpp"



//Here there will be the scheduler and dispacher


class Sched{

    //multilevel Feedback Queue
    
    public:
    
    
    Sched(List<PrBkCtr>&);

    //timer time1;

    List<PrBkCtr>* queue1;   //round robin //this is the ready queue
    //List<int> queue2;   //round robin
    //List<int> queue3;   //first come first serve



    int manage();
 
    void maintanance();
};




#endif