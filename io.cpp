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
