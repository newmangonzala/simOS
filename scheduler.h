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
//multilevel Feedback Queue
class Sched{

    
    
    public:
    
    Sched(List<PrBkCtr>&);
    //Sched();

    int qtime = 20;

    List<PrBkCtr>* queue1;   //round robin //this is the ready queue
    //List<int> queue2;   //round robin
    //List<int> queue3;   //first come first serve



    void updateQ();

    int manage();
 
    void running();
};




#endif