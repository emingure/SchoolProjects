//
// Created by emin on 08.11.2016.
//
#include "Task.h"

Task::Task() {

}

Task::Task(double arrivalTime, double cpuWork, double ioWork, int id) {
    this->arrivalTime = arrivalTime;
    this->cpuWork = cpuWork;
    this->ioWork = ioWork;
    this->id = id;
    this->exitTime = 0;
    this->serviceTime = 0;
}

Task::Task(const Task &t){
    this->cpuWork = t.cpuWork;
    this->arrivalTime = t.arrivalTime;
    this->exitTime = t.exitTime;
    this->ioWork = t.ioWork;
    this->serviceTime = t.serviceTime;
    this->id = t.id;
}

void Task::updateExitTime(double time) {
    this->exitTime = time;
}

bool Task::operator<(const Task& t){
    if(this->cpuWork < t.cpuWork)
        return true;
    return false;
}

bool Task::operator>(const Task &t) {
    if(this->cpuWork > t.cpuWork)
        return true;
    return false;
}