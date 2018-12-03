#include "memory.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

using namespace rapidxml;


mem::mem(int numPr){
    //numProcess = numPr;
    for(int i = 0 ; i < numFrames; i++){
        freeFrames.push(i);
        //second chance bit , PID, page number
        mainMem[i] = new short int[3];
        mainMem[i][1] = -1;
    }
    for(int i = 0 ; i < maxNumPages; i++){
        //Valid bit, Process ID, Frame number
        TLB[i] = new short int[4];
        TLB[i][0] = 0;
    }
    currentFrameIndex = 0;

    numProcess = MAXNUMPR;

}

void mem::createHD(){
      //open file of dummy proccesses 
    ifstream file("process.xml");

    xml_document<> doc;
    xml_node<> * root_node;

    vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    //make sure to zero terminate the buffer
    buffer.push_back('\0');

    //parse buffer
    doc.parse<0>(&buffer[0]);

    //access the root node
    root_node = doc.first_node("Processes");


    xml_document<> tmpPr;
     
    instrucion I1; 
    vector<instrucion> app4;


    for(xml_node<> * app = root_node->first_node("app"); app; app = app->next_sibling("app")){
        
        List<string> instructions;
        
        for (xml_node<> * inst = app->first_node(); inst; inst = inst->next_sibling()){
            xml_node<char>* SInst = tmpPr.clone_node(inst);

            tmpPr.append_node(SInst);
            string xml_as_string = "";
            print(std::back_inserter(xml_as_string), tmpPr);
            //std::cout << "-----\n" << xml_as_string << std::endl;

            instructions.insertNode(xml_as_string);

            tmpPr.remove_first_node();

        }
    
        //load proccess into memory
        listOfProcesses.push_back(instructions);

    }

    for(xml_node<> * app = root_node->first_node("child"); app; app = app->next_sibling("child")){
        
        List<string> instructions;
        
        for (xml_node<> * inst = app->first_node(); inst; inst = inst->next_sibling()){
            xml_node<char>* SInst = tmpPr.clone_node(inst);

            tmpPr.append_node(SInst);
            string xml_as_string = "";
            print(std::back_inserter(xml_as_string), tmpPr);
            //std::cout << "-----\n" << xml_as_string << std::endl;

            instructions.insertNode(xml_as_string);

            tmpPr.remove_first_node();

        }
    
        //load proccess into memory
        listOfProcessesChilds.push_back(instructions);
    }

    for(xml_node<> * app = root_node->first_node("child2"); app; app = app->next_sibling("child2")){
        
        List<string> instructions;
        
        for (xml_node<> * inst = app->first_node(); inst; inst = inst->next_sibling()){
            xml_node<char>* SInst = tmpPr.clone_node(inst);

            tmpPr.append_node(SInst);
            string xml_as_string = "";
            print(std::back_inserter(xml_as_string), tmpPr);
            //std::cout << "-----\n" << xml_as_string << std::endl;

            instructions.insertNode(xml_as_string);

            tmpPr.remove_first_node();

        }
    
        //load proccess into memory
        listOfProcessesChilds2.push_back(instructions);
    }
    

    file.close();

    return;
}



void mem::linker(){

    //open file of dummy proccesses 
    ifstream file("process.xml");

    xml_document<> doc;
    xml_node<> * root_node;

    vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    //make sure to zero terminate the buffer
    buffer.push_back('\0');

    //parse buffer
    doc.parse<0>(&buffer[0]);

    //access the root node
    root_node = doc.first_node("Processes");


    xml_document<> tmpPr;
     
    instrucion I1; 
    vector<instrucion> app4;



    xml_node<> * app = root_node->first_node("app");


    for (int i = 1; i <= numProcess; i++){

        List<string> instructions;
        
        for (xml_node<> * inst = app->first_node(); inst; inst = inst->next_sibling()){
            xml_node<char>* SInst = tmpPr.clone_node(inst);

            tmpPr.append_node(SInst);
            string xml_as_string = "";
            print(std::back_inserter(xml_as_string), tmpPr);
            //std::cout << "-----\n" << xml_as_string << std::endl;

            instructions.insertNode(xml_as_string);

            tmpPr.remove_first_node();

        }
        //std::cout << "/////////////////////////////////////////\n"  << std::endl;

        //load proccess into memory
        //memOfProcesses.push_back(instructions);

    
        //if last app then restart
        if(app->next_sibling("app") == NULL){
            app = root_node->first_node("app");
        }
        else{
            app = app->next_sibling("app");
        }
        
    }
    

    file.close();

    return;
}



