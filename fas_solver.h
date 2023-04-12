#ifndef ALGORITHMSHW_FAS_SOLVE_H_
#define ALGORITHMSHW_FAS_SOLVE_H_

#include <vector>
#include <set>
#include <string>
using namespace std;
using AdjList = vector<vector<int>>;
using PairSet = set<pair<int, int>>;

class FasSolver {
    public:
        FasSolver(const string data_path, int num_nodes, int num_arcs = -1);
        void loadGraph(const string &data_path);
        bool hasDirectedEdge(int u, int v, bool binary_search);
        void sortFAS();
        void runSortFAS(bool until_converge, int max_iters);
        void computeFeedbackArcSet();
        void once_forward();
        // n=-1 run untile converge, otherwise run n times
        void multiple_forward(int n);
        const vector<int> &getLinearArrangement() {
            return linear_arrangement;
        }
        const PairSet &getFAS() {
            return feedback_arc_set;
        }
        const int &getFASSize() {
            return fas_size;
        }
        const int &getIterations() {
            return iterations;
        }
        

    private:
        AdjList graph;
        PairSet feedback_arc_set; 
        vector<int> linear_arrangement;
        vector<int> edges_num;
        int fas_size;
        int num_arcs;
        int num_nodes;
        int iterations;
};

#endif // ALGORITHMSHW_FAS_SOLVE_H_
