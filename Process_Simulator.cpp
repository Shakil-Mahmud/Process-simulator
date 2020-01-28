#include<bits/stdc++.h>
#define SIZE 100
#define totalResources 3
using namespace std;

int sysResourceA;
int sysResourceB;
int sysResourceC;
int sysMemorySize;
int framesize = 8;
int availableFrame;

class Process
{
private:
    int processID;
    string processName;
    string processStatus;
    int processSize;
    int hasIOoperation; // 1 means yes, 0 means no
    int burst;
    int arrivalTime;
    int priority;
    static int processCounter;
public:
    Process() {}

    void setProcess(string processName, int processSize, int hasIOoperation, int burst, int arrivalTime, int priority)
    {
        this->processID = ++(this->processCounter);
        this->processName = processName;
        this->processStatus = "new";
        this->processSize = processSize;
        this->hasIOoperation = hasIOoperation;
        this->burst = burst;
        this->arrivalTime = arrivalTime;
        this->priority = priority;
    }
    void setProcessID(int processID)
    {
        this->processID = processID;
    }
    void setProcessName(string processName)
    {
        this->processName = processName;
    }
    void setProcessStatus(string processStatus)
    {
        this->processStatus = processStatus;
    }
    void setProcessSize(int processSize)
    {
        this->processSize = processSize;
    }
    void setHasIOoperation(int hasIOoperation)
    {
        this->hasIOoperation = hasIOoperation;
    }
    void setBurst(int burst)
    {
        this->burst = burst;
    }
    void setArrivalTime(int arrivalTime)
    {
        this->arrivalTime = arrivalTime;
    }
    void setPriority(int priority)
    {
        this->priority = priority;
    }
    int getProcessID()
    {
        return this->processID;
    }
    string getProcessName()
    {
        return this->processName;
    }
    string getProcessStatus()
    {
        return this->processStatus;
    }
    int getProcessSize()
    {
        return this->processSize;
    }
    int getHasIOoperation()
    {
        return this->hasIOoperation;
    }
    int getBurst()
    {
        return this->burst;
    }
    int getArrivalTime()
    {
        return this->arrivalTime;
    }
    int getPriority()
    {
        return this->priority;
    }
};

int Process::processCounter = 0;

vector<Process>JOB_QUEUE;
vector<Process>READY_QUEUE;
vector<Process>DEVICE_QUEUE;

vector<int>allocated[SIZE];
vector<int>Max[SIZE];
vector<int>need[SIZE];
vector<int>available;

Process obj;
int GANTT_CHART[1000];
vector<int>Memory_Map;


void initGanttChart()
{
    memset(GANTT_CHART,-1,sizeof(GANTT_CHART));
}

int compare( Process a, Process b)
{
    return a.getArrivalTime()<b.getArrivalTime();
}


void createProcess()
{
    int pSize, pOp, pBurst, pArrivalTime, pPriority;
    string pName;
    cout << "Enter Process Name: ";
    cin >> pName;
    cout << "Enter Process Size: ";
    cin >> pSize;
    cout << "Has IO operation? (1 for yes, 0 for no): ";
    cin >> pOp;
    cout << "Enter Burst: ";
    cin >> pBurst;
    cout << "Enter Arrival Time: ";
    cin >> pArrivalTime;
    cout << "Enter Priority: ";
    cin >> pPriority;
    obj.setProcess(pName, pSize, pOp, pBurst, pArrivalTime, pPriority);

    JOB_QUEUE.push_back(obj);
}

void showJobQueue()
{
    cout.setf(ios::left, ios::adjustfield);
    cout << "Job Queue" << endl;
    cout << "---------" << endl;
    cout << setw(10) << "ID" << setw(10) << "Name" << setw(10) << "Burst" << setw(10) << "Arrival"
         << setw(10) << "Priority" << setw(10) << "Size" << setw(10) << "hasIO?" <<endl;
    for(int i = 0; i < JOB_QUEUE.size(); i++)
    {
        cout << setw(10) << JOB_QUEUE[i].getProcessID() << setw(10) << JOB_QUEUE[i].getProcessName()
             << setw(10) << JOB_QUEUE[i].getBurst() << setw(10) << JOB_QUEUE[i].getArrivalTime()
             << setw(10) << JOB_QUEUE[i].getPriority()
             << setw(10) << JOB_QUEUE[i].getProcessSize()
             << setw(10) << JOB_QUEUE[i].getHasIOoperation() << endl;
    }

}

