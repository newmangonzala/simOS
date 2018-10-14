#include "scheduler.h"
//#include "rapidxml.hpp"
#include <thread> 





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


void foo() {
  for(int i = 0; i < 1000000; i++){

      std::cout << "-";
  }
    return;
}

void bar(){
    for(int i = 0; i < 1000000; i++){

      std::cout << "?";
    }
    return;
}

void Sched::running(){
    



    //while(!queue1->isEmpty()){
        



        //std::chrono::duration<double,std::milli> dif;
        //auto start = std::chrono::system_clock::now(); 
        //auto end = std::chrono::system_clock::now(); 
        //dif = end - start;

        List<PrBkCtr>::node* head = queue1->getHead();
        PrBkCtr* w = &head->data;
        List<mem::instrucion>::node* r = w->PC;
        mem::instrucion* b = &r->data;
        int duration = b->time;
        cout << "duration of the instruction from the current process: "<< duration << endl;
        cout << b->type << endl;
        if((duration - qtime) <= 0){
            //wait for qtime - duration
            std::this_thread::sleep_for(std::chrono::seconds(duration));
            b->time = 0;
        }
        else{
            //wait for qtime
            std::this_thread::sleep_for(std::chrono::seconds(qtime));
            b->time = duration - qtime;
        }
        /*
        head = queue1->getHead();
        w = &head->data;
        r = w->PC;
        b = &r->data;
        cout << "testing : "<< b->time << endl;
        */

        //cout << w.PID << endl;

        //cout << "pcb state: " << w.state << endl;

    return;

}
 