/*
ECGR 5181
Assignment 2: 4-stage pipelined CPU Architecture
Naseeruddin Lodge
*/

#include <iostream>
#include <string>
#include <vector>

using namespace std;


/*
Instruction Class - Can change and return values inside the instruction
Instruction(Instruction Name, Register, Register/offset, Regsiter/value)
Examples
("null", fld", "f0", "0", "x1")
("fetch", fsd", "f4", "f0", "f2")
("store", addi", "x1", "x1", "-4")
*/
class instruction{
    
    private:
        string stage;
        string instr;
        string reg;
        int val1;
        int val2;

    public:
        instruction(){}

        instruction(string stage, string instr, string reg, int val1, int val2){

            this->stage = stage;
            this->instr = instr;
            this->reg = reg;
            this->val1 = val1;
            this->val2 = val2;
        }

        void set_stage(string stage){
            this->stage = stage;
        }
            
        void set_instr(string instruction){
            this->instr = instr;
        }

        void set_reg(string register){
            this->reg = reg;
        }

        void set_val1(int val1){
            this->val1 = val1;
        }

        void set_val2(int val2){
            this->val2 = val2;
        }

        string get_stage(){
            return stage;
        }

        string get_instr(){
            return instr;
        }

        string get_reg(){
            return reg;
        }

        int get_val1(){
            return val1;
        }

        int get_val2(){
            return val2;
        }

};


/*
Global Variable - Makes it easier for multiple functions
to access these vectors and keep track of clock cycle
number.
Using vectors for four stages, instruction queue
and current clock cyle(used to display everything
in the current clock cycle)
*/
vector<instruction> InstructionQueue;
vector<instruction> Fetch;
vector<instruction> Decode;
vector<instruction> Execute;
vector<instruction> Store;
vector<instruction> Current_clock_cycle;
int x1=20, x2=0, clockCycle = 1;


/*
Reloads the instruction queue with instructions
when the instruction queue becomes empty.
*/
vector<instruction> Generate_InstructionQueue(){

    vector<instruction> InstructionQueue;

    //InstructionQueue = {bne, addi, fsd, fadd.d, fld}

    instruction I5("null", "bne","x1", 0, 0);
    InstructionQueue.push_back(I5);

    instruction I4("null", "addi","x1", 4, 20);
    InstructionQueue.push_back(I4);

    instruction I3("null", "fsd","f4", 0, 20);
    InstructionQueue.push_back(I3);

    instruction I2("null", "fadd.d","f4", 0, 20);
    InstructionQueue.push_back(I2);

    instruction I1("null", "fld","f0", 0, 20);
    InstructionQueue.push_back(I1);

    return InstructionQueue;
}

/*
Used to perform instructions and update therir values.
Will be used to print the values on screen
*/
instruction update_register(instruction instr, int Add){

    if(instr.get_instr() == "fld") instr.set_val2(Add);
    else if(instr.get_instr() == "fadd.d") instr.set_val2(Add);
    else if(instr.get_instr() == "fsd") instr.set_val2(Add);
    else if(instr.get_instr() == "addi") instr.set_val2(Add);
    else if(instr.get_instr() == "bne") {instr.set_val2(Add); instr.set_val1(0);}

    return instr;
}

/*
Used to increment stages
*/
instruction increment_stage(instruction instr){
    
    if(instr.get_stage() == "null") instr.set_stage("fetch");
    else if(instr.get_stage() == "fetch") instr.set_stage("decode");
    else if(instr.get_stage() == "decode") instr.set_stage("execute");
    else if(instr.get_stage() == "execute") instr.set_stage("store");

    return instr;
}

void increment_clock_cycle_stage(){

    for(int i=0 ; i<Current_clock_cycle.size() ; i++){

        if(Current_clock_cycle[i].get_stage() == "fetch") Current_clock_cycle[i].set_stage("decode");
        else if(Current_clock_cycle[i].get_stage() == "decode") Current_clock_cycle[i].set_stage("execute");
        else if(Current_clock_cycle[i].get_stage() == "execute") Current_clock_cycle[i].set_stage("store");
    }
}

/*
Used to print the instructions in the current clock cycle.
Also gets rid of instructions that are in the store stage.
*/
void print_Current_clock_cycle(){

    cout << endl << "Clock Cycle : " << clockCycle << endl; 

    for(int i=0 ; i<Current_clock_cycle.size() ; i++){

        cout << Current_clock_cycle[i].get_stage() << "[" << Current_clock_cycle[i].get_instr() << "]    "
             << Current_clock_cycle[i].get_reg() << "=" << Current_clock_cycle[i].get_val2() << endl;

        if(Current_clock_cycle[i].get_stage() == "store") Current_clock_cycle.erase(Current_clock_cycle.begin() + i);

    }

    clockCycle++;
    cout << endl;
}

