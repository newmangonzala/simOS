#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <time.h>
#include <chrono>
#include <fstream>
#include <thread> 

#define numFrames 10 //256

#include "rapidxml.hpp"

#include "timer.h"
#include "timer.cpp"

#include "linkedlist.h"

#include "memory.h"
#include "memory.cpp"

#include "pcb.h"
#include "pcb.cpp"

#include "ipc.h"

#include "scheduler.h"
#include "scheduler.cpp"



#include "log.h"


#include <cstdlib>

using namespace std;
using namespace rapidxml;

int PrBkCtr::id = 0;


int main(){

    int numPr;
    cout << "Please enter numbers of processes to be created" << endl;
    cin >> numPr;

    mem M1(numPr);

    M1.loadApps();
 

    List<PrBkCtr*> queue1;
    ipc tmpIPC;


    Sched S1(queue1, M1, tmpIPC);

/*
    for(int i = 0; i < M1.memOfProcesses.size(); i++){
        auto f = M1.memOfProcesses[i].getHead();
        while(f != NULL){
            cout << f->data << endl;
            f = f->next;
        }
        cout << "------" << endl;

    }*/

    

    for(int i = 0; i < M1.memOfProcesses.size(); i++){

        PrBkCtr* pcb = new PrBkCtr(M1.memOfProcesses[i].getHead());
        tmpIPC.mailboxes.insert({pcb->mailbox.id,&pcb->mailbox.messages});
                           
        pcb->state = READY;              //change PCB state to READY
        
        S1.queue1->insertNode(pcb);     //insert pcbs into READY QUEUE

    }

    S1.running2();

    pageTable p(3);
    p.entries[0][0] = 4;

    cout << p.entries[0][0] << endl;
    int * tmp = new int[2];
    p.entries.push_back(tmp);
    cout << p.entries[3][0] << endl;

    PrBkCtr* pcb = new PrBkCtr(M1.memOfProcesses[0].getHead());
    pcb->pgTbl->entries[0][0] = 5;;
    cout << pcb->pgTbl->entries[0][0] << endl;

    

    //cout << M1.currentFrame[0] << endl;


    /*
    log(logINFO) << "foo " << "bar " << "baz";

    int count = 3;
    log(logDEBUG) << "A loop with "    << count << " iterations";
    for (int i = 0; i != count; ++i)
    {
        log(logDEBUG) << "the counter i = " << i;
    }

    */

    
    system("pause");
}
