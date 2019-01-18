/*
Student Name:       Muhammed Emin Gure
Student Number:     2014400171
Project Number:     4
Operating System:   Ubuntu 16.04
Compile Status:     Compiles
Program Status:     Gives true results for testcases provided
Notes: Code is totally my own work except ps10's kruskal implementation

*/

#include <iostream>
#include <algorithm>
#include <fstream>
#include <set>
#include <queue>

using namespace std;

struct Edge;
int N, M;
priority_queue<Edge> edges;
set<int> g;
int result;

class DisjointSet{
public:
    int SIZE;
    int* arr;
    DisjointSet(int size){
        SIZE = size;
        arr = new int[SIZE];
        // Make each node a root.
        for (int i = 0; i < SIZE; ++i) {
            arr[i] = -1;
        }
    }
    int find(int set){
        if(arr[set] < 0){
            // Means this is a root
            return set;
        }else{
            // Call find with its root.
            return find(arr[set]);
        }
    }
    void Union(int set1,int set2){
        set1 = find(set1);
        set2 = find(set2);

        if(g.count(set1))
            arr[set2] = set1;
        else if(g.count(set2))
            arr[set1] = set2;
        else if(arr[set1] > arr[set2]){
            // First one is less deep, so make its root second.
            arr[set1] = set2;
        }else if(arr[set1] < arr[set2]){
            // Vice versa
            arr[set2] = set1;
        }else{
            // They are equal in depth. Set first one as second one's root. (Arbitrarily chosen)
            // And increase the depth.
            arr[set1]--;
            arr[set2] = set1;
        }
    }
};

struct Edge {
    int n1;
    int n2;
    double w;
    Edge(int n1_, int n2_, double w_) {
        n1 = n1_;
        n2 = n2_;
        w = w_;
    }
};

bool operator<(const Edge& e1, const Edge& e2) {
    return e1.w < e2.w;
}

void read_input(const string& filename) {
    fstream fin(filename);
    fin >> N >> M;
    int n1, n2;
    double w;

    for(int i=0; i<N-1; i++) {
        fin >> n1 >> n2 >> w;
        edges.push(Edge(n1,n2,w));
    }
    int t;
    for(int i=0; i<M; i++) {
        fin >> t;
        g.insert(t);
    }
    fin.close();
}

void kruskal() {

    DisjointSet ds(N);
    while (!edges.empty()) {
        Edge e = edges.top();
        edges.pop();
        int root1 = ds.find(e.n1);
        int root2 = ds.find(e.n2);


        if (root1 != root2){
            if((g.count(root1)) && (g.count(root2)))
                result += e.w;
            ds.Union(root1,root2);
        }
    }

}

int main(int argc, char* argv[]) {

    result = 0;
    read_input(argv[1]);

    kruskal();
    ofstream ofs (argv[2], ofstream::out);
    ofs << result;
    ofs.close();

    return 0;

}