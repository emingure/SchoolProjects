//
// Created by emin on 08.11.2016.
//

#ifndef PROJECT2_IO_H
#define PROJECT2_IO_H


class IO {
public:
    IO();
    IO(double quantum, int id);
    void busy();
    void assignTask(int id);
    void setIdle();
    bool operator<(IO& i);

public:
    double quantum;
    int id;
    bool idle;
    double nextAvalT;
    int taskId;
    double activeTime;
};


#endif //PROJECT2_IO_H
