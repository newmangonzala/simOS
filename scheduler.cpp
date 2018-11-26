#include "scheduler.h"
#include <thread> 

Sched::Sched(DoublyList<PrBkCtr*>& Q1, DoublyList<PrBkCtr*>& Q2, DoublyList<PrBkCtr*>& Q3, mem& A, ipc& MailB){ 
    queue1 = &Q1;
    queue2 = &Q2;
    queue3 = &Q3;
    M1 = &A;
    MB = &MailB;
}

//move current process to the back of the queue
void Sched::updateQ(DoublyList<PrBkCtr *>* queue){

    DoublyList<PrBkCtr*>::node* head = queue->getHead();
    DoublyList<PrBkCtr*>::node* tail = queue->getTail();
    DoublyList<PrBkCtr*>::node* prev;
    
    if(head == tail){
        return;
    }

    tail->next = head;  
    prev = head->next;
    queue->head = prev;
    queue->tail = head;
    queue->tail->next = NULL;
     
    return;
}
void Sched::updateQ2(){


    return;
}



void Sched::running2(){
//run until Ready queue is empty

 
    while(!queue1->isEmpty() || !queue2->isEmpty() || !queue3->isEmpty()){
        
        if(!queue1->isEmpty()){
            servingQ = Queue1;
            cout << "Q1 "<< endl;

            runRR(queue1);

            cout << endl;
 
            DoublyList<PrBkCtr*>::node* head = queue1->getHead();
            queue1->popHead();
            queue2->insertNode(head->data);
            

        }
        else if(!queue2->isEmpty()){
            servingQ = Queue2;
            cout << "Q2 "<< endl;

            runRR(queue2);

            cout << endl;

            DoublyList<PrBkCtr*>::node* head = queue2->getHead();
            queue2->popHead();
            queue3->insertNode(head->data);
        }
        else if(!queue3->isEmpty()){
            servingQ = Queue3;
            cout << "Q3 "<< endl;

            runFIFO(queue3);

            cout << endl;
            updateQ(queue3);
        }
        
    }
    return;
}

void Sched::runRR(DoublyList<PrBkCtr *>* queue){
    DoublyList<PrBkCtr*>::node* head = queue->getHead();
    PrBkCtr* w = head->data;
    List<string>::node* r = w->PCtmp;
    basic_string<char> toParse = r->data;
    
    //print instruction
    //cout << r->data << endl;

    xml_document<> doc;
    doc.parse<0>(&(toParse)[0]);
    xml_node<> * inst = doc.first_node();
    rapidxml::xml_attribute<char>* t = inst->first_attribute("time");

    int duration = stoi(t->value());
    w->state = RUNNING;     //updating state to running
    cout << "current process: "<< w->PID << " running inst: "  << inst->value() << " and has a current duration of: " << duration << endl;
    

    auto f = inst->first_attribute("pages");
    if(f != 0){
        string s = f->value();
        vector<int> pages = findPages(s);
        mmu(w, pages);
    }


    if((duration - qtime) <= 0){
        //wait for qtime - duration
        std::this_thread::sleep_for(std::chrono::microseconds(duration));
        
        r->data = updateTime(r->data, 0);
        
        toParse = r->data;
        doc.parse<0>(&(toParse)[0]);
        inst = doc.first_node();
        string c = inst->value();

        if(c.compare("\"yield\"") == 0){
            yield(w);
            return;
        }
        else if(c.compare("fork") == 0){
            fork(w);
        }
        else if(c.compare("send") == 0){
            //cout << MB->numProcess << endl;
            string tmpmail = inst->first_attribute("mailbox")->value();
            cout << "child of current pr is " << w->childs[0]->PID << endl;
            if(tmpmail.compare("child") == 0){

                string messg = inst->first_attribute("message")->value();
                
                auto search = MB->mailboxes.find(w->childs[0]->PID);
    
                search->second->insertNode(messg);

            }
        
        }
        else if(c.compare("receive") == 0){
            auto search = MB->mailboxes.find(w->PID);
            
            for(int i=0; i < search->second->size(); i++)
                cout << "message received: " << search->second->getHead()->data << endl;
                search->second->deleteHead();                
        }
        
        cout << "current process: "<< w->PID << " of name: " << inst->value() << " has been terminated" << endl;
        w->PCtmp = r->next;     //update PC

    }
    else{
        //wait for qtime
        std::this_thread::sleep_for(std::chrono::milliseconds(qtime));
        int timep = duration - qtime;
        //string tn = to_string(timep);
        r->data = updateTime(r->data, timep);
        
    }
}

