//Aaron Ng
//
//This is a simulation of how a CPU works regarding process handling and memory management
//The ready queue for the CPU uses priority queueing
//The device ready queues use First In First Out queueing
//note that this does not support more than 9 printers and more than 9 hard disks


#include "functions.h"


int main() {
    int process_counter = 0;
    
    //input declarations
    string entry;
    vector<vector<process>> printers;
    vector<vector<process>> hard_disks;
    vector<process> rqueue;
    vector<process> pid_history;
    
    
    //ask for initialized values
    cout << "How much memory is there?" << endl;
    cin >> entry;
    
    //double check if valid and if smaller than 4 billion
    while(!checkInt(entry) || stod(entry) > 4000000000){
        if(!checkInt(entry)) cout << "Please enter a valid amount" << endl;
        else if(stod(entry) > 4000000000)  cout << "Plese enter a smaller amount" << endl;
        
        cin >> entry;
    }
    memory CPUmem;
    CPUmem.left = 0;
    CPUmem.right = stod(entry)-1;
    memgaps.addHole(CPUmem);
    
    
    //ask for hard disks
    cout << "How many hard disks are there?" << endl;
    cin >> entry;
    
    //check if the integer input is valid
    while(!checkInt(entry)){
        cout << "Please enter a valid amount" << endl;
        cin >> entry;
    }
    
    //create a vector of hard disks
    for(int i = 0; i < stoi(entry); i++){
        vector<process> temp;
        hard_disks.push_back(temp);
    }
    
    
    
    //ask for number of printers
    cout << "How many printers are there?" << endl;
    cin >> entry;
    
    //check if the integer input is valid
    while(!checkInt(entry)){
        cout << "Please enter a valid amount" << endl;
        cin >> entry;
    }
    
    //create a vector of printers
    for(int i = 0; i < stoi(entry); i++){
        vector<process> temp;
        printers.push_back(temp);
    }
    
    
    //ask for events in the CPU
    while(entry != "exit"){
        cout << "Please enter an event" << endl << "Note the commands are:" << endl
        << "'A' to add a new process" << endl
        << "'t' to terminate the process currently using the CPU" << endl
        << "'Sr' to see a snap-shot of the CPU's ready queue and the state of the ready queue" << endl
        << "'Si' to see a snap-shot of the i/o devices' ready queues" << endl
        << "'Sm' to show the memory usage" << endl
        << "'d(followed by a number between 1 and " << hard_disks.size() << ")' to add a process to a hard disk" << endl
        << "'p(followed by a number between 1 and " << printers.size() << ")' to add a process to a printer" << endl
        << "'D(followed by a number between 1 and " << hard_disks.size() << ")' to remove a process from a hard disk and add it back to the CPU ready queue" << endl
        << "'P(followed by a number between 1 and " << printers.size() << ")' to remove a process from a printer and add it back to the CPU ready queue" << endl
        << "You may also enter 'exit' or 'stop' to end the program" << endl;
        cin >> entry;
        
        
        
        //new process
        if(entry == "A"){
            process A;
            process_counter++;
            A.pid=process_counter;
            
            //get the size of the process
            cout << "enter the size of the process " << endl;
            cin >> entry;
            
            //check if integer input is valid
            while(!checkInt(entry)){
                cout << "Please enter a valid amount" << endl;
                cin >> entry;
            }
            A.size = stod(entry);
            
            
            //get the priority
            cout << "enter the priority of the process" << endl;
            cin >> entry;
            
            //check if integer input is valid
            while(!checkInt(entry)){
                cout << "Please enter a valid amount" << endl;
                cin >> entry;
            }
            A.priority = stoi(entry);
            
            if(memgaps.search(A.size)==false){
                cout << "this process is too large for memory" << endl;
                process_counter--;
            }
            
            else{
                A.mem.left = memgaps.fillhole(A.size);
                A.mem.right = A.mem.left + A.size - 1;
                
                //check if the CPU is being used. If not then allocate the CPU to the new process
                if(CPU.taken==false){
                    CPU.taken = true;
                    CPU.current = A;
                }
                //if it is being used by a different process then check if preemption is necessary
                else if(CPU.current.priority > A.priority){
                    rqueue.push_back(CPU.current);
                    CPU.current = A;
                }
                //if not then place into ready queue
                else{
                    rqueue.push_back(A);
                }
                cout << "ADDED" << endl;
                
                pid_history.push_back(A);
            }
        }
        
        
        
        //process termination
        else if(entry == "t"){
            if(CPU.current.pid == 0){
                cout << "nothing to terminate" << endl;
            }
            else{
                int position = 0;
                
                //create a hole in memory where the running process was
                memgaps.addHole(CPU.current.mem);
                
                //set process to terminated in pid_history
                for(int i = 0; i < pid_history.size(); i++){
                    if(CPU.current.pid == pid_history[i].pid){
                        pid_history[i].terminated = true;
                    }
                }
                
                //check priorities within the ready queue and get the process with the lowest priority
                if(rqueue.size() != 0){
                    
                    //get the process with the smallest priority and its position
                    process temp = preempt(rqueue, position);
                    
                    //if the process is not in the last position on the ready queue, swap it
                    if (temp.pid != rqueue[rqueue.size()-1].pid){
                        swap(rqueue[position], rqueue[rqueue.size()-1]);
                    }
                    //pop off the last element
                    rqueue.pop_back();
                    
                    //set the CPU to the highest priority element
                    CPU.current = temp;
                }
                else{
                    //if the ready queue is empty make the CPU free
                    CPU.taken = false;
                    
                    //use the fact that a process is constructed with all perameters == 0
                    process temp;
                    CPU.current = temp;
                }
                cout << "PROCESS IN CPU HAS TERMINATED" << endl;
            }
        }
        
        
        
        //show ready queue for CPU
        else if(entry == "Sr"){
            if(rqueue.size() == 0){
                cout << "The ready queue is empty" << endl;
            }
            for(int i = 0; i < rqueue.size(); i++){
                cout << "pid" << rqueue[i].pid << endl;
            }
            cout << endl;
            
            if(CPU.taken == false){
                cout << "the CPU is empty" << endl;
            }
            else{
                cout << "CPU: pid" << CPU.current.pid << endl;
            }
        }
        
        
        
        //show input/output devices/queue
        else if(entry == "Si"){
            for(int i = 0; i < printers.size(); i++){
                if(printers[i].size() == 0)
                    cout << "No processes available for p" << i+1 << endl;
                else{
                    for(int j = 0; j<printers[i].size(); j++){
                        cout << "p" << i+1 << ": " << "pid" << printers[i][j].pid << endl;
                    }
                }
            }
            for(int i = 0; i < hard_disks.size(); i++){
                if(hard_disks[i].size() == 0)
                    cout << "No processes available for d" << i+1 << endl;
                else{
                    for(int j = 0; j<hard_disks[i].size(); j++){
                        cout << "d" << i+1 << ": " << "pid" << hard_disks[i][j].pid << endl;
                    }
                }
            }
        }
        
        
        
        //show processes and memory addresses
        else if(entry == "Sm"){
            int smallest = 0 , j = 0;
            bool temp = true, something = false;
            
            //while there are still processes in memory that haven't been checked
            while (temp == true){
                temp = false;
                
                //create a variable that will hold the next smallest value
                int smallest2 = CPUmem.right;
                
                //iterate through the vector
                for(int i = 0; i < pid_history.size(); i++){
                    
                    //if a non terminates process at a larger memory address is found
                    if(pid_history[i].terminated == false && pid_history[i].mem.left >= smallest){
                        temp = true;
                        something = true;
                        
                        //cout the data
                        if(pid_history[i].mem.left == smallest){cout << pid_history[i].mem.left << "–" << pid_history[i].mem.right << ":  pid" << pid_history[i].pid << endl;}
                        //find the next smallest
                        else if(pid_history[i].mem.left < smallest2) {smallest2 = pid_history[i].mem.left;}
                        
                    }
                }//end of for loop
                smallest = smallest2;
                
            }//end of while loop
            if(something == false){
                cout << "There is curently nothing in memory" << endl;
            }
		
	    //read the gaps of memory
	    cout << "The gaps are:" << endl;
	    for(int i = 0; i < memgaps.getsize(); i++){
		cout << memgaps.getgap(i).left << "–" << memgaps.getgap(i).right << endl;
	    }
	    cout << endl;
        }
        
        
        
        //access printer in value n starting at 1
        else if(entry[0] == 'p'){
            if(entry[1] == '0'){
                cout << "There is no p0" << endl;
            }
            else if(isalpha(entry[1])) {
                cout << "Please enter a valid event" << endl;
            }
	    else if(CPU.current.pid == 0){
		cout << "There is no process in the CPU to add" << endl;
	    }
            else{
		//ask the process for the name of a file
		cout << "Please enter a file name" << endl;
		cin >> CPU.current.filename;

                //get printer number
                int printer = entry[1]-'0';
                
                //place running process into the CPU
                printers[printer-1].push_back(CPU.current);
                
                
                int position = 0;
                
                //check priorities within the ready queue and get the process with the lowest priority
                if(rqueue.size() != 0){
                    
                    //get the process with the smallest priority and its position
                    process temp = preempt(rqueue, position);
                    
                    //if the process is not in the last position on the ready queue, swap it
                    if (temp.pid != rqueue[rqueue.size()-1].pid){
                        swap(rqueue[position], rqueue[rqueue.size()-1]);
                    }
                    //pop off the last element
                    rqueue.pop_back();
                    
                    //set the CPU to the highest priority element
                    CPU.current = temp;
                }
                else{
                    //if the ready queue is empty make the CPU free
                    CPU.taken = false;
                    
                    //use the fact that a process is constructed with all perameters == 0
                    process temp;
                    CPU.current = temp;
                }
		cout << "TRANSFERED PROCESS TO A HARD DISK" << endl;
            }
            
        }
        
        
        
        //access hard disk in value n starting at 1
        else if(entry[0] == 'd'){
            if(entry[1] == '0'){
                cout << "There is no d0" << endl;
            }
            else if(isalpha(entry[1])) {
                cout << "Please enter a valid event" << endl;
            }
	    else if(CPU.current.pid == 0){
		cout << "There is no process in the CPU to add" << endl;
	    }
            else{
		//ask the process for a name of a file
		cout << "please enter a file name" << endl;
		cin >> CPU.current.filename;

                //get hard disk number
                int hard_disk = entry[1]-'0';
                
                //place running process into the CPU
                hard_disks[hard_disk-1].push_back(CPU.current);
                
                
                int position = 0;
                
                //check priorities within the ready queue and get the process with the lowest priority
                if(rqueue.size() != 0){
                    
                    //get the process with the smallest priority and its position
                    process temp = preempt(rqueue, position);
                    
                    //if the process is not in the last position on the ready queue, swap it
                    if (temp.pid != rqueue[rqueue.size()-1].pid){
                        swap(rqueue[position], rqueue[rqueue.size()-1]);
                    }
                    //pop off the last element
                    rqueue.pop_back();
                    
                    //set the CPU to the highest priority element
                    CPU.current = temp;
                }
                else{
                    //if the ready queue is empty make the CPU free
                    CPU.taken = false;
                    
                    //use the fact that a process is constructed with all perameters == 0
                    process temp;
                    CPU.current = temp;
                }
		cout << "TRANSFERED PROCESS TO A HARD DISK" << endl;
            
            }

        }
        
        
        
        //resolve a process in a printer value n
        else if(entry[0] == 'P'){
            if(entry[1] == '0'){
                cout << "there is no p0" << endl;
            }
            else if(isalpha(entry[1])) {
                cout << "please enter a valid event" << endl;
            }
            else{
                //get printer number
                int printer = entry[1]-'0';
                printer = printer - 1;
                if(printers[printer].size() == 0){
                    cout << "no available process" << endl;
                }
                else if(printers.size() < printer){
                    cout << "that printer does not exist" << endl;
                }
                else{
                    //get process in first position of the queue of processes
                    process temp = fifo(printers[printer]);
		    cout << "file: " << temp.filename << " has been printed" << endl;
                    
                    //check if the CPU is being used. If not then allocate the CPU to the new process
                    if(CPU.taken==false){
                        CPU.taken = true;
                        CPU.current = temp;
                    }
                    //if it is being used by a different process then check if preemption is necessary
                    else if(CPU.current.priority > temp.priority){
                        rqueue.push_back(CPU.current);
                        CPU.current = temp;
                    }
                    //if not then place into ready queue
                    else{
                        rqueue.push_back(temp);
                    }
                }
            }
            
        }
        
        
        
        //resolve a process in hard disk value n
        else if(entry[0] == 'D'){
            if(entry[1] == '0'){
                cout << "there is no d0" << endl;
            }
            else if(isalpha(entry[1])) {
                cout << "please enter a valid event" << endl;
            }
            else {
                int hard_disk = entry[1]-'0';
                hard_disk = hard_disk - 1;
                
                if(hard_disks[hard_disk].size() == 0){
                    cout << "no available process" << endl;
                }
                else if(hard_disks.size()<hard_disk){
                    cout << "that hard disk does not exist" << endl;
                }
                else{
                    //get process in first position of the queue of processes
                    process temp = fifo(hard_disks[hard_disk]);
		    cout << "file: " << temp.filename << " has been removed from the hard disk" << endl;
                    
                    
                    //check if the CPU is being used. If not then allocate the CPU to the new process
                    if(CPU.taken==false){
                        CPU.taken = true;
                        CPU.current = temp;
                    }
                    //if it is being used by a different process then check if preemption is necessary
                    else if(CPU.current.priority > temp.priority){
                        rqueue.push_back(CPU.current);
                        CPU.current = temp;
                    }
                    //if not then place into ready queue
                    else{
                        rqueue.push_back(temp);
                    }
                }
            }

        }
        
        
        
        //check if exiting the program
        else if(entry == "exit" || entry =="stop") break;
        
        //check if there was an event typo
        else{
            cout << "please enter a valid event" << endl;
        }
        
        cout << endl;
    }
    
    
    cout << "Thank you for using this simulation of a CPU" << endl;
    
    return 0;
}
