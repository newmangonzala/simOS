#ifndef SCHEDULER_H
#define SCHEDULER_H


#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "linkedlist.h"
#include "timer.h"
#include "rapidxml.hpp"

//Here there will be the scheduler and dispacher


class Sched{

    //multilevel Feedback Queue
    
    public:
    
    Sched();

    //timer time1;

    List<int> queue1;   //round robin
    List<int> queue2;   //round robin
    List<int> queue3;   //first come first serve

    int manage();
    rapidxml::xml_node<>* getApp(int, int);
    
    void maintanance();
};




#endif