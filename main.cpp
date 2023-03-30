#include <iostream>
#include <vector>
#include <set>
#include <algorithm>  // for std::min_element

using namespace std;

// Graph is represented as an adjacency list
using AdjList = vector<vector<int>>;

// Computes the number of back arcs induced by adding node v at position pos
int num_backarcs(const AdjList& graph, const vector<int>& arrangement, int v, int pos) {
    int count = 0;
    for (int i = 0; i < pos; i++) {
        int w = arrangement[i];
        if (find(graph[v].begin(), graph[v].end(), w) != graph[v].end()) {
            count--;
        } else if (find(graph[w].begin(), graph[w].end(), v) != graph[w].end()) {
            count++;
        }
    }
    return count;
}

// SortFAS algorithm
void sortFAS(const AdjList& graph, vector<int>& arrangement) {
    int n = graph.size();
    vector<int> backarcs(n, 0);  // number of back arcs induced by each node in the current arrangement
    for (int i = 0; i < n; i++) {
        backarcs[arrangement[i]] = num_backarcs(graph, arrangement, arrangement[i], i);
    }
    for (int i = 1; i < n; i++) {
        int v = arrangement[i];
        int best_pos = i;
        int min_backarcs = backarcs[v];
        for (int j = i-1; j >= 0; j--) {
            int w = arrangement[j];
            int new_backarcs = backarcs[v] - (find(graph[v].begin(), graph[v].end(), w) != graph[v].end())
                                              + (find(graph[w].begin(), graph[w].end(), v) != graph[w].end());
            if (new_backarcs <= min_backarcs) {
                best_pos = j;
                min_backarcs = new_backarcs;
            }
        }
        if (best_pos != i) {
            arrangement.erase(arrangement.begin() + i);
            arrangement.insert(arrangement.begin() + best_pos, v);
            for (int j = i; j > best_pos; j--) {
                backarcs[arrangement[j]] = backarcs[arrangement[j-1]] +
                                            (find(graph[arrangement[j]].begin(), graph[arrangement[j]].end(), arrangement[j-1]) != graph[arrangement[j]].end())
                                            - (find(graph[arrangement[j-1]].begin(), graph[arrangement[j-1]].end(), arrangement[j]) != graph[arrangement[j-1]].end());
            }
            backarcs[v] = min_backarcs;
        }
    }
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
    // Example graph
    AdjList graph = {{1}, {2}, {3}, {0,1}};
    vector<int> arrangement = {0, 1, 2, 3};  // initial linear arrangement

    sortFAS(graph, arrangement);
    cout << "Sorted order of nodes: ";
    for (int v : arrangement) {
        cout << v << " ";
    }
    cout << endl;

    set<pair<int, int>> feedbackSet;    
    computeFeedbackArcSet(arrangement, graph, feedbackSet);
    cout << "Feedback arc set: ";
    for (auto edge : feedbackSet) {
        cout << "(" << edge.first << ", " << edge.second << ") ";
    }
    return 0;
}