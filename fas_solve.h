#ifndef ALGORITHMSHW_FAS_SOLVE_H_
#define ALGORITHMSHW_FAS_SOLVE_H_

#include <vector>
#include <set>
#include <string>
using namespace std;
using AdjList = vector<vector<int>>;
using PairSet = set<pair<int, int>>;

class FasSolution {
    public:
        FasSolution(const string input, int nodes, int arcs = -1);
        // FasSolution(const string input, const string property);
        FasSolution(AdjList g, int nodes, int arcs = -1);
        auto SortFAS() -> void;
        void ComputeFeedbackArcSet();
        void once_forward();
        // n=-1 run untile converge, otherwise run n times
        void multiple_forward(int n);
        const vector<int> &showarr() {
            return linear_arrangement;
        }
        const PairSet &showfb() {
            return fbarc_set;
        }
        const int &showfbsize() {
            return fbarcs_size;
        }
        const int &showruntime() {
            return runtimes;
        }
        

    private:
        AdjList graph;
        PairSet fbarc_set; 
        vector<int> linear_arrangement;
        vector<int> edges_num;
        int fbarcs_size;
        int arcs;
        int nodes;
        int runtimes;
        void GetGraphInfo(const string &path);
        bool BinarySearch(const vector<int> &array, int object, const int &edg_n);

    

};

#endif // ALGORITHMSHW_FAS_SOLVE_H_
