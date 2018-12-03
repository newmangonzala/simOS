#include "io.h"
#include <thread>


void Io::send(xml_node<> * inst, PrBkCtr* w, ipc* MB){
    
    string tmpmail = inst->first_attribute("mailbox")->value();
    
    if(tmpmail.compare("child") == 0){

        string messg = inst->first_attribute("message")->value();

        for(int i = w->childs.size() - 1; i >= 0; i--){

            //cout << "child of current pr is " << w->childs[i]->PID << endl;
        
            auto search = MB->mailboxes.find(w->childs[i]->PID);
            search->second->insertNode(messg);
        }

    }
}

void Io::receive(PrBkCtr* w, ipc* MB){
    auto search = MB->mailboxes.find(w->PID);
    
    for(int i=0; i < search->second->size(); i++)
        cout << "message received: " << search->second->getHead()->data << endl;
        search->second->deleteHead();
}

void Io::interruptHandler(){
    int random = rand() % 100;
    //cout << "IO for " << random << endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(random));
}


void Io::write(Sched* S1, PrBkCtr* pr){

    //writer
    wait(S1, pr->rw_mutex, pr);

    //writing
    int* tmp = new int;
    *tmp = rand();
    pr->sharedMem = tmp;

    signal(S1, pr->rw_mutex, pr);
}

void Io::read(Sched* S1, PrBkCtr* pr){
    
    //reader
    wait(S1, pr->mutex, pr);
    pr->read_count++;
    if(pr->read_count == 1)
        wait(S1, pr->rw_mutex, pr);
    signal(S1,pr->mutex, pr);
 
    //reading
    //cout << *pr->sharedMem << endl;

    wait(S1, pr->mutex, pr);
    pr->read_count--;
    if(pr->read_count == 0)
        signal(S1, pr->rw_mutex, pr);
    signal(S1, pr->mutex, pr);
}

void Io::wait(Sched* S1, semaphore *S, PrBkCtr* process){
    S->value--;
    if(S->value < 0){
        S->processes.insertNode(process);
        //S1->servingQ->deleteHead();
        //block
        process->state = WAITING;
        
    }
    
}

void Io::signal(Sched* S1, semaphore *S, PrBkCtr* process){
    S->value++;
    if(S->value <= 0){
        //wakeup
        S->processes.getHead()->data->state = READY;
        S->processes.deleteHead();
        S1->queue1->insertNode(process);
    }
}