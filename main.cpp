#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

// Graph is represented as an adjacency list
using AdjList = vector<vector<int>>;
using Pair = set<pair<int, int>>; 

    

auto sortFAS(AdjList& graph, vector<int>& arrangement) -> vector<int>* {
    vector<int> &linear_arrangement = arrangement;
    for (int v = 0; v < linear_arrangement.size(); v++) {
        int back_arcs = 0;
        int min_back_arcs = 0;
        int best_pos = v;
        for (int i = v - 1; i > -1; i--) {
            int u = linear_arrangement[i];
            if (find(graph[u].begin(), graph[u].end(), linear_arrangement[v]) != graph[u].end()) {
                    back_arcs++;
                }else if (find(graph[linear_arrangement[v]].begin(), graph[linear_arrangement[v]].end(), u) != graph[linear_arrangement[v]].end()) {
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
    return &linear_arrangement;
}


// Given a sorted order of nodes and the original graph, computes a minimum feedback arc set
// and stores it in the set `feedbackSet`.
void computeFeedbackArcSet(vector<int>& arrangement, const AdjList& graph, set<pair<int, int>>& feedbackSet) {
    int n = graph.size();
    vector<int> index(n, -1);
    for (int i = 0; i < n; i++) {
        index[arrangement[i]] = i;
    }
    for (int u = 0; u < n; u++) {
        for (int v : graph[u]) {
            if (index[u] < index[v]) continue;  // only consider edges from later nodes to earlier nodes
            feedbackSet.emplace(u, v);  // add the edge to the feedback set if it's a feedback arc
        }
    }
}

int main() {
    AdjList graph = {{1,2}, {2}, {3}, {4,5,6}, {6}, {4,7}, {0}, {1,2}};
    vector<int> arrangement = {0, 1, 2, 3, 4, 5, 6, 7};
    // vector<vector<int>> graph = {{1}, {2}, {3}, {0, 1}, {5}, {6}, {4}};
    // vector<int> arrangement = {0, 1, 2, 3, 4, 5, 6};
    // vector<vector<int>> graph = {{1}, {2}, {3}, {0, 1}};
    // vector<int> arrangement = {0, 1, 2, 3};
    vector<int> *linear_arrangement = &arrangement; 
    // Run multiple times
    for (int j = 0; j < 100; j++) {
        linear_arrangement = sortFAS(graph, *linear_arrangement);
        if (j == 0) {
            cout << "first time running result: " << endl;
            for (int v : *linear_arrangement) {
                cout << v << " ";
            }
            Pair fb;
            computeFeedbackArcSet(*linear_arrangement, graph, fb);
            cout << "Feedback arc set: ";
            for (auto edge : fb) {
                cout << "(" << edge.first << ", " << edge.second << ") ";
            }
        }
    }
    cout << endl << endl;
    cout << "final result (after 100 times running): " << endl;
    for (int v : *linear_arrangement) {
        cout << v << " ";
    }

    Pair feedbackSet;    
    computeFeedbackArcSet(*linear_arrangement, graph, feedbackSet);
    cout << "Feedback arc set: ";
    for (auto edge : feedbackSet) {
        cout << "(" << edge.first << ", " << edge.second << ") ";
    }
    cout << endl;
    return 0;
}
