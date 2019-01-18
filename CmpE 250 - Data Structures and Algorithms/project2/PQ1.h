//
// Created by emin on 12.11.2016.
//

#ifndef PROJECT2_PQ1_H
#define PROJECT2_PQ1_H

#include <vector>
#include "Task.h"

using namespace std;

class PQ1 {

public:
    PQ1();

    Task* pop();

    void push(Task& t);

    void percolateUp(int index);

    void percolateDown(int index);

    bool isEmpty();

public:
    vector<Task*> vec;
    int currentSize;





};


#endif //PROJECT2_PQ1_H
