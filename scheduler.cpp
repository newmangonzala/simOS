#include "scheduler.h"
#include <thread> 

Sched::Sched(List<PrBkCtr>& Q1, mem& A){ 
    queue1 = &Q1;
    M1 = &A;
}
//Sched::Sched(){}

int Sched::manage(){
    return 1;
}

//move current process to the back of the queue
void Sched::updateQ(){

    List<PrBkCtr>::node* head = queue1->getHead();
    List<PrBkCtr>::node* tail = queue1->getTail();
    List<PrBkCtr>::node* prev = head->next;
    

    if(head != tail){
        tail->next = head;
        
    }
    else{
        return;
    }
    
    
    queue1->head = prev;
    queue1->tail = head;
    queue1->tail->next = NULL;
  
 /*
    PrBkCtr* w = &queue1->head->data;
    List<mem::instrucion>::node* r = w->PC;
    mem::instrucion* b = &r->data;
    cout << b->type << endl;
  */  
    
    return;
}


        //std::chrono::duration<double,std::milli> dif;
        //auto start = std::chrono::system_clock::now(); 
        //auto end = std::chrono::system_clock::now(); 
        //dif = end - start;

void Sched::running(){
    
    //run until Ready queue is empty
    while(!queue1->isEmpty()){

        List<PrBkCtr>::node* head = queue1->getHead();
        PrBkCtr* w = &head->data;
        List<mem::instrucion>::node* r = w->PC;
        mem::instrucion* b = &r->data;
        int duration = b->time;
        w->state = RUNNING;     //updating state to running
        cout << "current process: "<< w->PID << " running inst: "  << b->type << "and has a current duration of: " << duration << endl;
        
        if((duration - qtime) <= 0){
            //wait for qtime - duration
            std::this_thread::sleep_for(std::chrono::microseconds(duration));
            b->time = 0;
            cout << "current process: "<< w->PID << " of name: " << b->type << " has been terminated" << endl;
            
            //check if instruction was the last from process
            string c = (r->next->data).type;
            if(c.compare("\"yield\"") == 0){

                cout << "deleting pcb " << w->PID << endl;
                w->state = TERMINANTED; //updating state to terminated
                queue1->deleteHead();
                continue;
            }
            if(c.compare("\"fork\"") == 0){

                cout << "forking " << w->PID << endl;
                auto tmp = w->baseAddress;
                

                w->state = WAITING; //updating state to terminated
                
                continue;
            }


            //update PC
            w->PC = r->next;

        }
        else{
            //wait for qtime
            std::this_thread::sleep_for(std::chrono::milliseconds(qtime));
            b->time = duration - qtime;
        }
        //move current process to the back of the queue
        updateQ();
        

    }
    return;

}

void Sched::running2(){
//run until Ready queue is empty
    while(!queue1->isEmpty()){

        List<PrBkCtr>::node* head = queue1->getHead();
        PrBkCtr* w = &head->data;
        List<string>::node* r = w->PCtmp;
        basic_string<char> toParse = r->data;
        basic_string<char>* b = &r->data;


        xml_document<> doc;
        doc.parse<0>(&(toParse)[0]);
        xml_node<> * inst = doc.first_node();
        rapidxml::xml_attribute<char>* t = inst->first_attribute("time");

        
        int duration = stoi(t->value());
        w->state = RUNNING;     //updating state to running
        cout << "current process: "<< w->PID << " running inst: "  << inst->value() << " and has a current duration of: " << duration << endl;
        
        //inst->remove_first_attribute();
        if((duration - qtime) <= 0){
            //wait for qtime - duration
            std::this_thread::sleep_for(std::chrono::microseconds(duration));
            b->replace(14,2,"00");
            //cout<< "NEW " << r->data << endl;
            
            cout << "current process: "<< w->PID << " of name: " << inst->value() << " has been terminated" << endl;
            
            //check if instruction was the last from process

            
            toParse = r->next->data;
            doc.parse<0>(&(toParse)[0]);
            inst = doc.first_node();
            string c = inst->value();
            //cout << c << endl;
            if(c.compare("\"yield\"") == 0){

                cout << "deleting pcb " << w->PID << endl;
                w->state = TERMINANTED; //updating state to terminated
                queue1->deleteHead();
                continue;
            }
          
            c = inst->name();
            if(c.compare("fork") == 0){

                cout << "forking " << w->PID << endl;
                xml_document<> doc;
                xml_node<> * root_node;
                ifstream file("process.xml");
                vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

                //make sure to zero terminate the buffer
                buffer.push_back('\0');

                doc.parse<0>(&buffer[0]);
                root_node = doc.first_node("Processes");
                root_node = root_node->first_node("child");


                List<string> instructions;
                xml_document<> tmpPr;
 
                for (xml_node<> * inst = root_node->first_node("action"); inst; inst = inst->next_sibling()){
                    if(inst == 0){
                        cout << "no node" << endl;
                        continue;
                    }

                    xml_node<char>* SInst = tmpPr.clone_node(inst);

                    tmpPr.append_node(SInst);
                    string xml_as_string = "";
                    print(std::back_inserter(xml_as_string), tmpPr);
                    //print instruction
                    //std::cout << "-----\n" << xml_as_string << std::endl;

                    instructions.insertNode(xml_as_string);

                    tmpPr.remove_first_node();
                }

                PrBkCtr pcb(instructions.getHead());
                pcb.state = READY;              //change PCB state to READY
                pcb.parent = w->PID;
                w->parent = true;
                w->childs.push_back(pcb.PID);
                queue1->insertNode(pcb);     //insert pcbs into READY QUEUE
                M1->mailboxes.insert({pcb.mailbox->id,&pcb.mailbox->messages});

            }
            else if(c.compare("send") == 0){
                //cout << M1->numProcess << endl;
                string tmpmail = inst->first_attribute("mailbox")->value();
                cout << "child of current pr is " << w->childs[0] << endl;
                if(tmpmail.compare("child") == 0){
                    auto search = M1->mailboxes.find(w->childs[0]);
                    search->second->push_front(inst->first_attribute("message")->value());
                    //auto search = M1.mailboxes.find(pcb.mailbox->id);
                    //cout << search->second->back() << endl;
                    //cout << search->second << endl;
                }
                
                //string me = inst->first_attribute("message")->value();
                //M1->mailbox->messages.push_back(me);
                //M1->mailbox->messages.pop_front();
                //if (M1->mailbox->messages.back().compare("end") == 0){
                //    cout << "SI" << endl;
               // }
            }
            else if(c.compare("receive") == 0){
                auto search = M1->mailboxes.find(w->id);
                cout << search->second->back() << endl;
            //    M1->mailbox->messages.pop_back();
            }
    


            //update PC
            w->PCtmp = r->next;

        }
        else{
            //wait for qtime
            std::this_thread::sleep_for(std::chrono::milliseconds(qtime));
            int timep = duration - qtime;
            string tn = to_string(timep);
            b->replace(14,2,tn);
            //cout<< "NEW " << r->data << endl;
        }
        //move current process to the back of the queue
        updateQ();
        

    }
    

    return;
}


 