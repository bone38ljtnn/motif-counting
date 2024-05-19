#include "Constraint.h"

int TimeAwareTopologyConstraint::getTopoOpMatrix(vector<vector<int>>& edges,
                     int& node_num,
                     map<int, bool>& mp_topo_purn) {
    int purn_num = 0;
    map<vector<int>, int64_t> mp_equivalent_adjacency_matrix;
    for(auto edge: edges) {
        ++mp_equivalent_adjacency_matrix[{edge[NODE_IN], edge[NODE_OUT]}];
        ++mp_equivalent_adjacency_matrix[{edge[NODE_OUT], edge[NODE_IN]}];
    }
    vector<Triplet<int>> v_smtri;
    for(auto it = mp_equivalent_adjacency_matrix.begin(); it != mp_equivalent_adjacency_matrix.end(); ++it) {
            v_smtri.push_back(Triplet<int>(it->first[0], it->first[1], it->second));
    }

    SparseMatrix<int> equivalent_adjacency_matrix;
    equivalent_adjacency_matrix.resize(node_num, node_num);
    equivalent_adjacency_matrix.setFromTriplets(v_smtri.begin(), v_smtri.end());
    equivalent_adjacency_matrix.makeCompressed();
    SparseMatrix<int> equivalent_adjacency_matrix2 = equivalent_adjacency_matrix * equivalent_adjacency_matrix;
    equivalent_adjacency_matrix2.makeCompressed();

    SparseMatrix<int> N_equivalent_adjacency_matrix = equivalent_adjacency_matrix.cwiseProduct(equivalent_adjacency_matrix2);
    equivalent_adjacency_matrix.makeCompressed();
    
    for(int node = 0; node < node_num; ++node) {
        bool flag = true;
        for(SparseMatrix<int>::InnerIterator it(N_equivalent_adjacency_matrix, node); it; ++it) {
            if(it.value() != 0) {
                flag = false;
                break;
            }
        }
        if(flag) {
            ++purn_num;
        }
        mp_topo_purn[node] = flag;
    }

    return purn_num;
}

int TimeAwareTopologyConstraint::getTimestampOp(vector<vector<int>>& edges, 
                   int& edge_num, 
                   int& ts_span, 
                   int& start_ts,
                   int& end_ts,
                   vector<vector<int>>& v_timestamp_purn, 
                   vector<vector<int>>& v_timestamp_purn_repeat,
                   int& delta, 
                   int& omega) {
    
    if(ts_span <= 0) {
        cout << "timestamp not sort" << endl;
        return -1;
    }

    int low = 0, fast = 0;
    int now_window = start_ts + omega;
    
    for(int item = 0; item < edge_num; ++item) {
        if(edges[item][2] >= (now_window - delta)) {
            fast = item;
            while(item < edge_num - 1 && edges[item][2] < now_window) {
                ++item;
            }
            if(item == edge_num - 1) {
                break;
            }
            if(item - low >= 3) {
                v_timestamp_purn.push_back({low, item});
            } 
            if(item - fast >= 3) {
                v_timestamp_purn_repeat.push_back({fast, item});
            }
            low = fast;
            if(now_window + omega >= end_ts) {
                break;
            } else {
                now_window += omega;
            }
        }
    }
    
    if(edge_num - 1 - low >= 3) {
        v_timestamp_purn.push_back({low, edge_num - 1});
    }
    
    return v_timestamp_purn.size(); //window_num
}