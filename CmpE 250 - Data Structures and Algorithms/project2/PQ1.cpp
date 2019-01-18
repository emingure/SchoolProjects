//
// Created by emin on 08.11.2016.
//

#include <stdexcept>
#include <iostream>
#include "PQ1.h"


PQ1::PQ1() {
    this->currentSize = 0;
    vec.resize(1);
    vec[0] = new Task(0.0,0.0,0.0,1);
}

Task* PQ1::pop() {
    if(isEmpty())
        throw std::out_of_range("Queue is empty");

    Task *min = new Task();
    *min = std::move(*vec[1]);
    vec[1] = std::move(vec[currentSize--]);
    percolateDown(1);

    return &(*min);
}

void PQ1::push(Task &t) {


    currentSize++;
    if(currentSize==vec.size())
        vec.resize(vec.size()*2);
    vec[currentSize] = &t;
    percolateUp(currentSize);

}

void PQ1::percolateUp(int index) {
    for(; vec[index]->cpuWork < vec[index/2]->cpuWork ; index/=2)
        std::swap(vec[index],vec[index/2]);
}

void PQ1::percolateDown(int index) {

    int child;
    Task *tmp = std::move(vec[index]);

    for(;index*2 <= currentSize; index = child){
        child = index*2;
        if(child!=currentSize && *vec[child+1] < *vec[child])
            ++child;
        if(*vec[child] < *tmp){
            vec[index] = std::move( vec[child]);
        }else{
            break;
        }
    }
    vec[index] = std::move(tmp);

}

bool PQ1::isEmpty() {
    if(currentSize==0)
        return true;
    return false;
}