void showReadyQueue()
{
    cout.setf(ios::left, ios::adjustfield);
    cout << "Ready Queue" << endl;
    cout << "---------" << endl;
    cout << setw(10) << "ID" << setw(10) << "Name" << setw(10) << "Burst" << setw(10) << "Arrival" << setw(10) << "Priority"
         << setw(10) << "Size" << setw(10) << "hasIO?" << endl;
    for(int i = 0; i < READY_QUEUE.size() ; i++)
    {
        cout << setw(10) << READY_QUEUE[i].getProcessID() << setw(10) << READY_QUEUE[i].getProcessName()
             << setw(10) << READY_QUEUE[i].getBurst() << setw(10) << READY_QUEUE[i].getArrivalTime()
             << setw(10) << READY_QUEUE[i].getPriority()
             << setw(10) << READY_QUEUE[i].getProcessSize()
             << setw(10) << READY_QUEUE[i].getHasIOoperation() << endl;
    }

}

void showDeviceQueue()
{
    cout.setf(ios::left, ios::adjustfield);
    cout << "Device Queue" << endl;
    cout << "---------" << endl;
    cout << setw(10) << "ID" << setw(10) << "Name" << setw(10) << "Burst" << setw(10) << "Arrival" << setw(10) << "Priority"
         << setw(10) << "Size" << setw(10) << "hasIO?" << endl;
    for(int i = 0; i < DEVICE_QUEUE.size() ; i++)
    {
        cout << setw(10) << DEVICE_QUEUE[i].getProcessID() << setw(10) << DEVICE_QUEUE[i].getProcessName()
             << setw(10) << DEVICE_QUEUE[i].getBurst() << setw(10) << DEVICE_QUEUE[i].getArrivalTime()
             << setw(10) << DEVICE_QUEUE[i].getPriority()
             << setw(10) << DEVICE_QUEUE[i].getProcessSize()
             << setw(10) << DEVICE_QUEUE[i].getHasIOoperation() << endl;
    }

}


void removeProcess(bool* finish){

    for(int i=0 ; i<DEVICE_QUEUE.size() ; i++){
        if(!finish[i]){
            available[0]+=allocated[i][0];
            available[1]+=allocated[i][1];
            available[2]+=allocated[i][2];
            allocated[i][0] = 0;
            allocated[i][1] = 0;
            allocated[i][2] = 0;
            cout<<"process "<<DEVICE_QUEUE[i].getProcessName()<<" has preempted"<<endl;
            break;
        }
    }
}

bool is_safe(bool* finish, vector<int>&work,queue<int>&safeseq){

    int count = 0, totalProcess = DEVICE_QUEUE.size();
    while (count < totalProcess)
    {
        bool found = false;
        for (int i = 0; i<totalProcess ; i++)
        {
            if (finish[i] == false)
            {
                int j;
                for (j = 0; j<totalResources ; j++)
                    if (need[i][j] > work[j])
                        break;

                if (j == totalResources)
                {
                    for (int k = 0 ; k < totalResources ; k++)
                        work[k] += allocated[i][k];

                    count++;
                    safeseq.push(i);
                    finish[i] = true;

                    found = true;
                }
             }
        }
        if (found == false)
        {
            return false;
        }
    }

    return true;
}

void calculateneed()
{
    int totalProcess = DEVICE_QUEUE.size();
    for(int i=0 ; i<totalProcess ; i++)
    {
        need[i].push_back(Max[i][0] - allocated[i][0]);
        need[i].push_back(Max[i][1] - allocated[i][1]);
        need[i].push_back(Max[i][2] - allocated[i][2]);
    }
}

void input()
{

    int totalProcess = DEVICE_QUEUE.size();
    cout<<"total process "<<totalProcess<<endl;
    int a,b,c;
    for(int i=0 ; i<totalProcess ; i++)
    {
        cout<<"Enter max instances of resources(A,B,C) can be allocated by process "
            <<DEVICE_QUEUE[i].getProcessName()<<" : ";
        cin>>a>>b>>c;
        Max[i].push_back(a);
        Max[i].push_back(b);
        Max[i].push_back(c);

    }
    int A=0, B=0, C=0;
    for(int i=0 ; i<totalProcess ; i++)
    {
        cout<<"Enter instances of resources(A,B,C) allocated by process "
            <<DEVICE_QUEUE[i].getProcessName()<<" : ";
        cin>>a>>b>>c;
        allocated[i].push_back(a);
        allocated[i].push_back(b);
        allocated[i].push_back(c);
        A+=a;
        B+=b;
        C+=c;
    }

    available.push_back(sysResourceA-A);
    available.push_back(sysResourceB-B);
    available.push_back(sysResourceC-C);

    cout<<"available: ";
    for(int i=0 ; i<available.size() ; i++)
        cout<<available[i]<< ' ';

    cout<<endl;
}

