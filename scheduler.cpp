#include "scheduler.h"
#include <thread> 
#include <exception>

Sched::Sched(DoublyList<PrBkCtr*>* Q1, DoublyList<PrBkCtr*>* Q2, DoublyList<PrBkCtr*>* Q3, mem& A, ipc& MailB){ 
    queue1 = Q1;
    queue2 = Q2;
    queue3 = Q3;
    M1 = &A;
    MB = &MailB;

    numOfProcess = 0;

    servingQ = queue1;
}

void* Sched::running(){
    //run until Ready queues are empty

    DoublyList<PrBkCtr*>::node* head;
    PrBkCtr* currentPr = NULL;

    while((queue1->size > 0 ) || (queue2->size > 0)){


        pthread_mutex_lock( &mutex1 );
        if(!queue1->isEmpty()){
 
            head = queue1->getHead();
            headPr = *head->data;
            currentPr = head->data;
            queue1->popHead();

            currentPr->state = RUNNING;     //updating state to running
        }
        else if(!queue2->isEmpty()){

            head = queue2->getHead();
            headPr = *head->data;
            currentPr = head->data;
            queue2->deleteHead();

            currentPr->state = RUNNING;     //updating state to running
        }
        pthread_mutex_unlock( &mutex1 );

        if(currentPr != NULL &&  currentPr->queue == 1){
            if(runRR(QTIME, currentPr)){

                pthread_mutex_lock( &mutex1 );
                queue2->insertNode(currentPr);
                pthread_mutex_unlock( &mutex1 );

                currentPr->state = READY;
                MB->PrTable.insert({currentPr->PID, queue2->tail});
                currentPr->queue = 2;
            }
        }
        else if(currentPr != NULL && currentPr->queue == 2){
            if(runRR(QTIME2, currentPr)){

                pthread_mutex_lock( &mutex1 );
                queue2->insertNode(currentPr);
                pthread_mutex_unlock( &mutex1 );

                currentPr->state = READY;
                MB->PrTable.insert({currentPr->PID, queue2->tail});
                currentPr->queue = 2;
            }
        }

        pthread_mutex_lock( &mutex1 );
        ClearScreen();
        printMainMem();
        pthread_mutex_unlock( &mutex1 );

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
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

    cout << "   Showing only 240MB\n     of Main Mem.     PCB of Running Process " << endl;
    cout << setw(17) << "__________" << setw(22) << "____________" << endl;;
    

    for(int i = 0; i < 30; i++){
        cout << setw(4) << " 0x" << dec << i  + 16  << "| "<< setw(5) << dec << M1->mainMem[i][1]  << " | " << M1->mainMem[i][2]  <<" |";
        if(i < sizeOfPageTable){
            cout << setw(8) << dec << i    << "| "<< dec << headPr.pgTbl->entries[i][0]  << " | " << setw(4) << headPr.pgTbl->entries[i][1]  <<" |";
        }
        if(i == sizeOfPageTable){
            cout << setw(20)<< "------------";
        }
        if(i == 0){
            cout << "   OS using 16MB out of 2048 MB of memory";
        }
        else if(i == 1){
            cout << "   Running Process ID -> " << headPr.PID;
        }
        else if(i == 2){
            cout << "   Round Robin with qtime of 20us - # of Processes on it: " << queue1->size;
        }
        else if(i == 3){
            cout << "   Round Robin with qtime of 40us - # of Processes on it: " << queue2->size;
        }
        else if(i == 4){
            cout <<  "   Num. of Free Frames " << M1->freeFrames.size();
        }
        else if(i == 5){
            cout <<  "   Memory available: " <<  M1->freeFrames.size() * 8 << "MB";
        }
        else if(i == 6){
            cout << "   Num. of Processes in Main Mem. " << MAXNUMPR - M1->numProcess;
        }
        else if(i == 7){
            cout << "   Process Running the folling command:";
        }
        else if(i == 8){
            unsigned size = headPr.PC->data.size();
            string tmp = headPr.PC->data;
            tmp.resize(size - 2);
            cout << "   "<< tmp;
        }

        cout << endl;
    }
    cout << setw(17) << "----------" << endl;
    cout << "Enter -1 to exit simulation: " << endl;



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
    
    pthread_mutex_lock( &mutex1 );
    auto f = inst->first_attribute("pages");
    vector<int> pages;
    if(f != 0){
        string s = f->value();
        pages = M1->findPages(s);
        M1->mmu(process, pages);
    }
    pthread_mutex_unlock( &mutex1 );

    if((duration - burst) <= 0){
        //wait for QTIME - duration
        std::this_thread::sleep_for(std::chrono::microseconds(duration));


        instruction->data = updateTime(instruction->data, 0);

        string c = inst->value();

        pthread_mutex_lock( &mutex1 );
        if(c.compare("\"yield\"") == 0){    
            yield(process);
            (M1->numProcess)++;
            pthread_mutex_unlock( &mutex1 );
            return false;
        }
        pthread_mutex_unlock( &mutex1 );

        if(c.compare("fork") == 0){
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
        
        //cout << "current process: "<< process->PID << " of name: " << inst->value() << " has been terminated" << endl;
        process->PC = instruction->next;     //update PC

    }
    else{
        //wait for burst time
        std::this_thread::sleep_for(std::chrono::milliseconds(burst));
        int timep = duration - burst;
        instruction->data = updateTime(instruction->data, timep);
        
    }
    //M1->resetEntryTLB(pages);
    return true;
}

string Sched::updateTime(string instruction, int newTime){
    
    int i=0;
    bool flag = true;
    int first = 0;
    int second = 0;
    string nt = "";
    nt.append(to_string(newTime));
    nt.append("");
    for(char C : instruction){
        if(C == '\"'){
            if(flag){
                first = i;
                first++;
                flag = false;
            }
            else{
                second = i;
                second;
                break;
            }
        }
        i++;
    }

    instruction.replace(first, second - first, nt);

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
                PrBkCtr* pcb = new PrBkCtr(instructions.getHead(), w->rw_mutex, w->mutex);
                pcb->state = READY;              //change PCB state to READY
                pcb->parent = w->PID;
                pcb->sharedMem = w->sharedMem;
                w->parent = true;
                w->childs.push_back(pcb);

                pthread_mutex_lock( &mutex1 );
                queue1->insertNode(pcb);     //insert pcbs into READY QUEUE
                pthread_mutex_unlock( &mutex1 );
                
                //tmpIPC.mailboxes.insert({pcb->mailbox.id,&pcb->mailbox.messages});
                MB->mailboxes.insert({pcb->mailbox.id,&pcb->mailbox.messages});
                MB->PrTable.insert({pcb->PID, queue1->tail});

    return;
}


void Sched::yield(PrBkCtr* w){

    while(checkChilds(w)){
        PrBkCtr* childProcess = w->childs.back();           //get childs
        switch(childProcess->state) {
            //case
            case READY:
                {
                yield(childProcess); //call recursively 

            
                //DoublyList<PrBkCtr*>::node* tmp = findProcessNode(childProcess);

                //childProcess->state = TERMINANTED;
                //terminatePr(tmp);
                //cout << "- child" << childProcess->PID << " of process " << w->PID << " has been terminated" << endl;
                }
                break;
            case TERMINANTED:

                M1->frameTable[(short int ) childProcess->PID] = NULL;
                //delete childProcess;

                break;
            default:
                //cout << "ERROR with child states" << endl;
                break;
        }
        w->childs.pop_back();
    }
    
    if(w->state == READY){
        DoublyList<PrBkCtr*>::node* tmp = findProcessNode(w);
        terminatePr(tmp);
    }



    cout << "process " << w->PID << " has been terminated " << endl;
    w->state = TERMINANTED;                         //updating state to terminated
    for(int i =0 ; i < sizeOfPageTable; i++){
        if(w->pgTbl->entries[i][0]){
            M1->releaseFrames(w, i);
        }
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
        for(; i->data->PID != process->PID; i = i->next){
            //cout << i->data->PID << "-";
        }
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