void Sched::runFIFO(DoublyList<PrBkCtr *>* queue){
    DoublyList<PrBkCtr*>::node* head = queue->getHead();
    PrBkCtr* w = head->data;
    List<string>::node* r = w->PCtmp;
    basic_string<char> toParse = r->data;
    
    //print instruction
    //cout << r->data << endl;

    xml_document<> doc;
    doc.parse<0>(&(toParse)[0]);
    xml_node<> * inst = doc.first_node();
    rapidxml::xml_attribute<char>* t = inst->first_attribute("time");

    int duration = stoi(t->value());
    w->state = RUNNING;     //updating state to running
    cout << "current process: "<< w->PID << " running inst: "  << inst->value() << " and has a current duration of: " << duration << endl;
    

    auto f = inst->first_attribute("pages");
    if(f != 0){
        string s = f->value();
        vector<int> pages = findPages(s);
        mmu(w, pages);
    }
  
    //wait for qtime - duration
    std::this_thread::sleep_for(std::chrono::microseconds(duration));
       
    toParse = r->data;
    doc.parse<0>(&(toParse)[0]);
    inst = doc.first_node();
    string c = inst->value();

    if(c.compare("\"yield\"") == 0){
        yield(w);
        return;
    }
    else if(c.compare("fork") == 0){
        fork(w);
    }
    else if(c.compare("send") == 0){
        //cout << MB->numProcess << endl;
        string tmpmail = inst->first_attribute("mailbox")->value();
        cout << "child of current pr is " << w->childs[0]->PID << endl;
        if(tmpmail.compare("child") == 0){

            string messg = inst->first_attribute("message")->value();
            
            auto search = MB->mailboxes.find(w->childs[0]->PID);

            search->second->insertNode(messg);

        }
    
    }
    else if(c.compare("receive") == 0){
        auto search = MB->mailboxes.find(w->PID);
        
        for(int i=0; i < search->second->size(); i++)
            cout << "message received: " << search->second->getHead()->data << endl;
            search->second->deleteHead();                
    }
    
    cout << "current process: "<< w->PID << " of name: " << inst->value() << " has been terminated" << endl;
    w->PCtmp = r->next;     //update PC

    r->data = updateTime(r->data, 0);
        

}

string Sched::updateTime(string instruction, int newTime){
    
    int i=0;
    string nt = to_string(newTime);
    for(char C : instruction){
        if(C == '\"'){
            instruction.replace(++i,2,nt);
            break;
        }
        i++;
    }

    return instruction;
}

void Sched::fork(PrBkCtr* w){

                //cout << "forking " << w->PID << endl;
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

                //PrBkCtr pcb(instructions.getHead());
                PrBkCtr* pcb = new PrBkCtr(instructions.getHead());
                pcb->state = READY;              //change PCB state to READY
                pcb->parent = w->PID;
                w->parent = true;
                w->childs.push_back(pcb);
                queue1->insertNode(pcb);     //insert pcbs into READY QUEUE
                
                //tmpIPC.mailboxes.insert({pcb->mailbox.id,&pcb->mailbox.messages});
                MB->mailboxes.insert({pcb->mailbox.id,&pcb->mailbox.messages});

    return;
}