void mem::releaseFrames(PrBkCtr* w, int page){
    pageTable* currentProcess = w->pgTbl;

    cout <<  page<< " page to be replaced from PID " << w->PID << endl; 


    int j = currentProcess->entries[page][1];
    if(j != -1){
        freeFrames.push(j);
        mainMem[j][1] = -1;
    }

    currentProcess->entries[page][0] = 0;

    frameTable[j] = NULL;

}


void mem::mmu(PrBkCtr* process, vector<int> pages){

    for(int i = 0; i < pages.size(); i++){
                
        //FIX check for segmentation fault here


        //cout << "Process id " << process->PID << " page: " << pages[i] << " bit: " << process->pgTbl->entries[pages[i]][0] << endl;

        //if(!lookUpTLB(process->PID, pages[i])){ //Page is not in TLB
        if(true){ //Page is not in TLB
            if(loopUpPageTlb(process, pages[i])){
                auto frame = process->pgTbl->entries[pages[i]][1];    //get location of page in Mmem; get frame
                if(!mainMem[frame][0]){
                    mainMem[frame][0] = 1;                //change second chance bit to 1
                }
            }
            else{
                pageSwapper(process, pages[i]);
            }
        }
        
        //cout << "p " << pages[i] << endl;
    }

    return;
}

vector<int> mem::findPages(string s){

    vector<int> pages;

    stringstream ss;

    ss << s;

    string temp;

    int page;

    while(!ss.eof()){

        ss >> temp;
        if(stringstream(temp) >> page){
            pages.push_back(page);
        } 
        temp = "";
    }
    return pages;
}

bool mem::lookUpTLB(int PID, int page){

    ///TLB[entry][Valid bit, Process ID, Frame number]
    if(TLB[page][0] && (TLB[page][1] == PID)){ //page is in TLB
        return true; 
    }
    else
        return false;
}
bool mem::loopUpPageTlb(PrBkCtr* process, int page){
    if(process->pgTbl->entries[page][0]) //valid bit is 1; meaning is in memory
        return true;
    else
        return false;
}



void mem::pageSwapper(PrBkCtr* process, int page){

    cout << "free frames : " << freeFrames.size() << " front "<< freeFrames.front() << endl;

    if(freeFrames.size() > 0){

 
         
        short int freeFrame = freeFrames.front();
        freeFrames.pop();                               //pop free frame
        process->pgTbl->entries[page][1] = freeFrame;         //map frame to page in page table
        
        mainMem[freeFrame][0] = 1;                //change second chance bit to 1
        mainMem[freeFrame][1] = process->PID;           //store process id into frame, just for reference
        mainMem[freeFrame][2] = page;

        //frametable
        frameTable[freeFrame] = process;

        //TLB
        /*
        TLB[page][0] = 1;
        TLB[page][1] = process->PID; 
        TLB[page][2] = page;
        */
        
    }
    else{
        // replace frames because there are not free frames
        while(mainMem[currentFrameIndex][0]){                           //find frame with second chance bit 0
            mainMem[currentFrameIndex][0] = 0;                //change second chance bit to 0
            currentFrameIndex = (currentFrameIndex + 1) % numFrames;
            continue; 
            
        }

        //update page tables of process store in frame
        PrBkCtr* tmp;
        tmp = frameTable[currentFrameIndex];

        if(tmp != NULL)
            releaseFrames(tmp, mainMem[currentFrameIndex][2]);

        
        mainMem[currentFrameIndex][0] = 1;                  //second chance bit set
        mainMem[currentFrameIndex][1] = process->PID;                    //replace page in current frame
        mainMem[currentFrameIndex][2] = page;
        frameTable[currentFrameIndex] = process;
        freeFrames.pop();
        //no need to set second chance to 0 since it was zero
        //update table FIX ME
        process->pgTbl->entries[page][1] = currentFrameIndex;         //map frame to page in page table
        
    }
    process->pgTbl->entries[page][0] = 1;                 //change valid bit to 1, meaning is now in Mmem
    currentFrameIndex = (currentFrameIndex + 1) % numFrames;        //Increase index of mem
}

void mem::resetEntryTLB(vector<int> pages){

    for(int i=0; i < pages.size(); i++){
        TLB[i][0] = 0;
    }
}