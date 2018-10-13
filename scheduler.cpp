#include "scheduler.h"
//#include "rapidxml.hpp"
//#include <iostream>





using namespace rapidxml;


Sched::Sched(List<PrBkCtr>& Q1){ queue1 = &Q1; }
//Sched::Sched(){}

int Sched::manage(){
    int current;
    
    //std::cout << (queue1.getHead()->data).PID << std::endl;

    //while(!queue1.isEmpty()){
        
        //std::chrono::duration<double,std::milli> dif;
        //auto start = std::chrono::system_clock::now(); 
        //auto end = std::chrono::system_clock::now(); 
        //dif = end - start;

        //current = queue1.getHead();
        return 1;
    //}
    /*
    else if(!queue2.isEmpty()) {
        current = queue2.getReference();
        return current;
    }
    else if(!queue3.isEmpty()) {
        current = queue3.getReference();
        return current;
    }
    */
    
}

void Sched::maintanance(){
    
}
/*
    xml_document<> doctmp;
    xml_node<> *node = doctmp.allocate_node(node_element, "p", "Google");
    doctmp.append_node(node);
    xml_attribute<> *attr = doctmp.allocate_attribute("href", "google.com");
    node->append_attribute(attr);

    //std::cout << node->value() << endl;
*/
 