void Sched::yield(PrBkCtr* w){

    if(checkChilds(w)){
            while(checkChilds(w)){
                auto childProcess = w->childs.back();           //get childs
                switch(childProcess->state) {
                    case READY:
                        {
                        auto node = MB->PrTable.find(childProcess->PID);
                        terminatePr(node->second);
                        }
                        break;
                    case TERMINANTED:
                        break;
                    default:
                        cout << "ERROR with child states" << endl;
                        break;
                }
                w->childs.pop_back();
            }
    }
            
    w->state = TERMINANTED;                         //updating state to terminated
    if(servingQ == Queue1){
            queue1->deleteHead();
            cout << "process " << w->PID << " has been terminated from queue 1" << endl;
    }
    else if(servingQ == Queue2){
            queue2->deleteHead();
            cout << "process " << w->PID << " has been terminated from queue 2" << endl;
    }
    else{
        queue3->deleteHead();
        cout << "process " << w->PID << " has been terminated from queue 3" << endl;
    }
    
/*
    if(w->childs.size() > 0){
        auto childProcess = w->childs[0];           //get childs
        List<PrBkCtr*>::node* prev = queue1->getHead();         //get current process
 
        int size = queue1->size();
        if(size > 1){
            
            List<PrBkCtr*>::node* tmpPr = prev->next;
            PrBkCtr* tmp = tmpPr->data;

            while(tmp != childProcess && tmpPr->next != NULL){ //iterate through each process to find child
                prev = tmpPr;
                tmpPr = tmpPr->next;
                tmp = tmpPr->data;
            }
            if(tmp == childProcess){
                tmp->state = TERMINANTED;               //updating state of child to terminated
                cout << "child of parent " << w->PID << " with id of " << tmp->PID << " has been terminated" << endl; 
            }
            if(tmpPr != NULL){
                prev->next = tmpPr->next;
                delete tmpPr;
            }
        }
    
    }

    w->state = TERMINANTED;                         //updating state to terminated

    queue1->deleteHead();

    cout << "process " << w->PID << " has been terminated" << endl;
*/
    return;
}

vector<int> Sched::findPages(string s){

    vector<int> pages;

    stringstream ss;

    ss << s;

    string temp;

    int page;

    while(!ss.eof()){

        ss >> temp;
        if(stringstream(temp) >> page){
            pages.push_back(page);
            //cout << page << endl;
        } 
        temp = "";
    }

    return pages;
}

void Sched::mmu(PrBkCtr* w, vector<int> pages){

    for(int i = 0; i < pages.size(); i++){
                
        if(pages[i] > pages.size()){
            continue; //FIX ME
        }

        cout << "Process id " << w->PID << " page: " << pages[i] << " bit: " << w->pgTbl->entries[pages[i]][0] << endl;

        if(!w->pgTbl->entries[pages[i]][0]){   //valid bit is 0; meaning is not in memory
            //page is not in memory

            if(M1->freeFrames.size() > 0){
                
                auto freeFrame = M1->freeFrames.front();
                M1->freeFrames.pop();                               //pop free frame
                w->pgTbl->entries[pages[i]][1] = freeFrame;         //map frame to page in page table
                
                M1->mainMem[freeFrame][0] = 0;                //change second chance bit to 0
                M1->mainMem[freeFrame][1] = w->PID;           //store process id into frame, just for reference
                
            }
            else{
                // replace frames because there are not free frames
                while(M1->mainMem[M1->currentFrameIndex][0]){                           //find frame with second chance bit 0
                    M1->mainMem[M1->currentFrameIndex][1] = 0;                //change second chance bit to 0
                    M1->currentFrameIndex++;
                    continue; 
                    
                }
                M1->mainMem[M1->currentFrameIndex][1] = w->PID;                    //replace page in current frame
                //no need to set second chance to 0 since it was zero
                //update table FIX ME
                w->pgTbl->entries[pages[i]][1] = M1->currentFrameIndex;         //map frame to page in page table
                

            }
            w->pgTbl->entries[pages[i]][0] = 1;                 //change valid bit to 1, meaning is now in Mmem
            M1->currentFrameIndex = M1->currentFrameIndex++ % numFrames;        //Increase index of mem
        }
        else{   //valid bit is 1 so page is in memory
            
            auto frame = w->pgTbl->entries[pages[i]][1];    //get location of page in Mmem; get frame
            if(!M1->mainMem[frame][0]){
                M1->mainMem[frame][1] = 1;                //change second chance bit to 1
                
            }
        }
        
    }

    return;
}

bool Sched::checkChilds(PrBkCtr* w){
    if(w->childs.size() > 0){
        return true;
    }
    else
        return false;
}

void Sched::terminatePr(DoublyList<PrBkCtr*>::node* prTerm){
    
    PrBkCtr* ptrData = prTerm->data;
  
    if(ptrData->queue == 1)
        queue1->deleteNode(prTerm);
    else if(ptrData->queue == 2)
        queue2->deleteNode(prTerm);
    else if(ptrData->queue == 3)
        queue3->deleteNode(prTerm);
    else
        cout << "ERROR prTerm" << endl;

}