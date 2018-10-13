#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <time.h>
#include <chrono>
#include <fstream>

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
    cin >> numPr;


    // mem M1(1);
    mem M1(numPr);
    vector<mem::instrucion> t = M1.loadApps();
    
    List<mem::instrucion> q = M1.memory1[0];
    List<mem::instrucion>::node* h = q.getHead();
    mem::instrucion z = h->data;

    cout << z.type << endl;

    h = h->next;
    z = h->data;
    cout << z.type << endl;

    /*
    for(int i = 0; i < M1.memory1[0].size(); i++){
        cout << M1.memory1[0][i].type << " ";
        cout << M1.memory1[0][i].time << endl;
    }*/

    //cout << M1.memory1.size() << endl;
    //cout << M1.numProcess << endl;   

    List<PrBkCtr> queue1;

    Sched S1(queue1);




    //for(int i = 0; i < M1.memory1.size(); i++){
    //    PrBkCtr pcb(M1.memory1[0].head);
    //    auto x = M1.memory1[0].head;
    //    auto n = x->data.type;
        //x = x->next;
        //cout << t  << endl;
        //t = x->data.type;
        //cout << t << endl;
        
    //    S1.queue1->insertNode(pcb);
    //}
        
  
    auto l = M1.memory1[0].getHead()->data.type;
    //cout << l << endl;
    
/*
    //PrBkCtr pcb1;

    //List<PrBkCtr> queue1; //READY

    

    Sched S1(queue1);

    cout << &queue1 << endl;
    cout << S1.queue1 << endl;

    queue1.insertNode(pcb1);  //these two are equev
    //S1.queue1->insertNode(pcb1);


    auto t = S1.queue1->getHead();
    auto g = queue1.getHead();

    cout << t << endl; //equ
    cout << g << endl;

    List<int> L1;
    L1.insertNode(5);
    auto m = L1.getHead();
    cout << m->data << endl;

  
    PrBkCtr* f = &(t->data);
    PrBkCtr* c = &(g->data);
    cout << &(*f) << endl;
    cout << &(*c) << endl;
 
    f->PID = 33;

    cout << f->PID << endl;
    cout << c->PID << endl;

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
