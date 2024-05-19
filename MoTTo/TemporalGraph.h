#ifndef TEMPORAL_GRAPH_H
#define TEMPORAL_GRAPH_H
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <fstream>

#define HOUR 3600
#define DAY 86400
#define WEEK 604800
#define YEAR 31536000 


#define NODE_IN 0
#define NODE_OUT 1

using namespace std;

/*
 * TemporalGraphMethod class provides methods to manipulate and analyze temporal graphs.
 * A temporal graph is represented by a set of edges, each with a timestamp indicating
 * when the edge is active. This class includes methods for modifying nodes and edges,
 * as well as loading temporal graph data from a file.
 *
 * Methods:
 * - changeNodeItem: Modifies the nodes within the temporal edges.
 * - changeSubNodeItem: Modifies sub-nodes and maintains a map for topology pruning.
 * - loadFile: Loads temporal edge data from a specified file.
 */
class TemporalGraphMethod {
public: 
    TemporalGraphMethod(){}
    ~TemporalGraphMethod();

    int changeNodeItem(vector<vector<int>>& temporal_edges);

    int changeSubNodeItem(vector<vector<int>>& temporal_edges,
                       map<int, bool>& mp_topo_purn, 
                       vector<bool>& counted);
    int loadFile(const string& file_path, vector<vector<int>>& temporal_edges, bool& num_Node_flag);
};

#endif