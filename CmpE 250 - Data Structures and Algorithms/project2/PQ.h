//
// Created by emin on 08.11.2016.
//

#ifndef PROJECT2_PQ_H
#define PROJECT2_PQ_H

#include <vector>
#include "Task.h"

using namespace std;

class PQ {
public:
    PQ();

    vector<double> pop();

    void push(double T, int type, int index);

    void percolateUp(int index);

    void percolateDown(int index);

    bool isEmpty();

public:
    vector<vector<double>> vec;
    int currentSize;
};


#endif //PROJECT2_PQ_H
