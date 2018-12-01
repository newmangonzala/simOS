#include "memory.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

using namespace rapidxml;


mem::mem(int numPr){
    numProcess = numPr;
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

}


//loader
void mem::loadApps(){

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

    //xml_node<> * inst = app->first_node();

    
    


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
        memOfProcesses.push_back(instructions);

    
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

void mem::loadPCBs(){

    xml_document<> doc;


    for(int i = 0; i < memOfProcesses.size(); i++){

        auto listOfInst = memOfProcesses[i];
        
        List<mem::instrucion> app1;

        for(List<std::string>::node* tmp = listOfInst.getHead(); tmp != NULL; tmp = tmp->next){
            auto Sinst = tmp->data;

            //std::cout << "-----\n" << Sinst << std::endl;
/*
            doc.parse<0>(&Sints[0]);

            
            instrucion I;

            //iterate throught the app to get all the instructions
            for (xml_node<> * inst = doc.first_node(); inst; inst = inst->next_sibling()){
                
                I.type = inst->value();

                rapidxml::xml_attribute<char>* t = inst->first_attribute("time");

                if(t != 0){
                    I.time = stoi(t->value());
                }

                app1.insertNode(I);
            }
            */

        }

        


        
        qOfPr.push_back(app1);
        
    }

    return;
}


void mem::releaseFrames(pageTable* currentProcess){
    for(int i = 0; i < currentProcess->entries.size(); i++){
        if(currentProcess->entries[i][0]){
            freeFrames.push(currentProcess->entries[i][1]);
            currentProcess->entries[i][0] = 0;
        }
    }
}


void mem::mmu(PrBkCtr* process, vector<int> pages){

    for(int i = 0; i < pages.size(); i++){
                
        //FIX check for segmentation fault here


        //cout << "Process id " << process->PID << " page: " << pages[i] << " bit: " << process->pgTbl->entries[pages[i]][0] << endl;

        if(!lookUpTLB(process->PID, pages[i])){ //Page is not in TLB
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
    }

    return;
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
    if(freeFrames.size() > 0){
        
        auto freeFrame = freeFrames.front();
        freeFrames.pop();                               //pop free frame
        process->pgTbl->entries[page][1] = freeFrame;         //map frame to page in page table
        
        mainMem[freeFrame][0] = 1;                //change second chance bit to 1
        mainMem[freeFrame][1] = process->PID;           //store process id into frame, just for reference
        mainMem[freeFrame][2] = page;

        //frametable
        frameTable[freeFrame].push_back(process);

        //TLB
        TLB[page][0] = 1;
        TLB[page][1] = process->PID; 
        TLB[page][2] = page;
        
    }
    else{
        // replace frames because there are not free frames
        while(mainMem[currentFrameIndex][0]){                           //find frame with second chance bit 0
            mainMem[currentFrameIndex][0] = 0;                //change second chance bit to 0
            currentFrameIndex = currentFrameIndex++ % numFrames;
            continue; 
            
        }

        //update page tables of process store in frame
        PrBkCtr* tmp;
        for(int i = 0; i < frameTable[currentFrameIndex].size(); i++){
            tmp = frameTable[currentFrameIndex].back();
            frameTable[currentFrameIndex].pop_back();
            releaseFrames(tmp->pgTbl);
        }
        
        mainMem[currentFrameIndex][0] = 1;                  //second chance bit set
        mainMem[currentFrameIndex][1] = process->PID;                    //replace page in current frame
        mainMem[currentFrameIndex][2] = page; 
        //no need to set second chance to 0 since it was zero
        //update table FIX ME
        process->pgTbl->entries[page][1] = currentFrameIndex;         //map frame to page in page table
        
    }
    process->pgTbl->entries[page][0] = 1;                 //change valid bit to 1, meaning is now in Mmem
    currentFrameIndex = currentFrameIndex++ % numFrames;        //Increase index of mem
}

void mem::resetEntryTLB(vector<int> pages){
    for(int i=0; i < pages.size(); i++){
        TLB[i][0] = 0;
    }
}