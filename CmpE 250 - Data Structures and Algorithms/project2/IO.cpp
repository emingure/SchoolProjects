//
// Created by emin on 08.11.2016.
//

#include <cfloat>
#include "IO.h"

IO::IO(){

}

IO::IO(double quantum, int id) {
    this->quantum = quantum;
    this->id = id;
    this->idle = true;
    this->nextAvalT = DBL_MAX -1;
    this->activeTime = 0;
    this->taskId = -1;
}

void IO::busy() {
    this->idle = false;
}

void IO::assignTask(int id) {
    this->taskId = id;
}

void IO::setIdle() {
    this->idle = true;
    this->taskId = -1;
}

bool IO::operator<(IO &i) {
    if(this->nextAvalT <= i.nextAvalT)
        return true;
    return false;
}
