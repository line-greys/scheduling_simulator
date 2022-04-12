#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include <cmath>
#include "process.cpp"

using namespace std;

vector<int> split_line(string);
void print(vector<int>&);
string trim(const string&);
int count_digit(int);
void print_table(int n, string &space1, string &space2);
void print_processes(vector<Process*>&, int);
void simulate_fifo(vector<int>&, vector<Process*>&);
void simulate_rr(vector<int>&, vector<Process*>&);
void SimulateCycle(vector<Process*>&, vector<Process*>&, Process*&,int&);

int main(){

    //read and parse input
    string input;
    cout << "Enter parameters: ";
    getline(cin, input);
    input = trim(input);
    vector <int> args = split_line(input);
    if(args.size()!=args[0]*4+2 && args.size()!=args[0]*4+3){
        cout << "Invalid input size." << endl;
        return 1;
    }

    //create a list of processes 
    vector<Process*> processes;
    for(int i = args[1]+1; i < args.size(); i = i + 4){
        Process *process = new Process((i-2)/4+1,args[i],args[i+1],args[i+2],args[i+3]);
        cout << "\nProcess " << (i-2)/4+1 << ": ";
        vector<int> values = process->getValues();
        print(values);
        processes.push_back(process);
    }

    //start simulation
    if (args[1] == 1){
        simulate_fifo(args, processes);
    } else if(args[1] == 2) {
        simulate_rr(args, processes);
    } else {
        cout << "Unknown parameter." << endl;
        return 1;
    }
    cout << endl;
    return 0;
}

void simulate_fifo(vector<int> &args, vector<Process*> &processes){
    
    cout << "\n\nSimulating FIFO\n" << endl;

    //print table and first row, define length of columns
    string text1 = "Blocked (99 of 99)";
    string text2 = "Process 9";
    string text3 = "Cycle";
    string space1(count_digit(args[0]) + 1, ' ');
    string space2(text1.length()-text2.length(), ' ');
    print_table(args[0],space1,space2);
    int length1 = text3.length()+count_digit(args[0])+1;
    int length2 = text1.length();
    cout << 0 << string(length1-1, ' ');
    print_processes(processes, length2);
    
    int num_processes = args[0];
    int cycle = 1;
    vector<Process*> Ready(processes); //Ready Queue contains all processes
    Process *Running = nullptr;

    //loop until all processes have terminated
    while(num_processes > 0){
        if(!Running){ //if no process is running, run one from the ready list
            if(Ready.size()>0){
                int state = Ready[0]->run();
                Running = Ready[0];
                Ready.erase(Ready.begin());
            }
        }
        SimulateCycle(processes, Ready, Running, num_processes);
        cout << cycle << string(length1-count_digit(cycle), ' ');
        print_processes(processes,length2);
        cycle++;
        
    }

}

void simulate_rr(vector<int> &args, vector<Process*> &processes){
    int q = args[2];
    cout << "\n\nSimulating Round Robin, quantum = " << q << "\n" << endl;

    //print table and first row, define length of columns
    string text1 = "Blocked (99 of 99)";
    string text2 = "Process 9";
    string text3 = "Cycle";
    string space1(count_digit(args[0]) + 1, ' ');
    string space2(text1.length()-text2.length(), ' ');
    print_table(args[0],space1,space2);
    int length1 = text3.length()+count_digit(args[0])+1;
    int length2 = text1.length();
    cout << 0 << string(length1-1, ' ');
    print_processes(processes, length2);

    int num_processes = args[0];
    int cycle = 1;
    vector<Process*> Ready(processes); //Ready Queue contains all processes
    Process* Running = nullptr;

    //loop until all processes have terminated
    while(num_processes > 0){

        if(!Running){
            if(Ready.size()>0){
                int state = Ready[0]->run();
                Running = Ready[0];
                Ready.erase(Ready.begin());;
            }
        }

        //one quantum
        for(int i=0; i != q; ++i){
            
            SimulateCycle(processes, Ready, Running, num_processes);
            cout << cycle << string(length1-count_digit(cycle), ' ');
            print_processes(processes,length2);
            cycle++;
        }
  
        //quantum over, block processes (if any running) and run new one (if available)
        cout << "--" << endl;
        if(Running){
            int status = Running->block();

            //block process
            if(status == 1){
                Ready.push_back(Running);
            } 
            //process terminated
            if (status == 5){
                num_processes--;
            }
            //find new process to run
            if(Ready.size()>0){ 
                int state = Ready[0]->run();
                Running = Ready[0];
                Ready.erase(Ready.begin());
            } else {
                Running = nullptr;
            }
        }
    }   
}

void SimulateCycle(vector<Process*> &processes, vector<Process*>&Ready, Process* &Running, int &num_processes){
    vector<Process*>::iterator iter;
    for(iter = processes.begin();iter!= processes.end();++iter){

        //call computeState for each process; depending on their state they will  
        //decrease their timer, change state, and return their state to the scheduler
        int state = (*iter)->computeState();

        //process changed to ready
        if(state==1){
            Ready.push_back(*iter);
            if(!Running){ //no other processes running
                (*iter)->run();
                Running = *iter;
                Ready.erase(Ready.begin());
            }
        //process just blocked
        } else if(state == 3){
            if(Ready.size()>0){ //run another process
                int state = Ready[0]->run();
                Running = Ready[0];
                Ready.erase(Ready.begin());
            } else {
                Running = nullptr;
            }
         //process just terminated
        } else if(state == 5){
            num_processes--;
            if(Ready.size()>0){ //run another process
                int state = Ready[0]->run();
                Running = Ready[0];
                Ready.erase(Ready.begin());;
            } else {
                Running = nullptr;
            }
        }
    }
}

void print_processes(vector<Process*>& processes, int length){
    vector<Process*>::iterator iter;
    for(iter = processes.begin();iter!=processes.end();++iter){
        (*iter)->printState(length);
    }
    cout << endl;
}

void print_table(int n, string &space1, string &space2){
    cout << "Cycle" << space1;
    for(int i = 0; i!=n; ++i){
        cout << "Process " << i + 1 << space2;
    }
    cout << endl;
}

void print(vector<int> &args){
    vector<int>::iterator iter;
    for(iter = args.begin();iter!=args.end();++iter){
        cout << *iter << " ";
    }
}

int count_digit(int number) {
   return int(log10(number) + 1);
}

const string WHITESPACE = " \n\r\t\f\v";
 
string ltrim(const string &s){
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == string::npos) ? "" : s.substr(start);
}
 
string rtrim(const string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == string::npos) ? "" : s.substr(0, end + 1);
}
 
string trim(const string &s) {
    return rtrim(ltrim(s));
}

vector<int> split_line(string input){
    vector<int> args;
    string space_delimiter = " ";
    size_t pos = 0;
    while ((pos = input.find(space_delimiter)) != string::npos) {
        args.push_back(stoi(input.substr(0, pos)));
        input.erase(0, pos + space_delimiter.length());
    }
    args.push_back(stoi(input));
    return args;
}