void bankers_algorithm(bool takeInput)
{
     cout << "Enter into bankers algorithm" << endl;
    if(takeInput){
        showDeviceQueue();
        input();
    }

    calculateneed();

    int totalProcess = DEVICE_QUEUE.size();
    bool finish[totalProcess+2];
    memset(finish,0,sizeof(finish));

    queue<int>safeseq;
    vector<int>work;

    work = available;

    bool safe = is_safe(finish,work,safeseq);

    if(safe){
        cout << "System is in safe state.\nSafe sequence is: ";
        while(!safeseq.empty())
        {
            int I = safeseq.front();
            cout<<DEVICE_QUEUE[I].getProcessName()<<' ';
            safeseq.pop();
        }
        cout<<endl;
    }
    else{
        cout << "System is not in safe state"<<endl;
        for(int t=0 ; t<totalProcess ; t++)
            if(!finish[t])
                cout<<"process: "<<DEVICE_QUEUE[t].getProcessName()<<" is in deadlock"<<endl;
        removeProcess(finish);
        bankers_algorithm(0);
    }
}

void MemoryMapOUT (int id)
{

    for(int i=0 ; i<Memory_Map.size() ; i++)
        if(Memory_Map[i]==id)
            Memory_Map[i] = -1;

}

void MemoryMapIN( int id, int total_frames)
{
     for(int i=0 ; i<Memory_Map.size() ; i++)
    {
        if(total_frames==0)
            break;

        if(Memory_Map[i]==-1)
        {
             Memory_Map[i] = id;
            total_frames--;
        }
    }
}


bool checkForFreeMemorySpace( int FrameRequire)
{

    if( FrameRequire <= availableFrame )
        return true;

    return false;
}

void readyProcess()
{
    cout << "Transferring Processes from Job Queue to Ready/Device Queue." << endl;
    int sz = JOB_QUEUE.size();
    sort(JOB_QUEUE.begin(),JOB_QUEUE.end(),compare);

    for(int i = 0; i<JOB_QUEUE.size(); i++)
    {
        Process P = JOB_QUEUE[i];

        if(P.getHasIOoperation() == 1)
        {
            DEVICE_QUEUE.push_back(P);
            JOB_QUEUE.erase(JOB_QUEUE.begin()+i);
            i--;
            cout<<"Process "<<P.getProcessName()<<" : transfered to Device Queue"<<endl;
        }

        else if( checkForFreeMemorySpace( ceil(P.getProcessSize()/(framesize*1.0)) ) )
        {
            READY_QUEUE.push_back(P);
            cout<<"Process "<<P.getProcessName()<<" : transfered to Ready Queue"<<endl;
            JOB_QUEUE.erase(JOB_QUEUE.begin()+i);
            i--;
            availableFrame -= ceil(P.getProcessSize()/(framesize*1.0));
            MemoryMapIN( P.getProcessID(), ceil(P.getProcessSize()/(framesize*1.0)) );
        }

        else
        {
            cout<<"Process "<<P.getProcessName()<<" : Memory Limit Exceeded"<<endl;
        }
    }


    cout << "Transfer Completed." << endl;
    if(DEVICE_QUEUE.size() > 0)
    {
        bankers_algorithm(1);
    }
}

void displayGanttChart(int clock)
{
    cout.setf(ios::left, ios::adjustfield);
    for(int i=0; i<clock; i++)
    {
        if(i%10==0 && i/10>=1)
            cout << endl;
        cout << setw(3) << GANTT_CHART[i];
    }
    cout << endl;
}

double average(int *list)
{
    int sum = 0;
    for(int i = 0; i<READY_QUEUE.size(); i++)
    {
        sum += list[i];
    }
    return sum*1.0/READY_QUEUE.size();
}

