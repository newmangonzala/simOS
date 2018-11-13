#ifndef PCB_H
#define PCB_H

#include <string>

//PCB states
enum STATE{NEW , READY , RUNNING , WAITING , TERMINANTED};

class PrBkCtr{
    public:

        static int id; //static id

        PrBkCtr();
        PrBkCtr(List<mem::instrucion>::node*);
        PrBkCtr(List<std::string>::node*);
    
        struct Registers{                           //CPU registers
            string R0;
            string R1;
            string R2;
            string R3;
            string R4;
        }Registers; 
        STATE state;                                //Process state
        unsigned int PID;                           //Unique process number ID
        unsigned int ParentPID;                     
        string baseAddress;
        List<mem::instrucion>::node* PC;            //Program Counter
        List<std::string>::node* PCtmp;
        unsigned int Privalage;                     //Privalage

        // priorities, scheduling queue pointers
        unsigned int priority;
        unsigned int queue;
        
        // memory allocated
        //unsigned int address;  //might not need this
           
        unsigned int IOinfo;
        bool parent;
        vector<PrBkCtr*> childs;

        mem::createMailbox *mailbox;
        
        //struct accounting{
            //time constraints
            //CPU used
            //
        //}accounting;

};

#endif