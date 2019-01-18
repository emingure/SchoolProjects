//
// Created by emin on 08.11.2016.
//

#ifndef PROJECT2_TASK_H
#define PROJECT2_TASK_H


class Task {
public:
    Task();

    Task(double arrivalTime, double cpuWork, double ioWork, int id);

    Task(const Task &t);

    void updateExitTime(double time);

    bool operator<(const Task& t);

    bool operator>(const Task& t);
public:
    double arrivalTime;
    double cpuWork;
    double ioWork;
    double exitTime;
    double serviceTime;
    int id;

};


#endif //PROJECT2_TASK_H
