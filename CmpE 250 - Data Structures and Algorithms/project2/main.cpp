#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <cfloat>
#include <iomanip>
#include "PQ.h"
#include "CPU.h"
#include "IO.h"
#include "PQ1.h"

/*
Student Name:       Muhammed Emin Gure
Student Number:     2014400171
Project Number:     2
Operating System:   Ubuntu 16.04
Compile Status:
Program Status:
Notes: Anything you want to say about your code that will be helpful in the grading process.

*/

using namespace std;


double toDouble(string &str) {
    return atof(str.c_str());
}

int toInt(string &str){
    return atoi(str.c_str());
}

int main(int argc, char* argv[]) {

    // below reads the input file
    // in your next projects, you will implement that part as well
    if (argc != 3) {
        cout << "Run the code with the following command: ./project2 [input_file] [output_file]" << endl;
        return 1;
    }

    int maxPQ1 = 0, maxPQ2 = 0;

    ifstream infile(argv[1]);
    string line;
    vector<string> input;
    while (getline(infile, line)) {
        istringstream iss(line);
        input.push_back(line);
    }

    PQ events;

    int nOfCpu = toInt(input[0]);
    vector<CPU*> cpu(nOfCpu);
    for (int i = 0; i < nOfCpu; ++i){
        cpu[i] = new CPU(toDouble(input[i + 1]),i);
        events.push(cpu[i]->nextAvalT, 1, i);
    }

    int nOfIo = toInt(input[toDouble(input[0]) + 1]);
    vector<IO*> io(nOfIo);
    for (int i = 0; i < nOfIo; i++){
        io[i] = new IO((toDouble(input[toInt(input[0]) + 2 + i])), i);
        events.push(io[i]->nextAvalT,2,i);
    }

    PQ1 pq1;
    queue<Task*> pq2;

    int nOfTask = toInt(input[nOfCpu + nOfIo + 2]);
    vector<Task> task(nOfTask);
    for (int i = 0; i < nOfTask; i++) {
        string str = input[nOfIo + nOfCpu + 3 + i];
        vector<string> arr(3);
        int j = 0;
        stringstream ssin(str);
        while (ssin.good() && j < 4) {
            ssin >> arr[j];
            ++j;
        }
        task[i] = *new Task(toDouble(arr[0]), toDouble(arr[1]), toDouble(arr[2]), i);
        events.push(task[i].arrivalTime,0,i);
    }

    double *T = new double(0);

    while(!events.isEmpty()){

        bool processed1 = false;
        vector<double> current = events.pop();

        if(current[0] == DBL_MAX)
            break;
        *T = current[0];
        if(current[1] == 0){
            Task* cTask = &task[current[2]];
            for(CPU* c : cpu){
                if(c->idle) {
                    c->busy();
                    processed1 = true;
                    c->assignTask(cTask->id);
                    double tmp = cTask->cpuWork / c->frequency;
                    cTask->serviceTime += tmp;
                    c->activeTime += tmp;
                    c->nextAvalT = *T + tmp;
                    events.push(c->nextAvalT,1,c->id);
                    break;
                }
            }
            if(!processed1) {
                pq1.push(*cTask);
                if(maxPQ1 < pq1.currentSize)
                    maxPQ1 = pq1.currentSize;
            }
        }else if(current[1] == 1){
            CPU* c = cpu[current[2]];
            *T = c->nextAvalT;
            c->idle = 1;
            Task *cTask = &task[c->taskId];
            bool processed2 = false;
            for(IO* i : io) {
                if (i->idle) {
                    if (pq2.empty()) {
                        i->taskId = cTask->id;
                    } else {
                        i->taskId = pq2.front()->id;
                        pq2.pop();
                    }
                    i->busy();
                    if (task[i->taskId].ioWork <= i->quantum) {
                        task[i->taskId].serviceTime += task[i->taskId].ioWork;
                        i->activeTime += task[i->taskId].ioWork;
                        i->nextAvalT = *T + task[i->taskId].ioWork;
                        task[i->taskId].ioWork = 0;
                    } else {
                        task[i->taskId].serviceTime += i->quantum;
                        i->activeTime += i->quantum;
                        i->nextAvalT = *T + i->quantum;
                        task[i->taskId].ioWork -= i->quantum;
                    }
                    events.push(i->nextAvalT, 2, i->id);
                    processed2 = true;
                    break;
                }
            }
            if(!processed2){
                pq2.push(cTask);
                if(maxPQ2 < pq2.size())
                    maxPQ2 = pq2.size();

            }
            cTask = nullptr;


            if(!pq1.isEmpty()) {
                Task *cTask1 = pq1.pop();
                c->busy();
                processed1 = true;
                c->assignTask(cTask1->id);
                double tmp = cTask1->cpuWork / c->frequency;
                task[cTask1->id].serviceTime += tmp;
                c->activeTime += tmp;
                c->nextAvalT = *T + tmp;
                events.push(c->nextAvalT, 1, c->id);
                cTask1 = nullptr;
            }
        }else if(current[1] == 2){
            IO* i = io[current[2]];
            *T = i->nextAvalT;
            if (task[i->taskId].ioWork != 0.0) {
                pq2.push(&task[i->taskId]);
            }else {
                task[i->taskId].updateExitTime(i->nextAvalT);
            }
            i->setIdle();
            if(!pq2.empty()){
                Task * cTask = pq2.front();
                pq2.pop();
                i->assignTask(cTask->id);
                i->busy();
                if (task[i->taskId].ioWork <= i->quantum) {
                    task[i->taskId].serviceTime += task[i->taskId].ioWork;
                    i->activeTime += task[i->taskId].ioWork;
                    i->nextAvalT = *T + task[i->taskId].ioWork;
                    task[i->taskId].ioWork = 0;
                } else {
                    task[i->taskId].serviceTime += i->quantum;
                    i->activeTime += i->quantum;
                    i->nextAvalT = *T + i->quantum;
                    task[i->taskId].ioWork -= i->quantum;
                }
                events.push(i->nextAvalT, 2, i->id);
            }
        }



    }

    int mostActiveCPU = 0;
    double activeTime = 0;
    for(CPU* c: cpu){
        if(activeTime < c->activeTime) {
            activeTime = c->activeTime;
            mostActiveCPU = c->id + 1;
        }
    }

    int mostActiveIO = 0;
    activeTime = 0;
    for(IO* i: io){
        if(activeTime < i->activeTime){
            activeTime = i->activeTime;
            mostActiveIO = i->id + 1;
        }
    }

    double averageWaitTime = 0;
    double totalWaitTime = 0;
    double longestWaitTime = 0;
    double averageTimeSpent = 0;
    double totalTimeSpent = 0;
    for(Task& t: task){
        double waitTime = t.exitTime-t.arrivalTime-t.serviceTime;
        totalWaitTime = totalWaitTime + waitTime;
        if(waitTime > longestWaitTime)
            longestWaitTime = waitTime;
        totalTimeSpent = totalTimeSpent + (t.exitTime - t.arrivalTime);
    }

    averageTimeSpent = totalTimeSpent/nOfTask;
    averageWaitTime = totalWaitTime/nOfTask;


    ofstream ofs (argv[2], ofstream::out);

    ofs << maxPQ1 << endl;

    ofs << maxPQ2 << endl;

    ofs << mostActiveCPU << endl;

    ofs << mostActiveIO << endl;

    ofs << fixed << setprecision(6) << averageWaitTime << endl;

    ofs << fixed << setprecision(6) << longestWaitTime << endl;

    ofs << fixed << setprecision(6) << averageTimeSpent << endl;

    ofs.close();


    return 0;
}