#include "pcb.h"


PrBkCtr::PrBkCtr(void){
    id++;

    Registers.R0 = "";        //CPU register
    Registers.R1 = "";  
    Registers.R2 = "";  
    Registers.R3 = "";  
    Registers.R4 = "";  
    

    state = NEW;
    PID = id;
    PC = 0;
    Privalage = 0;


}
