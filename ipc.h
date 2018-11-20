#ifndef IPC_H
#define IPC_H

class ipc{
    public:
        unordered_map<int, deque<string>*> mailboxes;
        int id;
        ipc(){

        }

    
};


#endif