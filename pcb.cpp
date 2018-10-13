#include "pcb.h"

PCB::PCB(){
    Registers.R0 = "";        //CPU register
    Registers.R1 = "";  
    Registers.R2 = "";  
    Registers.R3 = "";  
    Registers.R4 = "";  

    state = NEW;
    PID = 77;
    PC = 0;
    Privalage = 0;

}