void fcfs()
{
    initGanttChart();
    sort(READY_QUEUE.begin(), READY_QUEUE.end(), compare);
    showReadyQueue();
    initGanttChart();

    int clock = 0;
    int i = 0;

    bool seen[READY_QUEUE.size()];
    int remaining_time[READY_QUEUE.size()];

    for(int i=0; i<READY_QUEUE.size(); i++)
    {
        seen[i] = false;
        remaining_time[i] = READY_QUEUE[i].getBurst();
    }

    int complete = 0;
    int waiting_time [READY_QUEUE.size()], response_time[READY_QUEUE.size()], turnaround_time [READY_QUEUE.size()];

    while(complete<READY_QUEUE.size())
    {
        if(READY_QUEUE[i].getArrivalTime()<=clock && READY_QUEUE[i].getProcessStatus()!="Terminated")
        {
            for(int j = clock; j < clock + remaining_time[i]; j++)
            {
                GANTT_CHART[j] = READY_QUEUE[i].getProcessID();
            }

            if(seen[i]==false)
            {
                response_time[i] = clock - READY_QUEUE[i].getArrivalTime();
                seen[i]=true;
            }
            clock = clock + READY_QUEUE[i].getBurst();
            READY_QUEUE[i].setProcessStatus("Terminated");
            waiting_time[i] = clock - READY_QUEUE[i].getArrivalTime() - READY_QUEUE[i].getBurst();
            turnaround_time[i] = clock - READY_QUEUE[i].getArrivalTime();
            i++;
            complete++;
        }
        else
        {
            clock++;
        }
    }
    cout << "FCFS Scheduling" << endl;
    displayGanttChart(clock);
    cout << setw(30) << "Average Waiting Time: " << average(waiting_time) << endl;
    cout << setw(30) << "Average Response Time: " << average(response_time) << endl;
    cout << setw(30) << "Average Turnaround TIme: " << average(turnaround_time) << endl;
}

void sjf()
{
    initGanttChart();
    int clock = 0;
    int i = 0;
    bool seen[READY_QUEUE.size()];
    int remaining_time[READY_QUEUE.size()];
    for(int i=0; i<READY_QUEUE.size(); i++)
    {
        seen[i] = false;
        remaining_time[i] = READY_QUEUE[i].getBurst();
    }
    int complete = 0;
    int waiting_time [READY_QUEUE.size()], response_time[READY_QUEUE.size()], turnaround_time [READY_QUEUE.size()];

    while(complete<READY_QUEUE.size())
    {

        int minm = INT_MAX;
        int shortest = -1;
        //bool found = false;
        for(int i=0; i<READY_QUEUE.size(); i++)
        {
            if(READY_QUEUE[i].getBurst() < minm &&
                    READY_QUEUE[i].getArrivalTime() <= clock
                    && READY_QUEUE[i].getProcessStatus()!="Terminated")
            {
                minm = READY_QUEUE[i].getBurst();
                shortest = i;
            }
        }

        if(shortest==-1)
        {
            clock++;
            continue;
        }

        for(int j = clock; j < clock + remaining_time[shortest]; j++){
            GANTT_CHART[j] = READY_QUEUE[shortest].getProcessID();
        }
        if(seen[shortest]==false)
        {
            response_time[shortest] = clock - READY_QUEUE[shortest].getArrivalTime();
            seen[shortest]=true;
        }
        clock = clock + READY_QUEUE[shortest].getBurst();
        READY_QUEUE[shortest].setProcessStatus("Terminated");
        waiting_time[shortest] = clock -
                                 READY_QUEUE[shortest].getArrivalTime() -
                                 READY_QUEUE[shortest].getBurst();
        turnaround_time[shortest] = clock -
                                    READY_QUEUE[shortest].getArrivalTime();
        complete++;
    }
    cout << "SJF Scheduling" << endl;
    displayGanttChart(clock);
    cout << setw(30) << "Average Waiting Time: " << average(waiting_time) << endl;
    cout << setw(30) << "Average Response Time: " << average(response_time) << endl;
    cout << setw(30) << "Average Turnaround TIme: " << average(turnaround_time) << endl;
}

