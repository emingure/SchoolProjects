/*
Student Name:       Muhammed Emin Gure
Student Number:     2014400171
Project Number:     5
Operating System:   Ubuntu 16.04
Compile Status:     Compiles
Program Status:     Gives true results with testcases provided
Notes:

*/

#include <vector>
#include <queue>
#include <climits>
#include <stdio.h>

using namespace std;


class compare
{
public:
    bool operator() (std::pair<int, int> a, std::pair<int, int> b)
    {
        return a.second > b.second;
    }
};


int main(int argc, char* argv[]){

    int V,E;
    vector<vector<pair<int,int>>> graph;
    int source, dest;

    FILE * ioFile;

    ioFile = fopen(argv[1],"r");
    fscanf(ioFile,"%i %i",&V,&E);

    graph.resize(V);
    int h[V];
    int f[V];

    int cost[V];
    for(int i=0; i<V; i++) {
        fscanf(ioFile,"%i",&h[i]);
        f[i] = 0;
        cost[i] = INT_MAX;
    }

    for(int i=0; i<E; i++){
        int from = 0, to = 0, weight = 0;
        fscanf(ioFile,"%i %i %i",&from,&to,&weight);
        graph[from].push_back(make_pair(to,weight));
        graph[to].push_back(make_pair(from, weight));
    }
    fscanf(ioFile,"%i %i", &source, &dest);

    cost[source] = 0;

    priority_queue<pair<int,int>, vector<pair<int,int>>, compare> q;

    q.push(make_pair(source,0));

    while(!q.empty()){
        pair<int, int> a = q.top();
        int current = a.first;
        q.pop();
        if(current == dest)
            break;

        for(int i=0; i<graph[current].size(); i++){
            pair<int,int> edge = graph[current].at(i);
            int nCost = cost[current] + edge.second;
            if(cost[edge.first] > nCost) {
                cost[edge.first] = nCost;
                f[edge.first] = h[edge.first] + nCost;
                q.push(make_pair(edge.first, f[edge.first]));
            }
        }
    }

    ioFile = fopen(argv[2],"w+");

    fprintf(ioFile,"%i", cost[dest]);

    return 0;
}