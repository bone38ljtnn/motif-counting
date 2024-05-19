#ifndef CONSTRAINT_H
#define CONSTRAINT_H

#include "TemporalGraph.h"
#include <SparseMatrix/Sparse>
using namespace Eigen;

/*
 * TimeAwareTopologyConstraint class provides methods for applying time-aware 
 * constraints on the topology of temporal graphs. These constraints help in 
 * pruning and analyzing the graph based on temporal and topological criteria.
 *
 * Methods:
 * - getTopoOpMatrix: Generates a topological operation matrix based on the edges 
 *                    and a map for topology pruning.
 * - getTimestampOp: Processes the temporal edges to generate timestamp operations,
 *                   including pruning and repeating timestamps, considering 
 *                   parameters such as delta and omega.
 */
class TimeAwareTopologyConstraint{
public:
    int getTopoOpMatrix(vector<vector<int>>& edges,
                     int& node_num,
                     map<int, bool>& mp_topo_purn);

    int getTimestampOp(vector<vector<int>>& edges, 
                   int& edge_num, 
                   int& ts_span, 
                   int& start_ts,
                   int& end_ts,
                   vector<vector<int>>& v_timestamp_purn, 
                   vector<vector<int>>& v_timestamp_purn_repeat,
                   int& delta, 
                   int& omega);
};

#endif