#include "scheduler.h"
#include <thread> 


Sched::Sched(DoublyList<PrBkCtr*>* Q1, DoublyList<PrBkCtr*>* Q2, DoublyList<PrBkCtr*>* Q3, mem& A, ipc& MailB){ 
    queue1 = Q1;
    queue2 = Q2;
    queue3 = Q3;
    M1 = &A;
    MB = &MailB;
}

void* Sched::running(){
    //run until Ready queues are empty

    DoublyList<PrBkCtr*>::node* head;
    

    while(!queue1->isEmpty() || !queue2->isEmpty() || !queue3->isEmpty()){
        
        if(!queue1->isEmpty()){
            servingQ = queue1;
            //cout << "Q1 "<< endl;

            pthread_mutex_lock( &mutex1 );
            head = queue1->getHead();
            queue1->popHead();
            pthread_mutex_unlock( &mutex1 );

            //cout << "Queue1" << endl;
            
            //ClearScreen();
            //printMainMem();
            //printProcess(head->data);

            
            PrBkCtr* currentPr = head->data;
            currentPr->state = RUNNING;     //updating state to running

            

            if(runRR(QTIME, currentPr)){
                //cout << endl;
                queue2->insertNode(currentPr);
                currentPr->state = READY;
                MB->PrTable.insert({currentPr->PID, queue2->tail});
                currentPr->queue = 2;
            }
        }
        
        else if(!queue2->isEmpty()){
            servingQ = queue2;
            //cout << "Q2 "<< endl;

            head = queue2->getHead();

            //cout << "Queue2" << endl;
            
            //ClearScreen();
            //printMainMem();
            //printProcess(head->data);


            PrBkCtr* currentPr = head->data;
            currentPr->state = RUNNING;     //updating state to running

            queue2->deleteHead();
            
            if(runRR(QTIME2, currentPr)){
                //cout << endl;
                queue2->insertNode(currentPr);
                currentPr->state = READY;
                MB->PrTable.insert({currentPr->PID, queue2->tail});
                currentPr->queue = 2;
            }
        }

        /*
        else if(!queue3->isEmpty()){
            servingQ = 3;
            cout << "Q3 "<< endl;

            runFIFO(queue3);

            cout << endl;
            updateQ(queue3);
        }
        */

       std::this_thread::sleep_for(std::chrono::microseconds(100));
    }
    return NULL;
}

void Sched::printProcess(PrBkCtr* process){
    
    cout << "Process ID: " << process->PID << endl;
    cout << "Process PC: " << process->PC->data << endl;
    if(process->parent)
        cout << "Process Parent: False" << endl;
    else
        cout << "Process Parent: True" << endl;

    cout << "---Page Table---" << endl;
    cout << "| V Bit | Page# | Frame# |" << endl;
    
    for(int i=0; i < sizeOfPageTable;i++){
        cout << setw(5)<< process->pgTbl->entries[i][0]<< setw(8) << i << setw(8) << process->pgTbl->entries[i][1] << endl;
        
    }


}

void Sched::printMainMem(){
    cout << setw(30) << "---Main-Mem---" << endl;
    cout << setw(15) << "__________" << setw(16)<< "__________" << setw(16)<< "__________" << setw(16)<< "__________" << endl;
    for(int i = 0; i < numFrames; i=i+6){
        cout << setw(4) << hex << i   + 10 << "| "<< setw(5) << dec << M1->mainMem[i][1]  << ", " << M1->mainMem[i][2]  <<" |";
        cout << setw(4) << hex << i+1 + 10 << "| "<< setw(5) << dec << M1->mainMem[i+1][1]<< ", " << M1->mainMem[i+1][2]<<" |";
        cout << setw(4) << hex << i+2 + 10 << "| "<< setw(5) << dec << M1->mainMem[i+2][1]<< ", " << M1->mainMem[i+2][2]<<" |";
        cout << setw(4) << hex << i+3 + 10 << "| "<< setw(5) << dec << M1->mainMem[i+3][1]<< ", " << M1->mainMem[i+3][2]<<" |";
        cout << setw(4) << hex << i+4 + 10 << "| "<< setw(5) << dec << M1->mainMem[i+4][1]<< ", " << M1->mainMem[i+4][2]<<" |";
        cout << setw(4) << hex << i+5 + 10 << "| "<< setw(5) << dec << M1->mainMem[i+5][1]<< ", " << M1->mainMem[i+5][2]<<" |" << endl;
    }
    cout << setw(15) << "----------" << setw(16)<< "----------" << setw(16)<< "----------" << setw(16)<< "----------" << endl;
}
   

