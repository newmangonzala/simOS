#ifndef PCB_H
#define PCB_H

#include <string>

enum STATE{NEW , READY , RUNNING , WAITING , TERMINANTED};

class PrBkCtr{
    public:

        static int id;

        PrBkCtr();
        PrBkCtr(List<mem::instrucion>::node*);
    
        struct Registers{           //CPU registers
            string R0;
            string R1;
            string R2;
            string R3;
            string R4;
        }Registers; 
        STATE state;   //Process state {new, ready, running, waiting, terminated}
        unsigned int PID;                           //Unique process number ID
        List<mem::instrucion>::node* PC;            //Program Counter
        unsigned int Privalage;     //Privalage

        // priorities, scheduling queue pointers
        unsigned int priority;
        unsigned int queue;
        
        // memory allocated
        //unsigned int address;  //might not need this
           
        unsigned int IOinfo;
        
        //struct accounting{
            //time constraints
            //CPU used
            //
        //}accounting;

};

#endif