void srtf()
{
    initGanttChart();
   // sort(READY_QUEUE.begin(),READY_QUEUE.end(),compare);
    int clock = 0, complete = 0;

    bool seen[READY_QUEUE.size()+2];
    int remaining_time[READY_QUEUE.size()+2], waiting_time [READY_QUEUE.size()+2],
     response_time[READY_QUEUE.size()+2], turnaround_time [READY_QUEUE.size()+2];

    memset(seen,0,sizeof(seen));
    for(int i=0; i<READY_QUEUE.size(); i++)
    {
        remaining_time[i] = READY_QUEUE[i].getBurst();
    }

    while(complete<READY_QUEUE.size())
    {

        int minm = INT_MAX;
        int shortest = -1;
        for(int i=0; i<READY_QUEUE.size(); i++)
        {
            if( remaining_time[i] < minm && remaining_time[i] > 0 && READY_QUEUE[i].getArrivalTime() <= clock
                    && READY_QUEUE[i].getProcessStatus()!="Terminated")
            {
                minm = remaining_time[i];
                shortest = i;
            }
        }

        if(shortest==-1)
        {
            clock++;
            continue;
        }

        if(seen[shortest]==false)
        {
            response_time[shortest] = clock - READY_QUEUE[shortest].getArrivalTime();
            seen[shortest]=true;
        }

        GANTT_CHART[clock] = READY_QUEUE[shortest].getProcessID();
        remaining_time[shortest]--;

        if(remaining_time[shortest]<=0)
        {
            READY_QUEUE[shortest].setProcessStatus("Terminated");
            waiting_time[shortest] = clock - READY_QUEUE[shortest].getArrivalTime() - READY_QUEUE[shortest].getBurst() +1;
            turnaround_time[shortest] = READY_QUEUE[shortest].getBurst()+waiting_time[shortest];
            complete++;
        }

        clock++;
    }

    cout << " Shortest Job First Scheduling" << endl;
    displayGanttChart(clock);
    cout << setw(30) << "Average Waiting Time: " << average(waiting_time) << endl;
    cout << setw(30) << "Average Response Time: " << average(response_time) << endl;
    cout << setw(30) << "Average Turnaround TIme: " << average(turnaround_time) << endl;

}

void priorityScheduling()
{
    initGanttChart();
    int clock = 0;
    int i = 0;
    bool seen[READY_QUEUE.size()];
    int remaining_time[READY_QUEUE.size()];
    for(int i=0; i<READY_QUEUE.size(); i++)
    {
        seen[i] = false;
        remaining_time[i] = READY_QUEUE[i].getBurst();
    }
    int complete = 0;
    int waiting_time [READY_QUEUE.size()], response_time[READY_QUEUE.size()], turnaround_time [READY_QUEUE.size()];

    while(complete<READY_QUEUE.size())
    {
        // find next process to execute
        int minm = INT_MAX;
        int highest = -1;
        //bool found = false;
        for(int i=0; i<READY_QUEUE.size(); i++)
        {
            if(READY_QUEUE[i].getPriority() < minm &&
                    READY_QUEUE[i].getArrivalTime() <= clock
                    && READY_QUEUE[i].getProcessStatus()!="Terminated")
            {
                minm = READY_QUEUE[i].getPriority();
                highest = i;
            }
        }

        if(highest==-1)
        {
            clock++;
            continue;
        }

        for(int j = clock; j < clock + remaining_time[highest]; j++){
            GANTT_CHART[j] = READY_QUEUE[highest].getProcessID();
        }

        if(seen[highest]==false)
        {
            response_time[highest] = clock - READY_QUEUE[highest].getArrivalTime();
            seen[highest]=true;
        }
        clock = clock + READY_QUEUE[highest].getBurst();
        READY_QUEUE[highest].setProcessStatus("Terminated");
        waiting_time[highest] = clock -READY_QUEUE[highest].getArrivalTime() -
                                READY_QUEUE[highest].getBurst();
        turnaround_time[highest] = clock -
                                   READY_QUEUE[highest].getArrivalTime();
        complete++;
    }
    cout << "SJF Scheduling" << endl;
    displayGanttChart(clock);
    cout << setw(30) << "Average Waiting Time: " << average(waiting_time) << endl;
    cout << setw(30) << "Average Response Time: " << average(response_time) << endl;
    cout << setw(30) << "Average Turnaround TIme: " << average(turnaround_time) << endl;

}

