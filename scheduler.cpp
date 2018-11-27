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

void Sched::running(){
    //run until Ready queues are empty

    while(!queue1->isEmpty() || !queue2->isEmpty() || !queue3->isEmpty()){
        
        if(!queue1->isEmpty()){
            servingQ = Queue1;
            //cout << "Q1 "<< endl;

            DoublyList<PrBkCtr*>::node* head = queue1->getHead();
            queue1->popHead();
            
            PrBkCtr* currentPr = head->data;
            currentPr->state = RUNNING;     //updating state to running

            if(runRR(currentPr)){
                cout << endl;
                queue2->insertNode(currentPr);
                currentPr->state = READY;
                MB->PrTable.insert({currentPr->PID, queue2->tail});
                currentPr->queue = 2;
            }
        }
        
        else if(!queue2->isEmpty()){
            servingQ = Queue2;
            //cout << "Q2 "<< endl;

            DoublyList<PrBkCtr*>::node* head = queue2->getHead();

            queue2->popHead();
            
            PrBkCtr* currentPr = head->data;
            currentPr->state = RUNNING;     //updating state to running

            if(runRR(currentPr)){
                cout << endl;
                queue2->insertNode(currentPr);
                currentPr->state = READY;
                MB->PrTable.insert({currentPr->PID, queue2->tail});
                currentPr->queue = 2;
            }
        }

        /*
        else if(!queue3->isEmpty()){
            servingQ = Queue3;
            cout << "Q3 "<< endl;

            runFIFO(queue3);

            cout << endl;
            updateQ(queue3);
        }
        */
    }
    return;
}

bool Sched::runRR(PrBkCtr* PrTable){

    List<string>::node* instruction = PrTable->PCtmp;
    basic_string<char> toParse = instruction->data;
    
    //print instruction
    //cout << r->data << endl;

    xml_document<> doc;
    doc.parse<0>(&(toParse)[0]);
    xml_node<> * inst = doc.first_node();


    int duration = parseTime(inst);

    //PrTable->state = RUNNING;     //updating state to running
    cout << "current process: "<< PrTable->PID << " running inst: "  << inst->value() << " and has a current duration of: " << duration << endl;
    
    auto f = inst->first_attribute("pages");
    if(f != 0){
        string s = f->value();
        vector<int> pages = findPages(s);
        mmu(PrTable, pages);
    }


    if((duration - qtime) <= 0){
        //wait for qtime - duration
        std::this_thread::sleep_for(std::chrono::microseconds(duration));
        
        instruction->data = updateTime(instruction->data, 0);
        
        string c = inst->value();

        if(c.compare("\"yield\"") == 0){
            yield(PrTable);
            return false;
        }
        else if(c.compare("fork") == 0){
            fork(PrTable);
        }
        else if(c.compare("fork2") == 0){
            fork2(PrTable);
        }
        else if(c.compare("send") == 0){
            send(inst, PrTable);
        }
        else if(c.compare("receive") == 0){
            receive(PrTable);                 
        }
        else if(c.compare("write") == 0){
            write(PrTable);
        }
        else if(c.compare("read") == 0){
            read(PrTable);
        }
        
        cout << "current process: "<< PrTable->PID << " of name: " << inst->value() << " has been terminated" << endl;
        PrTable->PCtmp = instruction->next;     //update PC

    }
    else{
        //wait for qtime
        std::this_thread::sleep_for(std::chrono::milliseconds(qtime));
        int timep = duration - qtime;
        //string tn = to_string(timep);
        instruction->data = updateTime(instruction->data, timep);
        
    }
    return true;
}

