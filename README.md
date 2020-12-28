# Process Simulator
## List of Commands
The following commands are working in my project.
| Command | Remarks |
| create-process | Create a new process. |
| ready-process	| Transfer processes from job queue to ready queue and device queue. Also call the function bankers_algorithm() to check system is safe or not if there is any process in device queue. | 
| show-job-queue	| Show all process in job queue. |
| show-ready-queue	| Show all process in ready queue. |
| show-device-queue	| Show all process in device queue. |
| execute-process	| Execute all process in ready queue, before that users have to select a scheduling algorithm to start execution. |
|reexecute-process	| Re-execute all process in ready queue including all terminated process. |
| change-status	| Switch state of a process to another state. There are five option to switch state of a process, user has to select an option, then also need to select a particular process. Enough frames must be available in memory to switch state of a process to ‘ready’ state. If state of a process switch to ‘waiting’ state   bankers_algorithm() function will be called to check system is safe or not. |
| show-memory-map	| Display all frames in memory. |
| clear	| Clear the screen. |
| exit | Terminate the process simulator |

NB: As many functions depending on the data stored by the sysConfig(), it will be called automatically at the beginning of the program.  
## Data Structure used
I have used the data structure, Vector and also Queue in some cases, as given in the sample project.
## CPU Scheduling Algorithms
The following CPU scheduling algorithms are implemented.
..*	FCFS
..*	SJF
..*	SRTF
..*	Priority-based Scheduling 
..*	Preemptive Priority-based Scheduling
..*	Round Robin

## Banker’s Algorithm for Deadlock Avoidance
This algorithm is implemented in my project. After executing this algorithm –
..*	If the system is safe, safe sequence will be printed.
..*	If the system is unsafe, one process will be preempted and banker’s algorithm will be called, if the system is still unsafe, another process will be preempted and again the banker’s algorithm will be called. This process will continue until the system is in the safe state.

## Memory Management
..*	Process will be sent to ready queue according to available frame number in memory.
..*	Users can view which frame is allocated to which process and which frames are empty.
