#include "TemporalGraph.h"

TemporalGraphMethod::~TemporalGraphMethod() {}

int TemporalGraphMethod::loadFile(const string& file_path, 
                                  vector<vector<int>>& temporal_edges,
                                  bool& num_Node_flag) {
    
    ifstream infile(file_path, ios::in);
    if(!infile.is_open()) { 
        cout <<"lose file" << endl;
        return -1; 
    }
    while(!infile.eof()) {
        int n1, n2, tmp;
        infile >> n1 >> n2 >> tmp;
        if(n1 == n2) {
            continue;
        }
        temporal_edges.push_back({n1, n2, tmp});
    }
    infile.close();

    int node_num = changeNodeItem(temporal_edges);
    num_Node_flag = (node_num > 2 * DAY);
    return node_num;
}

int TemporalGraphMethod::changeNodeItem(vector<vector<int>>& temporal_edges) {
    set<int> st_nodes;
    map<int, int> mp_node2new;
    for(vector<int> temporal_edge: temporal_edges) {
        st_nodes.insert(temporal_edge[NODE_IN]);
        st_nodes.insert(temporal_edge[NODE_OUT]);
    }
    int new_node_item = 0;
    for(const int& node: st_nodes) {
        mp_node2new[node] = new_node_item++;
    }
    for(vector<int>& edge: temporal_edges) {
        edge[NODE_IN] = mp_node2new[edge[NODE_IN]];
        edge[NODE_OUT] = mp_node2new[edge[NODE_OUT]];
    }
    return st_nodes.size();
}

int TemporalGraphMethod::changeSubNodeItem(vector<vector<int>>& temporal_edges,
                       map<int, bool>& mp_topo_purn, 
                       vector<bool>& counted) {
    
    set<int> st_nodes;
    map<int, int> mp_node2new;
    for(vector<int> temporal_edge: temporal_edges) {
        st_nodes.insert(temporal_edge[NODE_IN]);
        st_nodes.insert(temporal_edge[NODE_OUT]);
    }
    int new_node_item = 0;
    for(const int& node: st_nodes) {
        mp_node2new[node] = new_node_item++;
    }
    counted.resize(st_nodes.size());
    for(vector<int>& edge: temporal_edges) {
        counted[mp_node2new[edge[NODE_IN]]] = mp_topo_purn[edge[NODE_IN]];
        counted[mp_node2new[edge[NODE_OUT]]] = mp_topo_purn[edge[NODE_OUT]];
        edge[NODE_IN] = mp_node2new[edge[NODE_IN]];
        edge[NODE_OUT] = mp_node2new[edge[NODE_OUT]];
    }
    return st_nodes.size();
}