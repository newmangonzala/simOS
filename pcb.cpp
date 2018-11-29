#include "pcb.h"


PrBkCtr::PrBkCtr(){

    Registers.R0 = "";        //CPU register
    Registers.R1 = "";  
    Registers.R2 = "";  
    Registers.R3 = "";  
    Registers.R4 = "";  
    
    state = NEW;
    PID = id;
    //Privalage = 0;
}

PrBkCtr::PrBkCtr(List<std::string>::node* nextInst){
    id++;  
    PCtmp = nextInst;     //PC points to current instruction
    PID = id;   //unique ID
    ParentPID = 0; 
    state = NEW;    //State of the process

    //mailbox = new createMailbox;
    mailbox.id = id;

    queue = 1;

    pgTbl =  new pageTable(4);

    rw_mutex = new semaphore;
    rw_mutex->value = 1;
    mutex = new semaphore;
    mutex->value = 1;
}
