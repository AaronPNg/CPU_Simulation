//Aaron Ng
//
//The structs for the program
//

//Memory marker
struct memory{
    int left=0;
    int right=0;
};



//Essentially a PCB
struct process{
    int pid=0;
    double size=0;
    int priority=0;
    memory mem;
    bool terminated = false;
    string filename = "";
};



//The CPU that should have only one process in it if there is/are a process/processes
struct CPU{
    bool taken = false;
    process current;
}CPU;





//Gaps in memory
class Holes{
public:
    
    //Create a gap in memory
    void addHole(memory x){
        holes.push_back(x);
        //iterate through all of the current holes in memory if more than 1 hole exists
        for(int i = 0; i < holes.size();i++){
            
            //check each hole to every other current hole and if 2 holes are adjacent then merge holes
            for(int j = 0; j<holes.size(); j++){
                
                if(holes[i].right + 1 == holes[j].left){
                    holes[i].right = holes[j].right;
                    swap(holes[j],holes[holes.size()-1]);
                    holes.pop_back();
                    
                    //Reset the iterator to the "front" of the list to start checking for gap adjacentcy
                    i = 0;
                }
            }
        }
    }
    
    
    //check if there is a gap in memory that can contain the size of the passed memory
    bool search(int x){
        for(int i = 0; i < holes.size(); i++){
            if((holes[i].right - holes[i].left + 1) > x) return true;
        }
        return false;
    }
    
    
    //Find a hole for memory with the assumption there is a hole in memory large enough
    int fillhole(int x){
        
        int best = holes[0].right - holes[0].left + 1;
        int j = 0, left = holes[0].left;
        
        //Iteratae through to find the "best-fit" memory gap
        for(int i = 1;i < holes.size();i++){
            int current = holes[i].right-holes[i].left + 1;
            if(current >= x && current < best){
                best = current;
                j = i;
                left = holes[i].left;
            }
        }
        
        //Adjust the found hole so it will keep track of the memory change and placement of the process into memory
        holes[j].left = holes[j].left + x;
        
        //Return the starting point in the memory gap
        return left;
        
    }
    
    //Get a specific memory gap
    memory getgap(int x){
        return holes[x];
    }
    
    //Get the number of holes currentlt in the simulation so far
    int getsize(){
        return holes.size();
    }
    
    
private:
    vector<memory> holes;
    
    //Declare an object
}memgaps;


