//
// Created by emin on 08.11.2016.
//

#ifndef PROJECT2_CPU_H
#define PROJECT2_CPU_H


class CPU {
public:
    CPU();
    CPU(double frequency, int id);
    void busy();
    void assignTask(int id);
    bool operator<(CPU& c);

public:
    double frequency;
    int id;
    bool idle;
    double nextAvalT;
    int taskId;
    double activeTime;
};


#endif //PROJECT2_CPU_H
