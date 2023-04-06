#include "fas_solve.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <string>
#include <vector>
#include <chrono>
#include <set>

// using namespace std;

// Graph is represented as an adjacency list
// using AdjList = vector<vector<int>>;
// using PairSet = set<pair<int, int>>; 

FasSolution::FasSolution(const string input, const int nn, const int an):arcs(an), nodes(nn) {
    runtimes = 0;
    fbarcs_size = -1;
    linear_arrangement.resize(nodes);
    graph.reserve(nodes);
    edges_num.reserve(nodes);
    GetGraphInfo(input);
    iota(linear_arrangement.begin(), linear_arrangement.end(), 0); 
}

FasSolution::FasSolution(AdjList g, const int nn, const int an):arcs(an), nodes(nn) {
    runtimes = 0;
    fbarcs_size = -1;
    linear_arrangement.resize(nodes);
    edges_num.reserve(nodes);
    graph.reserve(nodes);
    for (int i = 0; i < nodes; i++) {
        edges_num[i] = g[i].size();
    }
    graph = g;
    iota(linear_arrangement.begin(), linear_arrangement.end(), 0); 
}

void FasSolution::GetGraphInfo(const string &path) {
    string txtline;
    ifstream readtxt(path);
    int from, to;
    int count = 0;
    int oldfrom = -1;
    int edges = 0;
    while (getline(readtxt, txtline, ',')) {
        istringstream line_f(txtline);
        line_f >> from;
        if (oldfrom == from || oldfrom == -1) {
            edges += 1;

        } else if (oldfrom != from) {
            edges_num[oldfrom] = edges;
            edges = 1;
            oldfrom = from;
        }
        getline(readtxt, txtline);
        istringstream line_t(txtline);
        line_t >> to;
        graph[from].push_back(to);
        count++;
    }
    arcs = count;
    cout << "arcs count: " << arcs;
}

bool FasSolution::BinarySearch(const vector<int> &array, int object, const int &edg_n) {
    if (edg_n) {
        int f = 0;
        int b = edg_n - 1;
        int media = (f + b) / 2;
        int flag = 0;
        while (f <= b) {
            if (array[media] > object) {
                b = media - 1;
            } else if (array[media] < object) {
                f = media + 1;
            } else {
                flag = 1;
                break;
            }
            media = (f + b) / 2;
        }
        return flag; 
    } 
    return 0;
    
}

auto FasSolution::SortFAS() -> void {
    for (int v = 0; v < linear_arrangement.size(); v++) {
        int back_arcs = 0;
        int min_back_arcs = 0;
        int best_pos = v;
        for (int i = v - 1; i > -1; i--) {
            int u = linear_arrangement[i];
            // if (find(graph[u].begin(), graph[u].end(), linear_arrangement[v]) != graph[u].end())
            if (BinarySearch(graph[u], linear_arrangement[v], edges_num[u])) {
                    back_arcs++;
                //else if (find(graph[linear_arrangement[v]].begin(), graph[linear_arrangement[v]].end(), u) != graph[linear_arrangement[v]].end()) 
                } else if (BinarySearch(graph[v], u, edges_num[v])) {
                    back_arcs--;
                }
            
            // In case of a tie the leftmost position is taken.
            if (back_arcs <= min_back_arcs) {
                min_back_arcs = back_arcs;
                best_pos = i;
            }
        }
        linear_arrangement.insert(linear_arrangement.begin() + best_pos, linear_arrangement[v]);
        linear_arrangement.erase(linear_arrangement.begin() + v + 1);
    }
}


// Given a sorted order of nodes and the original graph, computes a minimum feedback arc set
// and stores it in the set `feedbackSet`.
void FasSolution::ComputeFeedbackArcSet() {
    int n = nodes;
    fbarcs_size = 0;
    vector<int> index(n, -1);
    for (int i = 0; i < n; i++) {
        index[linear_arrangement[i]] = i;
    }
    for (int u = 0; u < n; u++) {
        for (int v : graph[u]) {
            if (index[u] < index[v]) 
                continue;  // only consider edges from later nodes to earlier nodes
            fbarcs_size += 1;
            fbarc_set.emplace(u, v);  // add the edge to the feedback set if it's a feedback arc
        }
    }
}
// no checking mechanism for convergence. if want to know how many times should run for convergence, just use 
// multiple_forward setting n = -1, without using once_forward.
void FasSolution::once_forward() {
   SortFAS();
   if (runtimes)
      PairSet().swap(fbarc_set);
   ComputeFeedbackArcSet();
   runtimes += 1;
}
void FasSolution::multiple_forward(int n) {
    if (n != -1) {
        for (int i = 0; i < n ; i++) {
            runtimes += 1;
            SortFAS();
        }
        if (runtimes != n)
            PairSet().swap(fbarc_set);
        ComputeFeedbackArcSet();
    } else {
        vector<int> oldarr;
        do {
            oldarr = linear_arrangement;
            SortFAS();
            runtimes += 1;
        } while(oldarr != linear_arrangement);
        if (runtimes != n)
          PairSet().swap(fbarc_set);
        ComputeFeedbackArcSet();
    }

}

int main() {
    auto start = chrono::high_resolution_clock::now();
    // const string path = "./source/enron.txt"; 
    // FasSolution s(path, 69244);
    // s.multiple_forward(-1);
    //
    AdjList graph = {{1,2}, {2}, {3}, {4,5,6}, {6}, {4,7}, {0}, {1,2}};
    FasSolution s(graph, 8, 13);
    // vector<vector<int>> graph = {{1}, {2}, {3}, {0, 1}, {5}, {6}, {4}};
    // vector<vector<int>> graph = {{1}, {2}, {3}, {0, 1}};
    // s.once_forward();
    s.multiple_forward(-1);

    //const vector<int> &linear_arrangement = s.showarr(); 
    //for (int v : linear_arrangement) {
     //   cout << v << " ";
    //}

    const PairSet &feedbackSet = s.showfb();    
    cout << "Feedback arc set: ";
    //for (auto edge : feedbackSet) {
     //   cout << "(" << edge.first << ", " << edge.second << ") ";
    //}
    cout << " size : " << s.showfbsize(); 
    cout << endl;
    cout << "run_times: " << s.showruntime() << endl;
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "run_time: " << duration.count() << " microseconds" << endl;
    return 0;
}
