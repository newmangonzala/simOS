#ifndef IPC_H
#define IPC_H

class ipc{
    public:
        unordered_map<int, List<string>*> mailboxes;

        unordered_map<int, DoublyList<PrBkCtr*>::node*> PrTable;
        unordered_map<int, int> PrTableState;

        int id;
        ipc(){

        }

    
};


#endif