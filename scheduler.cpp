#include "scheduler.h"
#include <thread> 

Sched::Sched(List<PrBkCtr>& Q1){ queue1 = &Q1; }
//Sched::Sched(){}

int Sched::manage(){
    return 1;
}

//move current process to the back of the queue
void Sched::updateQ(){

    List<PrBkCtr>::node* head = queue1->getHead();
    List<PrBkCtr>::node* tail = queue1->getTail();
    List<PrBkCtr>::node* prev = head->next;
    head->next = NULL;

    if(head != tail){
        tail->next = head;
        
    }
    
    queue1->head = prev;
    queue1->tail = head;
  
 /*
    PrBkCtr* w = &queue1->head->data;
    List<mem::instrucion>::node* r = w->PC;
    mem::instrucion* b = &r->data;
    cout << b->type << endl;
  */  
    
    return;
}


        //std::chrono::duration<double,std::milli> dif;
        //auto start = std::chrono::system_clock::now(); 
        //auto end = std::chrono::system_clock::now(); 
        //dif = end - start;

void Sched::running(){
    
    //run until Ready queue is empty
    while(!queue1->isEmpty()){

        List<PrBkCtr>::node* head = queue1->getHead();
        PrBkCtr* w = &head->data;
        List<mem::instrucion>::node* r = w->PC;
        mem::instrucion* b = &r->data;
        int duration = b->time;
        w->state = RUNNING;     //updating state to running
        cout << "current process: "<< w->PID << " running inst: "  << b->type << "and has a current duration of: " << duration << endl;
        
        if((duration - qtime) <= 0){
            //wait for qtime - duration
            std::this_thread::sleep_for(std::chrono::microseconds(duration));
            b->time = 0;
            cout << "current process: "<< w->PID << " of name: " << b->type << " has been terminated" << endl;
            
            //check if instruction was the last from process
            string c = (r->next->data).type;
            if(c.compare("\"yield\"") == 0){

                cout << "deleting pcb " << w->PID << endl;
                w->state = TERMINANTED; //updating state to terminated
                queue1->deleteHead();
                continue;
            }
            //update PC
            w->PC = r->next;

        }
        else{
            //wait for qtime
            std::this_thread::sleep_for(std::chrono::milliseconds(qtime));
            b->time = duration - qtime;
        }
        //move current process to the back of the queue
        updateQ();
        

    }
    return;

}
 