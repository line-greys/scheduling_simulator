#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>
#include <cmath>

using namespace std;

class Process {
    protected:
        int state;
        vector<int> times;
        int time;
        int burst;
        int prnumber;
    public:
        Process(int aNumber, int aR1, int aB1, int aR2, int aB2){
            state = 1; //1: Ready, 2: Running, 3: Blocked, 4: Terminating, 5: Terminated
            burst = 1; //1: R1, 2: B1, 3: R2, 4: B2
            prnumber = aNumber;
            times.push_back(aR1);
            times.push_back(aB1);
            times.push_back(aR2);
            times.push_back(aB2);
            times.push_back(1);
            time = times[burst-1]+1;
        }
        int getState(){return state;}
        int getTime(){return time;}
        int getNumber(){return prnumber;}
        vector<int> getValues(){return times;}
        //void decreaseTime(){time--;}

        int run(){

            if(burst==5){
                state = 4; //change state to terminating
                return 4; 
            } else {
                state = 2; //change state to running
                return 2; 
            }
        }

        int block(){

            if(time==1){
                if (state==4){
                    state = 5; //terminated
                    return 5;
                }
                ++burst;
                time = times[burst-1]+1;
                state = 3; //move to blocked 
                return 3;
            } else {
                state = 1; //move to ready
                return 1;
            }
        }

        int computeState(){
            
            if(state == 1 || state == 5){
                return 0;
            } else {
                time--;
                if(time==0){
                    if(state==2){
                        ++burst;
                        time = times[burst-1]+1;
                        state = 3; //move to blocked
                        return 3;
                    } else if(state==3) {
                        ++burst;
                        time = times[burst-1]+1;
                        state = 1; //move to ready
                        return 1;                      
                    } else if(state==4) {
                        state = 5; //terminated
                        return 5;
                    }
                } 
                return 2;
            }
        }
    
        void printState(int length){
            switch(state){
                case 1:
                {
                    string text = "Ready";
                    cout << "Ready" << string(length-text.length(), ' ');
                    break;
                }
                case 2:
                {   if(time == times[burst-1]+1){time--;}; //process moved to running after being checked, decrease time
                    string text = "Running (" + to_string(times[burst-1]-time+1) + " of " + to_string(times[burst-1]) + ")";
                    cout << text << string(length - text.length(), ' '); 
                    break;
                }
                case 3:
                {
                    if(time == times[burst-1]+1){time--;}
                    string text = "Blocked (" + to_string(times[burst-1]-time+1) + " of " + to_string(times[burst-1]) + ")";
                    cout << text << string(length - text.length(), ' '); 
                    break;
                }
                case 4:
                {
                    if(time == times[burst-1]+1){time--;};
                    string text = "Terminating";
                    cout << text << string(length-text.length(), ' ');
                    break;
                }
                default:
                    cout << string(length, ' ');
                    

            }

        }
    
};