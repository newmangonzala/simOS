#ifndef PCB_H
#define PCB_H

#include <string>
#include <queue>



//PCB states
enum STATE{NEW , READY , RUNNING , WAITING , TERMINANTED};

class pageTable{

    public: 

    vector<int*> entries;
    //int** entries = new int*[256];
    
    pageTable(){
        for(int i = 0 ; i < sizeOfPageTable; i++){
            int * tmp = new int[2];
            tmp[0] = 0; //initialize valid bits to 0
            entries.push_back(tmp);
        }

    }
    
};

class PrBkCtr;

typedef struct{
    int value;
    DoublyList<PrBkCtr*> processes;
}semaphore;

class PrBkCtr{
    public:

        static int id; //static id

        PrBkCtr();
        //PrBkCtr(List<mem::instrucion>::node*);
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
        bool parent;
        int queue;                     
        short int PTLR;                             //Page Table Length Register
        List<std::string>::node* PCtmp;             //Program Counter
        unsigned int priority;                      // priorities, scheduling queue pointers
        vector<PrBkCtr*> childs;
        pageTable* pgTbl;                           //page table pointer
        
        
        //unsigned int Privalage;                     //Privalage
        
        // For critical section
        int sharedMem;
        semaphore* rw_mutex;
        semaphore* mutex;
        int read_count = 0;
        
        //Mailboxes
        struct createMailbox{
            List<string> messages;
            int id;
        };
        createMailbox mailbox;

};



#endif