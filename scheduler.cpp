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

void Sched::updateQ(){

    List<PrBkCtr>::node* head = queue1->getHead();
    List<PrBkCtr>::node* prev = head->next;
    head->next = NULL;
    queue1->tail->next = head;
    queue1->head = prev;
    queue1->tail = head;

/*
    cout << "checking tail:" << endl;

    PrBkCtr* w = &queue1->tail->data;
    List<mem::instrucion>::node* r = w->PC;
    mem::instrucion* b = &r->data;
    cout << b->type << endl;
 
    cout << "done checking:" << endl;


    cout << "checking head:" << endl;

    w = &queue1->head->data;
    r = w->PC;
    b = &r->data;
    cout << b->type << endl;
 
    cout << "done checking:" << endl;
 */   
 /*
    PrBkCtr* w = &queue1->head->data;
    List<mem::instrucion>::node* r = w->PC;
    mem::instrucion* b = &r->data;
    cout << b->type << endl;
  */  
    
    return;
}



void Sched::running(){
    



    //while(!queue1->isEmpty()){
        



        //std::chrono::duration<double,std::milli> dif;
        //auto start = std::chrono::system_clock::now(); 
        //auto end = std::chrono::system_clock::now(); 
        //dif = end - start;


    for(int x = 0; x < 13; x++){



        List<PrBkCtr>::node* head = queue1->getHead();
        PrBkCtr* w = &head->data;
        List<mem::instrucion>::node* r = w->PC;
        mem::instrucion* b = &r->data;
        int duration = b->time;
        cout << "current process: "<< b->type << " has a duration of: " << duration << endl;
        
        if((duration - qtime) <= 0){
            //wait for qtime - duration
            std::this_thread::sleep_for(std::chrono::microseconds(duration));
            b->time = 0;
            cout << "current process: "<< b->type << " has been terminated" << endl;
            
            //check if instruction was the last from process
            //b = &r->next->data;
            //string yield = "\"yield\"";
            string c = (r->next->data).type;
            if(c.compare("\"yield\"") == 0){
                //cout << " //// checking if yield "<< c << endl;
            }
            w->PC = r->next;

        }
        else{
            //wait for qtime
            std::this_thread::sleep_for(std::chrono::milliseconds(qtime));
            b->time = duration - qtime;
        }
        updateQ();
        

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
 