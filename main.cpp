#include <omp.h>
#include <chrono>
#include "TemporalGraph.h"
#include "Constraint.h"
#include "CountAllMotifs.h"

int main(int argc, char** argv) {
    cout << "****************************************************" << endl; 
    cout << "           graphs load" << endl;
    cout << "****************************************************" << endl << endl;

    auto time_load_s = chrono::high_resolution_clock::now();
    int delta = stoi(argv[4]);
    int omega = stoi(argv[5]) * delta;
    int threads = stoi(argv[3]);
    int threads_flag = (stoi(argv[2]) == 1);
    if(!threads_flag) {
        threads = 1;
    }
    CountMethod count_method;
    TemporalGraphMethod temporal_graph_method;
    TimeAwareTopologyConstraint time_aware_topology_constraint;

    vector<vector<int>> edges;
    bool num_flag = false;
    int node_num = temporal_graph_method.loadFile(argv[1], edges, num_flag);
    
    cout << "[SHOWLOG] Temporal edges loaded." << endl;
    int edge_num = edges.size();

    cout << "[SHOWVAR] node num : " << node_num << endl;
    cout << "[SHOWVAR] edge num : " << edge_num << endl;

    map<int, bool> mp_topo_purn;
    vector<vector<int>> v_timestamp_purn, v_timestamp_purn_repeat;
    int time_span = 0;
    int win_num = 0;

    if(!num_flag) {
        cout << "[SHOWLOG] topology matrix loading..." << endl;
        int cut_num = time_aware_topology_constraint.getTopoOpMatrix(edges, node_num, mp_topo_purn);
        cout << "[SHOWLOG] topology matrix loaded." << endl;
        ++cut_num;

        time_span = edges[edge_num - 1][2] - edges[0][2];
        cout << "[SHOWVAR] time span : " << time_span << endl;

        win_num = time_aware_topology_constraint.getTimestampOp(edges, edge_num, time_span, edges[0][2], edges[edge_num - 1][2], v_timestamp_purn, v_timestamp_purn_repeat, delta, omega);
        cout << "[SHOWLOG] time window loaded." << endl;
        cout << "[SHOWVAR] win num : " << win_num << endl;
    }

    auto time_load_e = chrono::high_resolution_clock::now();
    cout << "[SHOWTMP] load time : " << chrono::duration<double, std::milli>(time_load_e - time_load_s).count() << " (ms)" << endl;

    cout << "loaded. " << endl << endl;

    cout << "****************************************************" << endl; 
    cout << "           MoTTo counting" << endl;
    cout << "****************************************************" << endl << endl;
    

    int matrix[6][6] = {0};
    auto time_c_s = chrono::high_resolution_clock::now();

    if(num_flag) {
        map<int, vector<vector<int>>> mp_starEdges;
        vector<map<int, vector<int>>> degree_out;
        count_method.getStarEdgesMap(mp_starEdges, edges);
        count_method.getDegreeOut(degree_out, mp_starEdges);

        vector<bool> counted(node_num, false);
        int tried[3][2][2][2] = {0};
        int star[3][2][2][2] = {0};
        int pair[2][2][2] = {0};
        for(int node = 0; node < node_num; ++node) {
            count_method.countTriedNumWithNode(mp_starEdges[node], counted, counted, delta, tried, degree_out);
            counted[node] = true;
        }

        for(int node = 0; node < node_num; ++node) {
            count_method.countStarNumWithNode(mp_starEdges[node], node_num, delta, star, pair);
        }

        count_method.getAllMatrix(true, tried, star, pair, matrix);   
    } else if(threads == 1){
        map<int, vector<vector<int>>> mp_starEdges;
        vector<map<int, vector<int>>> degree_out;
        count_method.getStarEdgesMap(mp_starEdges, edges);
        count_method.getDegreeOut(degree_out, mp_starEdges);

        vector<bool> counted(node_num);
        for(int node = 0; node < node_num; ++node) {
            counted[node] = mp_topo_purn[node];
        }
        int tried[3][2][2][2] = {0};
        int star[3][2][2][2] = {0};
        int pair[2][2][2] = {0};
        for(int node = 0; node < node_num; ++node) {
            if(mp_topo_purn[node]) {
                continue;
            }
            count_method.countTriedNumWithNode(mp_starEdges[node], counted, counted, delta, tried, degree_out);
            counted[node] = true;
        }
        for(int node = 0; node < node_num; ++node) {
            count_method.countStarNumWithNode(mp_starEdges[node], node_num, delta, star, pair);
        }

        count_method.getAllMatrix(true, tried, star, pair, matrix);
    } else {
        #pragma omp parallel for num_threads(threads)
        for(int epoch = 0; epoch < win_num; ++epoch) {
            int start_edge_item = v_timestamp_purn[epoch][0];
            int end_edge_item = v_timestamp_purn[epoch][1];

            vector<vector<int>> sub_edges(edges.begin() + start_edge_item, edges.begin() + end_edge_item + 1);
            vector<bool> counted;
            int sub_node_num = temporal_graph_method.changeSubNodeItem(sub_edges, mp_topo_purn, counted);
            vector<bool> c2(counted);


            int tried[3][2][2][2] = {0};
            count_method.countTriedNum(sub_edges, sub_node_num, counted, c2, delta, tried);

        
            int star[3][2][2][2] = {0};
            int pair[2][2][2] = {0};
            count_method.countStarNum(sub_edges, sub_node_num, delta, star, pair);
            count_method.getAllMatrix(true, tried, star, pair, matrix);
        }

        int win_repeat_num = v_timestamp_purn_repeat.size();
        #pragma omp parallel for num_threads(threads)
        for(int epoch = 0; epoch < win_repeat_num; ++epoch) {
            int start_edge_item = v_timestamp_purn_repeat[epoch][0];
            int end_edge_item = v_timestamp_purn_repeat[epoch][1];

            vector<vector<int>> sub_edges(edges.begin() + start_edge_item, edges.begin() + end_edge_item + 1);
            vector<bool> counted;
            int sub_node_num = temporal_graph_method.changeSubNodeItem(sub_edges, mp_topo_purn, counted);
            vector<bool> c2(counted);

            int tried[3][2][2][2] = {0};
            count_method.countTriedNum(sub_edges, sub_node_num, counted, c2, delta, tried);

            int star[3][2][2][2] = {0};
            int pair[2][2][2] = {0};
            count_method.countStarNum(sub_edges, sub_node_num, delta, star, pair);
        
            count_method.getAllMatrix(false, tried, star, pair, matrix);
        }
    }
    
    cout << "[SHOWALL] all num : " << endl;
    for(int i = 0; i < 6; ++i) {
        for(int j = 0; j < 6; ++j) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }

    auto time_c_e = chrono::high_resolution_clock::now();
    cout << "[SHOWTMP] count time : " << chrono::duration<double, std::milli>(time_c_e - time_c_s).count() << " (ms)" << endl;
    return 0;
}