void preemptivePriorityScheduling()
{
    initGanttChart();
    int clock = 0, complete = 0;

    bool seen[READY_QUEUE.size()];
    int remaining_time[READY_QUEUE.size()], waiting_time [READY_QUEUE.size()], response_time[READY_QUEUE.size()],
        turnaround_time [READY_QUEUE.size()];

    memset(seen,0,sizeof(seen));
    for(int i=0; i<READY_QUEUE.size(); i++)
    {
        remaining_time[i] = READY_QUEUE[i].getBurst();
    }

    while(complete<READY_QUEUE.size())
    {

        int minm = INT_MAX;
        int shortest = -1;
        for(int i=0; i<READY_QUEUE.size(); i++)
        {
            if(READY_QUEUE[i].getPriority() < minm && READY_QUEUE[i].getArrivalTime() <= clock
                    && READY_QUEUE[i].getProcessStatus()!="Terminated")
            {
                minm = READY_QUEUE[i].getPriority();
                shortest = i;
            }
        }

        if(shortest==-1)
        {
            clock++;
            continue;
        }

        if(seen[shortest]==false)
        {
            response_time[shortest] = clock - READY_QUEUE[shortest].getArrivalTime();
            seen[shortest]=true;
        }

        GANTT_CHART[clock] = READY_QUEUE[shortest].getProcessID();
        remaining_time[shortest]--;

        if(remaining_time[shortest]<=0)
        {
            READY_QUEUE[shortest].setProcessStatus("Terminated");
            waiting_time[shortest] = clock - READY_QUEUE[shortest].getArrivalTime() - READY_QUEUE[shortest].getBurst() +1;
            turnaround_time[shortest] = READY_QUEUE[shortest].getBurst()+waiting_time[shortest];
            complete++;
        }

        clock++;
    }

    cout << "preemptivePriorityScheduling" << endl;
    displayGanttChart(clock);
    cout << setw(30) << "Average Waiting Time: " << average(waiting_time) << endl;
    cout << setw(30) << "Average Response Time: " << average(response_time) << endl;
    cout << setw(30) << "Average Turnaround TIme: " << average(turnaround_time) << endl;

}

void roundRobin()
{
    sort(READY_QUEUE.begin(),READY_QUEUE.end(),compare);
    initGanttChart();
    cout<<"Enter time quantum"<<endl;
    int time_quantum;
    cin>>time_quantum;

    queue<Process>QUEUE;
    int indexing[1000][5];
    bool finished[ READY_QUEUE.size()+2 ];
    bool seen[READY_QUEUE.size()];
    int waiting_time [READY_QUEUE.size()+2], response_time[READY_QUEUE.size()+2],
        turnaround_time [READY_QUEUE.size()+2], remaining_time[READY_QUEUE.size()+2] ;

    for( int i=0 ; i<READY_QUEUE.size() ; i++)
    {
        remaining_time[i] = READY_QUEUE[i].getBurst();
        waiting_time[i] = 0;
        int x = READY_QUEUE[i].getProcessID();
        indexing[x][0] = i;
        finished[i] = false;
    }

    int clock = 0, complete=0, ready_queue_i=0, index=-1;

    cout<<"start round robin"<<endl;
    while( complete < READY_QUEUE.size() )
    {
        Process P;

        for( int i=0 ; i<time_quantum ; i++)
        {
            ///  push newly arrived processes in the QUEUE
            for(int j = ready_queue_i ; j<READY_QUEUE.size() ; j++)
            {
                if( READY_QUEUE[j].getArrivalTime()<=clock )
                {
                    QUEUE.push( READY_QUEUE[j] );
                    ready_queue_i++;
                }
            }

            if(!QUEUE.empty())
            {
                P = QUEUE.front();
                index = indexing[P.getProcessID()][0];
                if(!seen[index]){
                    response_time[index] = clock-READY_QUEUE[index].getArrivalTime()+1;
                    seen[index] = true;
                }
                remaining_time[index]--;
                GANTT_CHART[clock] = P.getProcessID();
                /// execution completed
                if(remaining_time[index]<=0)
                {
                    waiting_time[index] = clock - P.getBurst() - P.getArrivalTime()+1;
                    turnaround_time[index] = clock - P.getArrivalTime()+1;
                    QUEUE.pop();
                    i = -1;
                    complete++;
                    finished[index] = true;
                    READY_QUEUE[index].setProcessStatus("Terminated");
                }
                clock++;
            }
            else
            {
                clock++;
                if(complete < READY_QUEUE.size())
                    i=-1;
            }
        }
        if(!QUEUE.empty())
            QUEUE.pop();
        if(index!=-1 && remaining_time[index]>0)
            QUEUE.push(P);

    }
    cout<<setw(10)<<"wa time"<<setw(10)<<"tu time"<<endl;
    for(int m=0 ; m<READY_QUEUE.size() ; m++)
    {
        cout<<setw(10)<<waiting_time[m]<<setw(10)<<turnaround_time[m]<<endl;
    }
    cout << "Round Robin Scheduling" << endl;
    displayGanttChart(clock);
    cout << setw(30) << "Average Waiting Time: " << average(waiting_time) << endl;
    cout << setw(30) << "Average Response Time: " << average(response_time) << endl;
    cout << setw(30) << "Average Turnaround Time: " << average(turnaround_time) << endl;
}

