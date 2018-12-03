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
    PC = nextInst;     //PC points to current instruction
    PID = id;   //unique ID
    ParentPID = 0; 
    state = NEW;    //State of the process

    //mailbox = new createMailbox;
    mailbox.id = id;

    queue = 1;

    pgTbl =  new pageTable();
    for(int i=0; i < sizeOfPageTable; i++){
        pgTbl->entries[i][1] = -1;
        pgTbl->entries[i][0] = 0;

    }

    rw_mutex = new semaphore;
    rw_mutex->value = 1;
    mutex = new semaphore;
    mutex->value = 1;
}

PrBkCtr::PrBkCtr(List<std::string>::node* nextInst, semaphore* rw, semaphore* mu ){
    id++;  
    PC = nextInst;     //PC points to current instruction
    PID = id;   //unique ID
    ParentPID = 0; 
    state = NEW;    //State of the process

    //mailbox = new createMailbox;
    mailbox.id = id;

    queue = 1;

    pgTbl =  new pageTable();
    for(int i=0; i < sizeOfPageTable; i++){
        pgTbl->entries[i][1] = -1;
    }

    rw_mutex = rw;
    rw_mutex->value = 1;
    mutex = mu;
    mutex->value = 1;

    sharedMem = new int;
}
