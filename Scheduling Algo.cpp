#include<bits/stdc++.h>
#include<fstream>
using namespace std;
ifstream inputFile("input.txt");
ofstream outputFile("output.txt"); 
class process{
    public:
    int pid;
    int at;
    int bt;
    int ct;
    int tat;
    int wt;
    int priority=1;
};

bool sorttheprocesses(const process &p1,const process &p2)
{
    if(p1.at!=p2.at)
    {
        return p1.at<p2.at;
    }
    if(p1.priority!=p2.priority)
    {
        return p1.priority>p2.priority;
    }
    return p1.pid<p2.pid;
}

bool sortbypid(const process &p1,const process &p2)
{
    return p1.pid<p2.pid;
}

void printans(vector<process>&v,vector<int>&z)
{
    double totalwt=0,totaltat=0;
    double k=v.size();
    outputFile<<endl;
    sort(v.begin(),v.end(),sortbypid);
    outputFile<<"Gantt chart:-|";
    int sz=z.size();
    for(int i=sz-1;i>=0;i--)
    {
        if(i+1<z.size() && z[i]==z[i+1])
        {
            z.erase(z.begin()+i+1);
        }
    }
    for(int i=0;i<z.size();i++)
    {
        outputFile<<"P"<<z[i]<<"|";
    }
    outputFile<<endl<<endl;
    outputFile<<"|    "<<"Pid"<<"   |    "<<"AT"<<"    |     "<<"BT"<<"      |       "<<"CT"<<"      |    "<<"TAT"<<"     |       "<<"WT"<<"      |"<<endl;
    for(int i=0;i<v.size();i++)
    {
        totalwt+=v[i].wt;
        totaltat+=v[i].tat;
        outputFile<<"|    "<<v[i].pid<<"     |    "<<v[i].at<<"     |     "<<v[i].bt<<"       |       "<<v[i].ct<<"       |     "<<v[i].tat<<"      |       "<<v[i].wt<<"       |"<<endl;
    }
    outputFile<<endl;
    outputFile<<"Average Turnaround time: "<<(totaltat/k)<<endl;
    outputFile<<"Average Waiting time: "<<(totalwt/k)<<endl;
    outputFile<<endl;
}

void tatandwt(vector<process>&v)
{
    for(int i=0;i<v.size();i++)
    {
        v[i].tat=v[i].ct-v[i].at;
        v[i].wt=v[i].tat-v[i].bt;
    }
}

void fcfs(int &n,int& ct,int &currtime,vector<process>&v,vector<int>&z)
{
    for(int i=0;i<n;i++)
    {
        currtime=max(currtime,v[i].at);
        currtime+=v[i].bt;
        v[i].ct=currtime;
        ++ct;
        z.push_back(v[i].pid);
    }
}