void executeProcess()
{
    int choice;
    cout << "CPU Scheduling Algorithm Selection" << endl;
    cout << "-------------------" << endl;
    cout << "(1) First-Come-First-Served (FCFS)" << endl;
    cout << "(2) Shortest-Job-First (SJF)" << endl;
    cout << "(3) Preemptive SJF (shortest-remaining-time-first)" << endl;
    cout << "(4) Priority-based Scheduling" << endl;
    cout << "(5) Preemptive Priority-based Scheduling" << endl;
    cout << "(6) Round-Robin Scheduling" << endl;
    cout << "Enter Choice: ";
    cin >> choice;
    switch(choice)
    {
    case 1:
        fcfs();
        break;
    case 2:
        sjf();
        break;
    case 3:
        srtf();
        break;
    case 4:
        priorityScheduling();
        break;
    case 5:
        preemptivePriorityScheduling();
        break;
    case 6:
        roundRobin();
        break;
    default:
        cout << "Unrecognized Option. " << endl;
    }
}

void reexecuteProcess()
{
    for(int i=0; i<READY_QUEUE.size(); i++)
    {
        READY_QUEUE[i].setProcessStatus("ready");
    }
    executeProcess();
}

void sysConfig()
{
    cout << "Enter number of instances of A: ";
    cin >> sysResourceA;
    cout << "Enter number of instances of B: ";
    cin >> sysResourceB;
    cout << "Enter number of instances of C: ";
    cin >> sysResourceC;
    cout << "Enter memory size: ";
    cin >> sysMemorySize;
    cout<< "Enter frame size: ";
    cin>>framesize;

    availableFrame = (sysMemorySize/framesize);

    for(int i=0 ; i<availableFrame ; i++)
        Memory_Map.push_back(-1);
}

void deviceToready()
{

    showDeviceQueue();
    int select;
    bool flage = false;
    cout<<"Select a process (process id) : ";
    cin>>select;

    for(int i=0 ; i<DEVICE_QUEUE.size() ; i++)
    {
        Process P = DEVICE_QUEUE[i];

        if(P.getProcessID()==select)
        {
            ///check for empty space;
            int Frames = ceil(P.getProcessSize()/(framesize*1.0));
            if( checkForFreeMemorySpace( Frames ) )
            {
                READY_QUEUE.push_back(P);
                cout<<"Process "<<P.getProcessName()<<" : transfered to Ready Queue"<<endl;
                DEVICE_QUEUE.erase(DEVICE_QUEUE.begin()+i);
                i--;
                availableFrame -= Frames;
                MemoryMapIN( P.getProcessID(), Frames);
                flage = true;
                break;
            }
            else{
                cout<<"Memory limit exceeded"<<endl;
                flage = true;
            }

        }
    }
    if(!flage){
        cout<<"Process id not found. Try again"<<endl;
        deviceToready();
    }
}

void jobToready()
{

    showJobQueue();
    bool flage = false;
    int select;
    cout<<"Select a process (process id) : ";
    cin>>select;

    for(int i=0 ; i<JOB_QUEUE.size() ; i++)
    {
        Process P = JOB_QUEUE[i];

        if(P.getProcessID()==select)
        {
            ///check for empty space;
            int Frames = ceil(P.getProcessSize()/(framesize*1.0));
            if( checkForFreeMemorySpace( Frames ) )
            {
                READY_QUEUE.push_back(P);
                cout<<"Process "<<P.getProcessName()<<" : transfered to Ready Queue"<<endl;
                JOB_QUEUE.erase(JOB_QUEUE.begin()+i);
                i--;
                availableFrame -= Frames;
                MemoryMapIN( P.getProcessID(), Frames);
                flage = true;
            }
            else{
                cout<<"Memory limit exceeded"<<endl;
                flage = true;
            }

        }
    }
    if(!flage){
        cout<<"Process id not found. Try again"<<endl;
        jobToready();
    }
}

