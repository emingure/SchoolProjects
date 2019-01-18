/*
Student Name:       Muhammed Emin Güre
Student Number:     2014400171
Project Number:     3
Operating System:   Ubuntu 16.04
Compile Status:     Compiles
Program Status:     All testcases matched
Notes: I used the implementation which is shown in PS8 to do the topological sort.
*/

#include <iostream>
#include <queue>
#include <algorithm>
#include <fstream>
#include <iomanip>

using namespace std;

int main(int argc, char* argv[]) {

    // below reads the input file
    // in your next projects, you will implement that part as well
    if (argc != 3) {
        cout << "Run the code with the following command: ./project3 [input_file] [output_file]" << endl;
        return 1;
    }

    int N,M;
    fstream fin(argv[1]);
    fin >> N >> M;
    int indegree[N];
    vector<vector<int>> graph(N);
    double time[N];
    int from, to;

    for(int i=0; i<N; i++){
        indegree[i]=0;
        fin >> time[i];
    }

    for(int i=0; i<M; i++) {
        fin >> from >> to;
        graph[from].push_back(to);
        indegree[to]++;
    }
    fin.close();

    queue<int> zero_list;
    vector<double> finn(N);

    for(int i=0; i<N; i++){
        finn[i] = 0;
        if(indegree[i]==0){
            zero_list.push(i);
            finn[i] = time[i];
        }
    }

    int cnt = 0;
    int topOr[N];

    while(!zero_list.empty()){
        int node = zero_list.front();
        zero_list.pop();
        topOr[cnt] = node;
        cnt++;
        for(int i=0; i<graph[node].size(); i++){
            indegree[graph[node][i]]--;
            if(indegree[graph[node][i]]==0){
                zero_list.push(graph[node][i]);
            }
        }
    }

    ofstream ofs (argv[2], ofstream::out);
    if (cnt<N) {
        ofs << fixed << setprecision(6) << -1.0 << endl;
        return 0;
    }

    double max = 0;

    for(int i=0; i< N; i++){
        from = topOr[i];
        while(!graph[from].empty()){
            to = graph[from].back();
            graph[from].pop_back();
            finn[to] = std::max(finn[to], (finn[from] + time[to]) );
            if(max < finn[to]) max = finn[to];
        }
    }

    ofs << fixed << setprecision(6) << max << endl;
    ofs.close();

    return 0;
}