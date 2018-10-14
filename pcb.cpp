#include "pcb.h"


PrBkCtr::PrBkCtr(){

    Registers.R0 = "";        //CPU register
    Registers.R1 = "";  
    Registers.R2 = "";  
    Registers.R3 = "";  
    Registers.R4 = "";  
    
    state = NEW;
    PID = id;
    Privalage = 0;
}

PrBkCtr::PrBkCtr(List<mem::instrucion>::node* v){
    id++;  
    PC = v;     //PC points to current instruction
    PID = id;   //unique ID 
    state = NEW;    //State of the process
}