void readyTojob()
{

    showReadyQueue();

    int select;
    bool flage = false;
    cout<<"Select a process (process id) : ";
    cin>>select;

    for(int i=0 ; i<READY_QUEUE.size() ; i++)
    {
        Process P = READY_QUEUE[i];

        if(P.getProcessID()==select)
        {
            JOB_QUEUE.push_back(P);
            cout<<"Process "<<P.getProcessName()<<" : transfered to Job Queue"<<endl;
            READY_QUEUE.erase(READY_QUEUE.begin()+i);
            i--;
            availableFrame += ceil(P.getProcessSize()/(framesize*1.0));
            MemoryMapOUT( P.getProcessID() );
            flage = true;
            break;
        }
    }
    if(!flage){
         cout<<"Process id not found. Try again"<<endl;
        readyTojob();
    }
}

void jobTodevice()
{

    showJobQueue();
    bool flage = false;
    int select;
    cout<<"Select a process (process id) : ";
    cin>>select;

    for(int i=0 ; i<JOB_QUEUE.size() ; i++)
    {
        Process P = JOB_QUEUE[i];

        if(P.getProcessID()==select)
        {
            DEVICE_QUEUE.push_back(P);
            cout<<"Process "<<P.getProcessName()<<" : transfered to Device Queue"<<endl;
            JOB_QUEUE.erase(JOB_QUEUE.begin()+i);
            i--;
            flage = true;
            break;
        }
    }

    if(!flage){
        cout<<"Process id not found. Try again"<<endl;
        jobTodevice();
    }
    else{
        bankers_algorithm(1);
    }
}

void readyTodevice()
{
    showReadyQueue();
    bool flage = false;
    int select;
    cout<<"Select a process (process id) : ";
    cin>>select;

    for(int i=0 ; i<READY_QUEUE.size() ; i++)
    {
        Process P = READY_QUEUE[i];

        if(P.getProcessID()==select)
        {
            DEVICE_QUEUE.push_back(P);
            cout<<"Process "<<P.getProcessName()<<" : transfered to Device Queue"<<endl;
            READY_QUEUE.erase(READY_QUEUE.begin()+i);
            i--;
            availableFrame += ceil(P.getProcessSize()/(framesize*1.0));
            MemoryMapOUT( P.getProcessID() );
            flage = true;
            break;
        }
    }
    if(!flage){
         cout<<"Process id not found. Try again"<<endl;
        readyTodevice();
    }
    else{
        bankers_algorithm(1);
    }
}

void changeStatus()
{
    cout<<"1. waiting to ready (device queue to ready queue)."<<endl;
    cout<<"2. ready to waiting (ready queue to device queue)."<<endl;
    cout<<"3. send a new process to complete I/O operation (job queue to device queue)."<<endl;
    cout<<"4. make a process ready to execute (job queue to ready queue)."<<endl ;
    cout<<"5. terminate a process (ready queue to job queue)"<<endl;
    cout<<endl<<"Enter choice : ";

    int choice;
    cin>>choice;

    switch(choice)
    {
    case 1:
        deviceToready();
        break;
    case 2:
        readyTodevice();
        break;
    case 3:
        jobTodevice();
        break;
    case 4:
        jobToready();
        break;
    case 5:
        readyTojob();
        break;
    default :
        cout<<"Unrecognized Command."<<endl;
        break;
    }
}

void showMemoryMap()
{
    cout.setf(ios::left, ios::adjustfield);

    for(int i=0 ; i<Memory_Map.size() ; i++)
        cout<<setw(10)<<"Frame no: "<<setw(10)<<i<<setw(11)<<" Process id: "<<Memory_Map[i]<<endl;
        //cout<<setw(12)<<i<<setw(11)<<Memory_Map[i]<<endl;
}


void Clear(){
    system("CLS");
}

int main()
{
    string command;
    sysConfig();
    do
    {
        cout << "> ";
        cin >> command;
        if(command=="create-process")
        {
            createProcess();
        }
        else if(command=="ready-process")
        {
            readyProcess();
        }
        else if(command=="show-job-queue")
        {
            showJobQueue();
        }
        else if(command=="show-ready-queue")
        {
            showReadyQueue();
        }
        else if(command=="show-device-queue")
        {
            showDeviceQueue();
        }
        else if(command=="execute-process")
        {
            executeProcess();
        }
        else if(command=="reexecute-process")
        {
            reexecuteProcess();
        }
        else if(command=="change-status")
        {
            changeStatus();
        }
        else if(command=="show-memory-map")
        {
            showMemoryMap();
        }
        else if(command=="clear")
        {
            Clear();
        }
        else if(command=="exit")
        {
            cout << "Thank You." << endl;
            exit(0);
        }
        else
        {
            cout << "Unrecognized Command. Try again." << endl;
        }
    }
    while(true);
}
