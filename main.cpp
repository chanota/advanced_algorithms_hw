#include "fas_solver.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <numeric>
#include <string>
#include <vector>
#include <chrono>
#include <set>


FasSolver::FasSolver(const string data_path, const int nn):num_nodes(nn) {
    iterations = 0;
    graph.resize(num_nodes);
    loadGraph(data_path);
    linear_arrangement.resize(num_nodes);
    iota(linear_arrangement.begin(), linear_arrangement.end(), 0);
}

// Load graph from txt file
void FasSolver::loadGraph(const string &data_path) {
    cout << "Loading graph from " << data_path << endl;
    ifstream readtxt(data_path);
    if (!readtxt) {
        cerr << "Failed to open input file." << endl;
        exit(1);
    }
    string txtline;
    int from, to;
    int edge_count = 0;
    while (getline(readtxt, txtline)) {
        stringstream ss(txtline);
        string str_from, str_to;
        if (getline(ss, str_from, ',') && getline(ss, str_to, ',')) {
            from = stoi(str_from);
            to = stoi(str_to);
            graph[from].push_back(to);
            edge_count++;   
        }
    }
    readtxt.close();
    num_arcs = edge_count;
    cout << "number of nodes: " << num_nodes << ", number of arcs: " << num_arcs << endl;
}

bool FasSolver::hasDirectedEdge(int u, int v, bool binary_search) {
    if (binary_search) {
        auto it = std::lower_bound(graph[u].begin(), graph[u].end(), v);
        return it != graph[u].end() && *it == v;
    } else {
        return find(graph[u].begin(), graph[u].end(), v) != graph[u].end();
    }
}

void FasSolver::sortFAS() {
    // main loop over each element of linear_arrangement
    for (int i = 1; i < num_nodes; i++) {
        int curr = linear_arrangement[i];
        int val = 0;
        int min = 0;
        int best_pos = i;
        // check all candidate positions before i
        for (int j = i - 1; j >= 0; j--) {
            // 三种方式，当两个节点间存在相互指向的边时，前两种方式对最终结果有影响
            // 1. （原文伪代码）优先执行val--
            // if (hasDirectedEdge(curr, linear_arrangement[j])) {
            //     val--;
            // } else if (hasDirectedEdge(linear_arrangement[j], curr)) {
            //     val++;
            // }
            // 2. 优先执行val++
            // if (hasDirectedEdge(linear_arrangement[j], curr)) {
            //     val++;
            // } else if (hasDirectedEdge(curr, linear_arrangement[j])) {
            //     val--;
            // }
            // 3. 既执行val++又执行val--
            if (hasDirectedEdge(curr, linear_arrangement[j])) {
                val--;
            }
            if (hasDirectedEdge(linear_arrangement[j], curr)) {
                val++;
            }
            if (val <= min) {
                min = val;
                best_pos = j;
            }
        }
        // insert linear_arrangement[i] to the best position
        linear_arrangement.insert(linear_arrangement.begin() + best_pos, curr);
        linear_arrangement.erase(linear_arrangement.begin() + i + 1);
    }
}


// store the feedback arc set based on current linear arrangement
void FasSolver::computeFeedbackArcSet() {
    fas_size = 0;
    feedback_arc_set.clear();
    vector<int> index(num_nodes, -1);
    for (int i = 0; i < num_nodes; i++) {
        index[linear_arrangement[i]] = i;
    }
    for (int u = 0; u < num_nodes; u++) {
        for (int v : graph[u]) {
            if (index[u] < index[v]) 
                continue;  // only consider edges from later nodes to earlier nodes
            fas_size += 1;
            feedback_arc_set.emplace(u, v);  // add the edge to the feedback set if it's a feedback arc
        }
    }
}

void FasSolver::runSortFAS(bool until_convergence, int max_iters) {
    // while new_fas_size < old_fas_size, keep running sortFAS
    if (until_convergence) {
        int old_fas_size = num_arcs + 1;
        int new_fas_size = num_arcs;
        while (new_fas_size < old_fas_size) {
            old_fas_size = new_fas_size;
            sortFAS();
            computeFeedbackArcSet();
            cout << "iter: " << iterations << ", fas size: " << fas_size << endl;
            new_fas_size = fas_size;
            iterations += 1;
            if (iterations == max_iters) {
                break;
            }
        }
    } else {
        sortFAS();
        computeFeedbackArcSet();
        iterations += 1;
    }
}

int main(int argc, char **argv) {
    auto start_time = chrono::high_resolution_clock::now();
    // basename = argv[1], default: "wordassociation-2011"
    // num_nodes = argv[2], default: 10617
    string basename;
    int num_nodes;
    if (argc > 1) {
        basename = argv[1];
        num_nodes = stoi(argv[2]);
    }
    else {
        basename = "wordassociation-2011";
        num_nodes = 10617;
    }
    const string data_path = "./graph_datasets/" + basename + ".txt";
    FasSolver fas_solver(data_path, num_nodes);

    //fas_solver.runSortFAS();
    fas_solver.runSortFAS(true, -1);

    // show the linear arrangement
    for (int v : fas_solver.getLinearArrangement()) {
       cout << v << " ";
    }

    // show the feedback arc set
    cout << "\nFeedback arc set: ";
    for (auto arc : fas_solver.getFAS()) {
        cout << "(" << arc.first << ", " << arc.second << ") ";
    }
    cout << "\nfas size : " << fas_solver.getFASSize() << endl; 
    cout << "iterations: " << fas_solver.getIterations() << endl;
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    cout << "run_time: " << duration.count() / 1000000.0 << " seconds" << endl;
    return 0;
}
