//
// Created by emin on 08.11.2016.
//

#include <stdexcept>
#include <iostream>
#include "PQ.h"


PQ::PQ() {
    this->currentSize = 0;
    vec.resize(1);
}

vector<double> PQ::pop() {
    if(isEmpty())
        throw std::out_of_range("Queue is empty");

    vector<double> v;

    v = std::move(vec[1]);
    vec[1] = std::move(vec[currentSize--]);
    percolateDown(1);


    return v;
}

void PQ::push(double T, int type, int index) {


    currentSize++;
    if(currentSize==vec.size())
        vec.resize(vec.size()*2);
    vec[currentSize] = {T,type*1.0,index*1.0};

    percolateUp(currentSize);

}

void PQ::percolateUp(int index) {
    for (; index/2 !=0; index /= 2) {
        if(vec[index][0] <= vec[index / 2][0]) {
            std::swap(vec[index], vec[index / 2]);
        }
    }

}

void PQ::percolateDown(int index) {

    int child;
    vector<double> tmp = std::move(vec[index]);

    for(;index*2 <= currentSize; index = child){
        child = index*2;
        if(child!=currentSize && vec[child+1][0] < vec[child][0])
            ++child;
        if(vec[child][0] < tmp[0]){
            vec[index] = std::move( vec[child]);
        }else{
            break;
        }
    }
    vec[index] = std::move(tmp);

}

bool PQ::isEmpty() {
    if(currentSize==0)
        return true;
    return false;
}