/*
Responsible for adding and handling stalls
*/
int check_for_stalls(){

    int temp1=0, temp2=0, temp3=0, temp4=0, stall=0;
    
    for(int i=0 ; i<Current_clock_cycle.size() ; i++){

        if((Current_clock_cycle[i].get_stage() == "execute") && (Current_clock_cycle[i].get_instr() == "fld"))
            temp1 = 1;

        else if((Current_clock_cycle[i].get_stage() == "decode") && (Current_clock_cycle[i].get_instr() == "fadd.d"))
            temp2 = 1;

        else if((Current_clock_cycle[i].get_stage() == "execute") && (Current_clock_cycle[i].get_instr() == "fadd.d"))
            temp3 = 1;
        
        else if((Current_clock_cycle[i].get_stage() == "decode") && (Current_clock_cycle[i].get_instr() == "fsd"))
            temp4 = 1;

        if((temp1 == 1) && (temp2 == 1)) stall = 1;
        else if((temp3 == 1) && (temp4 == 1)) stall = 2;
        else stall = 0;
    }

    return stall;
}

/*
Adds stalls to the pipeline
*/
void stall(instruction instr, int no_of_stalls, int Add){
    
    if(no_of_stalls == 1){

        instruction stall1("null", "stall", "nope", 0, 0);
        Current_clock_cycle.insert(Current_clock_cycle.begin(), stall1);

        print_Current_clock_cycle();

        Current_clock_cycle.erase(Current_clock_cycle.begin());

        increment_clock_cycle_stage();
        instr = increment_stage(instr);
        instr = update_register(instr, Add);

        Current_clock_cycle.insert(Current_clock_cycle.begin(), instr);
        InstructionQueue.pop_back();

        print_Current_clock_cycle();

    }

    else if(no_of_stalls == 2){

        instruction stall1("null", "stall", "nope", 0, 0);
        instruction stall2("null", "stall", "nope", 0, 0);
        Current_clock_cycle.insert(Current_clock_cycle.begin(), stall1);
        Current_clock_cycle.insert(Current_clock_cycle.begin(), stall2);

        print_Current_clock_cycle();

        Current_clock_cycle.erase(Current_clock_cycle.begin());

        print_Current_clock_cycle();

        Current_clock_cycle.erase(Current_clock_cycle.begin());

        increment_clock_cycle_stage();
        instr = increment_stage(instr);
        instr = update_register(instr, Add);

        Current_clock_cycle.insert(Current_clock_cycle.begin(), instr);
        InstructionQueue.pop_back();

        print_Current_clock_cycle();
    }
    
}

/*
Logic for fetch
*/
void fetch(instruction instr, int i, int Add){
    
    instr = increment_stage(instr);
    instr = update_register(instr, Add);

    Current_clock_cycle.insert(Current_clock_cycle.begin(), instr);
    InstructionQueue.pop_back();

}


/*
Logic for decode
*/
void decode(instruction instr, int i, int Add){

    instr = increment_stage(instr);
    instr = update_register(instr, Add);

    Current_clock_cycle.insert(Current_clock_cycle.begin(), instr);
    InstructionQueue.pop_back();
}


/*
Logic for execute
*/
void execute(instruction instr, int i, int Add){

    instr = increment_stage(instr);
    instr = update_register(instr, Add);

    Current_clock_cycle.insert(Current_clock_cycle.begin(), instr);
    InstructionQueue.pop_back();
}


/*
Logic for store
*/
void store(instruction instr, int i, int Add){

    instr = increment_stage(instr);
    instr = update_register(instr, Add);

    Current_clock_cycle.insert(Current_clock_cycle.begin(), instr);
    InstructionQueue.pop_back();
}


/*
All the intrcutions are fed to null
and null handles pipelining using all
stages functions.
*/
void null(instruction instr, int Add){

    int x;
    
    if(Current_clock_cycle.empty() == true){
        
        instr = increment_stage(instr);
        instr = update_register(instr, Add);

        Current_clock_cycle.insert(Current_clock_cycle.begin(), instr);
        InstructionQueue.pop_back();
    }

    else if(Current_clock_cycle.empty() == false){
        
        increment_clock_cycle_stage();
        instr = increment_stage(instr);
        instr = update_register(instr, Add);

        Current_clock_cycle.insert(Current_clock_cycle.begin(), instr);
        InstructionQueue.pop_back();
    }

    print_Current_clock_cycle();
}


/*
Assissts the 4 stages to work in a pipeline manner
*/
void pipeline(int Add){

    int i=4, stalls;

    while(InstructionQueue.empty() == false){

        stalls = check_for_stalls();

        if(stalls == 0){

            if(InstructionQueue[i].get_stage() == "null") null(InstructionQueue[i], Add);
            else if(InstructionQueue[i].get_stage() == "fetch") fetch(InstructionQueue[i], i, Add);
            else if(InstructionQueue[i].get_stage() == "decode") decode(InstructionQueue[i], i, Add);
            else if(InstructionQueue[i].get_stage() == "execute") execute(InstructionQueue[i], i, Add);
            else if(InstructionQueue[i].get_stage() == "store") store(InstructionQueue[i], i, Add);
        }

        else if(stalls == 1) stall(InstructionQueue[i], stalls, Add);
        else if(stalls == 2) stall(InstructionQueue[i], stalls, Add);

        i--;
    }
}


/*
Main Function
*/
int main(){

    InstructionQueue = Generate_InstructionQueue();

    int MemAdd = 20;

    for(x1=20 ; x2<x1; x1=x1-4){

        pipeline(MemAdd);

        InstructionQueue = Generate_InstructionQueue();

        MemAdd = MemAdd - 4;
    }

    while(Current_clock_cycle.empty() == false){

        increment_clock_cycle_stage();
        print_Current_clock_cycle();
    }

    print_Current_clock_cycle();


    return 0;
}