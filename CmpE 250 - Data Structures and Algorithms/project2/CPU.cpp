//
// Created by emin on 08.11.2016.
//

#include <cfloat>
#include "CPU.h"

CPU::CPU() {

}

CPU::CPU(double frequency, int id) {
    this->frequency = frequency;
    this->id = id;
    this->nextAvalT = DBL_MAX - 1;
    this->idle = true;
    this->taskId = -1;
    this->activeTime = 0;
}

void CPU::busy() {
    this->idle = false;
}

void CPU::assignTask(int id) {
    this->taskId = id;
}

bool CPU::operator<(CPU &c) {
    if(this->nextAvalT <= c.nextAvalT)
        return true;
    return false;
}

