
#include <iostream>
#include <vector>
using namespace std;

#include "objects.h"



//check the string char by char if it is an integer
bool checkInt(string x){
    for(int i = 0; i < x.size(); i++ ){
        if(isalpha(x[i])){
            return false;
        }
    }
    return true;
}


//implement highest priority queueing
process preempt(vector<process> x, int &y){
    process temp;
    temp = x[0];
    for (int i = 0; i < x.size(); i++){
        //iterate through the queue to find the one with the smallest/highest priority
        if (temp.priority > x[i].priority){
            temp = x[i];
            y=i;
        }
    }
    
    return temp;
}

//implement First In First Out queueing
process fifo(vector<process> &x){
    process temp = x[0];
    for(int i = 0; i < x.size()-1; i++){
        x[i] = x[i+1];
    }
    x.pop_back();
    
    return temp;
}


