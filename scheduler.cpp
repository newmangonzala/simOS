#include "scheduler.h"
#include <thread> 

Sched::Sched(List<PrBkCtr*>& Q1, mem& A, ipc& MailB){ 
    queue1 = &Q1;
    M1 = &A;
    MB = &MailB;
}
//Sched::Sched(){}

int Sched::manage(){
    return 1;
}

//move current process to the back of the queue
void Sched::updateQ(){

    List<PrBkCtr*>::node* head = queue1->getHead();
    List<PrBkCtr*>::node* tail = queue1->getTail();
    List<PrBkCtr*>::node* prev = head->next;
    

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


void Sched::running2(){
//run until Ready queue is empty
    while(!queue1->isEmpty()){

        List<PrBkCtr*>::node* head = queue1->getHead();
        PrBkCtr* w = head->data;
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
        

        auto f = inst->first_attribute("pages");
        string s = f->value();
        vector<int> pages = findPages(s);

        
        mmu(w, pages);

        //inst->remove_first_attribute();
        if((duration - qtime) <= 0){
            //wait for qtime - duration
            std::this_thread::sleep_for(std::chrono::microseconds(duration));
            b->replace(14,2,"00");
            
            cout << "current process: "<< w->PID << " of name: " << inst->value() << " has been terminated" << endl;
            
            //check if instruction was the last from process

            toParse = r->next->data;
            doc.parse<0>(&(toParse)[0]);
            inst = doc.first_node();
            string c = inst->value();
            //cout << c << endl;
            if(c.compare("\"yield\"") == 0){

                yield(w);
                continue;
            }
          
            c = inst->name();
            if(c.compare("fork") == 0){

                fork(w);

            }
            /*
            else if(c.compare("send") == 0){
                //cout << M1->numProcess << endl;
                string tmpmail = inst->first_attribute("mailbox")->value();
                cout << "child of current pr is " << w->childs[0]->PID << endl;
                if(tmpmail.compare("child") == 0){
                    auto search = M1->mailboxes.find(w->childs[0]->PID);
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
            */
            
            w->PCtmp = r->next;     //update PC

        }
        else{
            //wait for qtime
            std::this_thread::sleep_for(std::chrono::milliseconds(qtime));
            int timep = duration - qtime;
            string tn = to_string(timep);
            b->replace(14,2,tn);            
        }
        //move current process to the back of the queue
        updateQ();
        

    }
    

    return;
}


void Sched::fork(PrBkCtr* w){

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
                w->childs.push_back(&pcb);
                queue1->insertNode(&pcb);     //insert pcbs into READY QUEUE
                //M1->mailboxes.insert({pcb.mailbox->id,&pcb.mailbox->messages});

    return;
}

void Sched::yield(PrBkCtr* w){

    if(w->childs.size() > 0){
        auto childProcess = w->childs[0];           //get childs
        List<PrBkCtr*>::node* prev = queue1->getHead();         //get current process
        List<PrBkCtr*>::node* tmpPr = prev->next;
        PrBkCtr* tmp = tmpPr->data;

        while(tmp != childProcess){                 //iterate through each process to find child
            prev = tmpPr;
            tmpPr = tmpPr->next;
            tmp = tmpPr->data;
            
        }
        
        tmp->state = TERMINANTED;               //updating state of child to terminated
        cout << "child of parent " << w->PID << " with id of " << tmp->PID << " has been terminated" << endl; 
        prev->next = tmpPr->next;

        delete tmpPr;


    }

    w->state = TERMINANTED;                         //updating state to terminated

    queue1->deleteHead();

    cout << "process " << w->PID << " has been terminated" << endl;


    

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