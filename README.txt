
This program simulates a scheduling algorithm. It takes an input specifying
number of processes, scheduling algorithm (FIFO and RR are available) 
and outputs a simulation of which process is running at which time. 

How to run:
Compile and run main.cpp.
process.cpp contains the Process class which keeps track of states and run/block time
and handles operations on processes.

A couple of assumptions have been made for simplicity:
1) All processes are created at time 1.
2) Each process runs for time R1, then blocks for time B1, 
   then runs for time R2, then blocks for time B2, then terminates.

Input format: n s q R1 B1 R2 B2 R1 B1 R2 B2... [read below]

Input must be given as a string of numbers, each separated by a space.
1st number: number of processes
2nd number: scheduling algorithm (1 for FIFO (First In First Out), 2 for RR (Round Robin))
(if RR) 3rd number: quantum size
Afterwards specify the run and block times of each process, 4 times total for each process.

I advise to keep number of processes reasonably small (<7) and times < 99 for output format purposes.

Example Input 1: 2 1 3 2 3 4 1 4 1
(2 processes, FIFO, p1 times: 3 2 3 2, p2 times: 4 1 4 1)

Output: 

Cycle  Process 1         Process 2         
0      Ready             Ready             
1      Running (1 of 3)  Ready             
2      Running (2 of 3)  Ready             
3      Running (3 of 3)  Ready             
4      Blocked (1 of 2)  Running (1 of 4)  
5      Blocked (2 of 2)  Running (2 of 4)  
6      Ready             Running (3 of 4)  
7      Ready             Running (4 of 4)  
8      Running (1 of 3)  Blocked (1 of 1)  
9      Running (2 of 3)  Ready             
10     Running (3 of 3)  Ready             
11     Blocked (1 of 2)  Running (1 of 4)  
12     Blocked (2 of 2)  Running (2 of 4)  
13     Ready             Running (3 of 4)  
14     Ready             Running (4 of 4)  
15     Terminating       Blocked (1 of 1)  
16                       Terminating 

Example Input 2: 3 2 2 3 2 3 2 4 1 4 1 3 3 3 3
(3 processes, RR, p1 times: 3 2 3 2, p2 times: 4 1 4 1, p3 times: 3 3 3 3)

Output: 

Cycle  Process 1         Process 2         Process 3         
0      Ready             Ready             Ready             
1      Running (1 of 3)  Ready             Ready             
2      Running (2 of 3)  Ready             Ready             
--
3      Ready             Running (1 of 4)  Ready             
4      Ready             Running (2 of 4)  Ready             
--
5      Ready             Ready             Running (1 of 3)  
6      Ready             Ready             Running (2 of 3)  
--
7      Running (3 of 3)  Ready             Ready             
8      Blocked (1 of 2)  Running (3 of 4)  Ready             
--
9      Blocked (2 of 2)  Ready             Running (3 of 3)  
10     Ready             Running (3 of 4)  Blocked (1 of 3)  
--
11     Running (1 of 3)  Ready             Blocked (2 of 3)  
12     Running (2 of 3)  Ready             Blocked (3 of 3)  
--
13     Ready             Running (4 of 4)  Ready             
14     Running (2 of 3)  Blocked (1 of 1)  Ready             
--
15     Ready             Ready             Running (1 of 3)  
16     Ready             Ready             Running (2 of 3)  
--
17     Running (3 of 3)  Ready             Ready             
18     Blocked (1 of 2)  Running (1 of 4)  Ready             
--
19     Blocked (2 of 2)  Ready             Running (3 of 3)  
20     Ready             Running (1 of 4)  Blocked (1 of 3)  
--
21     Terminating       Ready             Blocked (2 of 3)  
22                       Running (2 of 4)  Blocked (3 of 3)  
--
23                       Running (3 of 4)  Ready             
24                       Running (4 of 4)  Ready             
--
25                       Blocked (1 of 1)  Terminating       
26                       Terminating    