void Sched::runFIFO(DoublyList<PrBkCtr *>* queue){
    DoublyList<PrBkCtr*>::node* head = queue->getHead();
    PrBkCtr* PrTable = head->data;
    List<string>::node* instruction = PrTable->PCtmp;
    basic_string<char> toParse = instruction->data;
    
    //print instruction
    //cout << instruction->data << endl;

    xml_document<> doc;
    doc.parse<0>(&(toParse)[0]);
    xml_node<> * inst = doc.first_node();

    int duration = parseTime(inst);

    PrTable->state = RUNNING;     //updating state to running
    cout << "current process: "<< PrTable->PID << " running inst: "  << inst->value() << " and has a current duration of: " << duration << endl;
    

    auto f = inst->first_attribute("pages");
    if(f != 0){
        string s = f->value();
        vector<int> pages = findPages(s);
        mmu(PrTable, pages);
    }
  
    //wait for qtime - duration
    std::this_thread::sleep_for(std::chrono::microseconds(duration));
       
    
    string c = inst->value();

    if(c.compare("\"yield\"") == 0){
        yield(PrTable);
        return;
    }
    else if(c.compare("fork") == 0){
        fork(PrTable);
    }
    else if(c.compare("fork2") == 0){
        fork2(PrTable);
    }
    else if(c.compare("send") == 0){
        send(inst, PrTable);
    }
    else if(c.compare("receive") == 0){
        receive(PrTable);              
    }
    
    cout << "current process: "<< PrTable->PID << " of name: " << inst->value() << " has been terminated" << endl;
    PrTable->PCtmp = instruction->next;     //update PC

    instruction->data = updateTime(instruction->data, 0);
        

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


void Sched::send(xml_node<> * inst, PrBkCtr* w){
    
    string tmpmail = inst->first_attribute("mailbox")->value();
    
    if(tmpmail.compare("child") == 0){

        string messg = inst->first_attribute("message")->value();

        for(int i = w->childs.size() - 1; i >= 0; i--){

            cout << "child of current pr is " << w->childs[i]->PID << endl;
        
            auto search = MB->mailboxes.find(w->childs[i]->PID);
            search->second->insertNode(messg);
        }

    }
}

void Sched::receive(PrBkCtr* w){
    auto search = MB->mailboxes.find(w->PID);
    
    for(int i=0; i < search->second->size(); i++)
        cout << "message received: " << search->second->getHead()->data << endl;
        search->second->deleteHead();
}


int Sched::parseTime(xml_node<> * inst){

    rapidxml::xml_attribute<char>* t = inst->first_attribute("time");
    return stoi(t->value());
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
                MB->PrTable.insert({pcb->PID, queue1->tail});

    return;
}

void Sched::fork2(PrBkCtr* w){

    //cout << "forking " << w->PID << endl;
    xml_document<> doc;
    xml_node<> * root_node;
    ifstream file("process.xml");
    vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    //make sure to zero terminate the buffer
    buffer.push_back('\0');

    doc.parse<0>(&buffer[0]);
    root_node = doc.first_node("Processes");
    root_node = root_node->first_node("child2");


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
    MB->PrTable.insert({pcb->PID, queue1->tail});

    return;
}

void Sched::yield(PrBkCtr* w){


    while(checkChilds(w)){
        PrBkCtr* childProcess = w->childs.back();           //get childs
        switch(childProcess->state) {
            case READY:
                {
                auto node = MB->PrTable.find(childProcess->PID);

                yield(childProcess);

                //removeNode(node);
                childProcess->state = TERMINANTED;
                DoublyList<PrBkCtr*>::node* tmp = findProcessNode(childProcess);

                terminatePr(tmp);
                cout << "- child" << childProcess->PID << " of process " << w->PID << " has been terminated" << endl;
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
    
            
    w->state = TERMINANTED;                         //updating state to terminated
    if(servingQ == Queue1){
            //queue1->deleteHead();
            cout << "process " << w->PID << " has been terminated from queue 1" << endl;
    }
    else if(servingQ == Queue2){
            //queue2->deleteHead();
            cout << "process " << w->PID << " has been terminated from queue 2" << endl;
    }
    else{
        //queue3->deleteHead();
        cout << "process " << w->PID << " has been terminated from queue 3" << endl;
    }
    
    return;
}

DoublyList<PrBkCtr*>::node* Sched::findProcessNode(PrBkCtr* process){

    if(process->queue == 1){
        DoublyList<PrBkCtr*>::node* i = queue1->getHead();
        for(; i->data->PID != process->PID; i = i->next){}
        return i;
    }
    else if(process->queue == 2){
        DoublyList<PrBkCtr*>::node* i = queue2->getHead();
        for(; i->data->PID != process->PID; i = i->next){}
        return i;
    }
    else{
        DoublyList<PrBkCtr*>::node* i = queue3->getHead();
        for(; i->data->PID != process->PID; i = i->next){}
        return i;
    }
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

void Sched::write(PrBkCtr* pr){

    //writer
    wait(pr->rw_mutex, pr);

    //writing

    signal(pr->rw_mutex, pr);
}

void Sched::read(PrBkCtr* pr){
    
    //reader
    wait(pr->mutex, pr);
    pr->read_count++;
    if(pr->read_count == 1)
        wait(pr->rw_mutex, pr);
    signal(pr->mutex, pr);

    //reading

    wait(pr->mutex, pr);
    pr->read_count--;
    if(pr->read_count == 0)
        signal(pr->rw_mutex, pr);
    signal(pr->mutex, pr);
}

void Sched::wait(semaphore *S, PrBkCtr* process){
    S->value--;
    if(S->value < 0){
        S->processes.insertNode(process);
        if(servingQ == Queue1)
            queue1->deleteHead();
        
        else if (servingQ == Queue2)
            queue2->deleteHead();
        else
            queue3->deleteHead();

        //block
        process->state = WAITING;
    }
}

void Sched::signal(semaphore *S, PrBkCtr* process){
    S->value++;
    if(S->value <= 0){
        //wakeup
        S->processes.getHead()->data->state = READY;
        S->processes.deleteHead();
        queue1->insertNode(process);
    }
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