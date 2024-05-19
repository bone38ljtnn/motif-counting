#ifndef COUNT_ALL_MOTIFS_H
#define COUNT_ALL_MOTIFS_H

#include "TemporalGraph.h"
#include <algorithm>
#include <unordered_map>

/*
 * CountMethod class provides methods for counting specific motifs within temporal graphs.
 * Motifs are recurring, significant subgraph patterns, and this class offers functions 
 * to count different types of motifs such as tried numbers and star numbers. 
 * The class also includes methods for generating matrices from these counts and managing 
 * edges and degrees within the graph.
 *
 * Methods:
 * - countTriedNum: Counts the number of tried motifs in the temporal edges.
 * - countStarNum: Counts the number of star motifs in the temporal edges.
 * - getAllMatrix: Generates a matrix representation of the counted motifs.
 * - getStarEdgesMap: Maps star edges from the temporal edges.
 * - getDegreeOut: Computes the out-degree for each node from the star edges map.
 * - countTriedNumWithNode: Counts tried motifs with node information.
 * - countStarNumWithNode: Counts star motifs with node information.
 */
class CountMethod {
public:
    void countTriedNum(vector<vector<int>>& edges,
                    int& node_num,
                    vector<bool>& counted,
                    vector<bool>& purn, 
                    int& delta,
                    int tried[3][2][2][2]);

    void countStarNum(vector<vector<int>>& edges,
                    int& node_num,
                    int& delta,
                    int star[3][2][2][2],
                    int pair[2][2][2]);

    void getAllMatrix(bool flag_plus, 
                    int tried[3][2][2][2],
                    int star[3][2][2][2],
                    int pair[2][2][2],
                    int matrix[6][6]);

    void getStarEdgesMap(map<int, vector<vector<int>>>& mp_starEdges, 
                     vector<vector<int>>& edges);
    void getDegreeOut(vector<map<int, vector<int>>>& degree_out,
                   map<int, vector<vector<int>>>& mp_starEdges);

    void countTriedNumWithNode(vector<vector<int>>& starEdges,
                           vector<bool>& counted,
                           vector<bool>& purn,
                           int& delta,
                           int tried[3][2][2][2],
                           vector<map<int, vector<int>>>& degree_out);

    void countStarNumWithNode(vector<vector<int>>& starEdges,
                    int& node_num,
                    int& delta,
                    int star[3][2][2][2],
                    int pair[2][2][2]);
};
#endif