void sjf(int &n,int& ct,int &currtime,vector<process>&v,vector<int>&z)
{
    priority_queue<vector<int>,vector<vector<int>>,greater<vector<int>>>pq;
    int i=0;
    while(ct<n)
    {
        for(;i<n;i++)
        {
            if(pq.empty())
            {
                currtime=max(currtime,v[i].at);
            }
            if(v[i].at<=currtime)
            {
                pq.push({v[i].bt,v[i].at,v[i].pid,i});
            }
            else
            {
                break;
            }
        }
        int a=pq.top()[0];
        int b=pq.top()[1];
        int c=pq.top()[2];
        int d=pq.top()[3];
        pq.pop();
        currtime+=a;
        v[d].ct=currtime;
        z.push_back(c);
        ++ct;
        
    }
}
void srtf(int &n,int& ct,int &currtime,vector<process>&v,vector<int>&z)
{
    priority_queue<vector<int>,vector<vector<int>>,greater<vector<int>>>pq;
    int i=0;
    while(ct<n)
    {
        for(;i<n;i++)
        {
            if(pq.empty())
            {
                currtime=max(currtime,v[i].at);
            }
            if(currtime>=v[i].at)
            {
                pq.push({v[i].bt,v[i].at,v[i].pid,i});
            }
            else
            {
                break;
            }
        }
        int a=pq.top()[0];
        int b=pq.top()[1];
        int c=pq.top()[2];
        int d=pq.top()[3];
        currtime+=1;
        if(a==1)
        {
            pq.pop();
            v[d].ct=currtime;
            z.push_back(c);
            ++ct;
            
        }
        else
        {
          pq.pop();
          z.push_back(c);
          pq.push({a-1,b,c,d});
        }
    }
}
void rr(int &n,int& ct,int &currtime,vector<process>&v,vector<int>&z,int &qt)
{
    queue<vector<int>>q;
    int i=0;
    while(ct<n)
    {
       for(;i<n;i++)
       {
           if(q.empty())
           {
               currtime=max(currtime,v[i].at);
           }
           if(currtime>=v[i].at)
           {
               q.push({v[i].bt,v[i].pid,i});
           }
           else
           {
               break;
           }
       }
       int a=q.front()[0];
       int b=q.front()[1];
       int c=q.front()[2];
       q.pop();
       currtime+=min(qt,a);
       for(;i<n;i++)
       {
           if(currtime>=v[i].at)
           {
               q.push({v[i].bt,v[i].pid,i});
           }
           else
           {
               break;
           }
       }
       if(a>qt)
       {
           a-=qt;
           q.push({a,b,c});
           z.push_back(b);
       }
       else
       {
           z.push_back(b);
           ++ct;
           v[c].ct=currtime;
       }
    }
}
void prio_nonpre(vector<process>& v, vector<int>& z) {
    int till_here=0, n=v.size(), curr_time=0;
    priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>>pq;
    while(till_here<n) {
        while(till_here<n && v[till_here].at<=curr_time) {
            pq.push({-v[till_here].priority, v[till_here].bt, v[till_here].pid,till_here});
            ++till_here;
        } 
        if(!pq.empty()) {
            auto curr_process=pq.top(); pq.pop();
            curr_time+=curr_process[1];
            v[curr_process[3]].ct=curr_time;
            z.push_back(curr_process[2]);
        }
        else if(pq.empty() && till_here<n) curr_time=max(curr_time,v[till_here].at);
    }
    while(!pq.empty()) {
        auto curr_process=pq.top(); pq.pop();
        curr_time+=curr_process[1];
        v[curr_process[3]].ct=curr_time;
        z.push_back(curr_process[2]);
    }
}
void prio_pre(vector<process>& v, vector<int>& z) {
    int till_here=0, n=v.size(), curr_time=0;
    priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>>pq;
    while(till_here<n) {
        while(till_here<n && v[till_here].at<=curr_time) {
            pq.push({-v[till_here].priority, v[till_here].bt, v[till_here].pid, till_here++});
        } 
        if(!pq.empty()) {
            auto curr_process=pq.top(); pq.pop();
            curr_time++;
            curr_process[1]--;
            if(!curr_process[1]) v[curr_process[3]].ct=curr_time;
            z.push_back(curr_process[2]);
            if(curr_process[1]) pq.push(curr_process);
        }
        else if(pq.empty() && till_here<n) curr_time=max(curr_time, v[till_here].at);
    }
    while(!pq.empty()) {
        auto curr_process=pq.top(); pq.pop();
        curr_time+=curr_process[1];
        v[curr_process[3]].ct=curr_time;
        z.push_back(curr_process[2]);
    }
}
int main()
{ 
    if(!inputFile)
    {
        cout<<"Error in opening input file"<<endl;
    }
    if(!outputFile)
    {
        cout<<"Error in accessing ouput file"<<endl;
    }
    int n;
    outputFile<<"Enter Number of processes to be scheduled:-\n";
    inputFile>>n;
    vector<process>v(n);
    outputFile<<"Enter process id of all n processes in sorted order seperated by space:-\n";
    for(int i=0;i<n;i++)
    {
        inputFile>>v[i].pid;
    }
    outputFile<<"Enter Arrival Time of all n processes in seperated by space:-\n";
    for(int i=0;i<n;i++)
    {
        inputFile>>v[i].at;
    }
    outputFile<<"Enter Burst Time of all n processes seperated by space:-\n";
    for(int i=0;i<n;i++)
    {
        inputFile>>v[i].bt;
    }
    int type;
    outputFile<<"Enter 1)FCFS 2)SJF 3)SRTF 4)RR 5)Priority non preemptive 6)Priority preemptive :-\n";
    vector<int>z;
    int ct=0,currtime=0;
    inputFile>>type;
    switch (type)
    {
        case 1:
        {
            sort(v.begin(),v.end(),sorttheprocesses);
            fcfs(n,ct,currtime,v,z);
            tatandwt(v);
            printans(v,z);
            break;
        }
        case 2:
        {
            sort(v.begin(),v.end(),sorttheprocesses);
            sjf(n,ct,currtime,v,z);
            tatandwt(v);
            printans(v,z);
            break;
        }
        case 3:
        {
            sort(v.begin(),v.end(),sorttheprocesses);
            srtf(n,ct,currtime,v,z);
            tatandwt(v);
            printans(v,z);
            break;
        }
        case 4:
        {
            sort(v.begin(),v.end(),sorttheprocesses);
            outputFile<<"Enter Time Quantam for Round Robin Schedular:";
            int qt;
            inputFile>>qt;
            rr(n,ct,currtime,v,z,qt);
            tatandwt(v);
            printans(v,z);
            break;
        }
        case 5: {
            outputFile<<"Enter priorities: ";
            for(int i=0; i<n; i++) inputFile>>v[i].priority;
            sort(v.begin(),v.end(),sorttheprocesses);
            prio_nonpre(v, z);
            tatandwt(v);
            printans(v, z);
            break;
        }
        case 6:{
            outputFile<<"Enter priorities: ";
            for(int i=0; i<n; i++) inputFile>>v[i].priority;
            sort(v.begin(),v.end(),sorttheprocesses);
            prio_pre(v,z);
            tatandwt(v);
            printans(v,z);
            break;
        }
        default:
        {
            outputFile<<"Wrong  process selection index entered... try again...";
        }
    }
    inputFile.close();
    outputFile.close();

    cout<<"Output is stored Successfull in output.txt file based on input from input.txt file... please refer that:>>>";
}
