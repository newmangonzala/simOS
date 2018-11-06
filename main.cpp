#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <time.h>
#include <chrono>
#include <fstream>
#include <thread> 

#include "rapidxml.hpp"

#include "timer.h"
#include "timer.cpp"

#include "linkedlist.h"

#include "memory.h"
#include "memory.cpp"

#include "pcb.h"
#include "pcb.cpp"

#include "scheduler.h"
#include "scheduler.cpp"



#include "log.h"

using namespace std;
using namespace rapidxml;

int PrBkCtr::id = 0;

int main(){

/*
    timer t1;
    cout << t1.initTime << endl;

    for(int i = 0; i < 1000000; i++){
        for(int j = 0; j < 100; j++){
                
    }
    }

    cout <<  t1.getTime() << endl;

    std::chrono::duration<double,std::milli> dif;
    auto end = std::chrono::system_clock::now(); 
    dif = end - t1.time_point  ;

    std::cout << dif.count() << std::endl;
   
*/
    int numPr;
    cout << "Please enter numbers of processes to be created" << endl;
    cin >> numPr;

    mem M1(numPr);

    //mem::createMailbox *tmp = new mem::createMailbox;
    //M1.mailbox = tmp;

    M1.loadApps();
 
 /*
    auto app = M1.memOfProcesses[1];
    List<std::string>::node* tmp = app.getHead();
    //tmp = tmp->next;
    List<std::string>::node* ptr = tmp->next;

    PrBkCtr pcb(ptr);


    cout << ptr->data << endl;

    cout << pcb.PCtmp->data <<endl;

    auto ti = pcb.PCtmp->data;
    basic_string<char>* tp = &pcb.PCtmp->data;

    xml_document<> doc;
    doc.parse<0>(&ti[0]);
    xml_node<> * inst = doc.first_node();
    rapidxml::xml_attribute<char>* t = inst->first_attribute("time");
    int tmpTime = stoi(t->value());

    cout << tmpTime << endl;

    int replace = tmpTime - 20;
    inst->remove_first_attribute();
    tp->replace(14,2,"99");

    cout << pcb.PCtmp->data <<endl;

*/


    List<PrBkCtr> queue1;


    Sched S1(queue1, M1);

    for(int i = 0; i < M1.memOfProcesses.size(); i++){
        PrBkCtr pcb(M1.memOfProcesses[i].getHead());
        M1.mailboxes.insert({pcb.mailbox->id,&pcb.mailbox->messages});
                            auto search = M1.mailboxes.find(pcb.mailbox->id);
                            //search->second->push_front("test");
                            //cout <<  &search->second << endl;
                    //.push_front("test");

        pcb.state = READY;              //change PCB state to READY
        S1.queue1->insertNode(pcb);     //insert pcbs into READY QUEUE
    }


    S1.running2();

    //auto search = M1.mailboxes.find(2);
    //search->second->push_front("test");
    //cout << search->second->back() << endl;


    

/*
    List<PrBkCtr>::node* h2 = S1.queue1->getHead();
    PrBkCtr w = h2->data;
    List<mem::instrucion>::node* r = w.PC;
    mem::instrucion b = r->data;
    cout << b.type << endl;
    cout << w.PID << endl;
    cout << "pcb state: " << w.state << endl;

    h2 = h2->next;
    w = h2->data;
    r = w.PC;
    b = r->data;
    cout << b.type << endl;
    cout << w.PID << endl;
*/
  
        

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