bool Sched::runRR(int burst, PrBkCtr* process){

    List<string>::node* instruction = process->PC;
    basic_string<char> toParse = instruction->data;
    
    xml_document<> doc;
    doc.parse<0>(&(toParse)[0]);
    xml_node<> * inst = doc.first_node();


    int duration = parseTime(inst);

    //process->state = RUNNING;     //updating state to running
    //cout << "current process: "<< process->PID << " running inst: "  << inst->value() << " and has a current duration of: " << duration << endl;
    
    auto f = inst->first_attribute("pages");
    vector<int> pages;
    if(f != 0){
        string s = f->value();
        pages = M1->findPages(s);
        M1->mmu(process, pages);
    }
    //printPageTable(process);


    if((duration - burst) <= 0){
        //wait for QTIME - duration
        std::this_thread::sleep_for(std::chrono::microseconds(duration));
        
        instruction->data = updateTime(instruction->data, 0);
        
        string c = inst->value();

        if(c.compare("\"yield\"") == 0){
            yield(process);
            return false;
        }
        else if(c.compare("fork") == 0){
            fork(process, 1);
        }
        else if(c.compare("fork2") == 0){
            fork(process, 2);
        }
        else if(c.compare("send") == 0){
            Io::send(inst, process, MB);
        }
        else if(c.compare("receive") == 0){
            Io::receive(process, MB);                 
        }
        else if(c.compare("write") == 0){
            Io::write(this, process);
        }
        else if(c.compare("read") == 0){
            Io::read(this, process);
        }
        else if(c.compare("io") == 0){
            Io::interruptHandler();
        }
        
        cout << "current process: "<< process->PID << " of name: " << inst->value() << " has been terminated" << endl;
        process->PC = instruction->next;     //update PC

    }
    else{
        //wait for burst time
        std::this_thread::sleep_for(std::chrono::milliseconds(burst));
        int timep = duration - burst;
        instruction->data = updateTime(instruction->data, timep);
        
    }
    M1->resetEntryTLB(pages);
    return true;
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

int Sched::parseTime(xml_node<> * inst){

    rapidxml::xml_attribute<char>* t = inst->first_attribute("time");
    return stoi(t->value());
}


void Sched::fork(PrBkCtr* w, int type){

                //cout << "forking " << w->PID << endl;
                xml_document<> doc;
                xml_node<> * root_node;
                ifstream file("process.xml");
                vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

                //make sure to zero terminate the buffer
                buffer.push_back('\0');

                doc.parse<0>(&buffer[0]);
                root_node = doc.first_node("Processes");
                if(type == 1)
                    root_node = root_node->first_node("child");
                else
                    root_node = root_node->first_node("child2");


                List<string> instructions;
                xml_document<> tmpPr;
 
                for (xml_node<> * inst = root_node->first_node("action"); inst; inst = inst->next_sibling()){
                    if(inst == 0){
                        //cout << "no node" << endl;
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
                yield(childProcess); //call recursively 

                //removeNode(node);
                childProcess->state = TERMINANTED;
                DoublyList<PrBkCtr*>::node* tmp = findProcessNode(childProcess);

                terminatePr(tmp);
                //cout << "- child" << childProcess->PID << " of process " << w->PID << " has been terminated" << endl;
                }
                break;
            case TERMINANTED:
                break;
            default:
                //cout << "ERROR with child states" << endl;
                break;
        }
        w->childs.pop_back();
    }
    
    cout << "process " << w->PID << " has been terminated " << endl;
    w->state = TERMINANTED;                         //updating state to terminated
    M1->releaseFrames(w->pgTbl);
    
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