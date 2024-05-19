#include "CountAllMotifs.h"

bool cmp(const vector<int>& se1, const vector<int>& se2) {
    if(se1[0] == se2[0]) {
        return se1[1] < se2[1];
    } else return se1[0] < se2[0];
}

void CountMethod::countTriedNum(vector<vector<int>>& edges,
                    int& node_num,
                    vector<bool>& counted,
                    vector<bool>& purn, 
                    int& delta,
                    int tried[3][2][2][2]) {

    map<int, vector<vector<int>>> mp_starEdges;
    //0 : ts 1: nbr 2:dir
    vector<map<int, vector<int>>> degree_out;

    for(vector<int> edge: edges) {
        mp_starEdges[edge[0]].push_back({edge[2], edge[1], 1});
        mp_starEdges[edge[1]].push_back({edge[2], edge[0], 0});
    }
    int starEdges_num = mp_starEdges.size();
    for(int i = 0; i < starEdges_num; ++i) {
        sort(mp_starEdges[i].begin(), mp_starEdges[i].end(), cmp);
    }

    degree_out.resize(starEdges_num);
    for(int n = 0; n < starEdges_num; ++n) {
        for(vector<int> starEdge: mp_starEdges[n]) {
            if(starEdge[2] == 1) {
                degree_out[n][starEdge[1]].push_back(starEdge[0]);
            }
        }
    }
    for(int i = 0; i < node_num; ++i) {
        vector<vector<int>> starEdges = mp_starEdges[i];
        if(purn[i]) {
            continue;
        }
        int len = starEdges.size();
        for(int j = 0; j < len; ++j) {
            vector<int> se1 = starEdges[j];
            if(counted[se1[1]]) {
                continue;
            }
            for(int k = j + 1; k < len; ++k) {
                vector<int> se2 = starEdges[k];

                if(counted[se2[1]]) {
                    continue;
                }

                if(se2[0] - se1[0] > delta) {
                    break;
                }
                if(se2[1] == se1[1]) {
                    continue;
                }
                if(degree_out[se1[1]].find(se2[1]) != degree_out[se1[1]].end()) {
                    for(int t : degree_out[se1[1]][se2[1]]) {
                        if(t < se1[0] && t >= se2[0] - delta) {
                            ++tried[0][se1[2]][se2[2]][0];
                        } else if(t >= se1[0] && t <= se2[0]) {
                            ++tried[1][se1[2]][se2[2]][0];
                        } else if(t >= se2[0] && t <= se1[0] + delta) {
                            ++tried[2][se1[2]][se2[2]][0];
                        }
                        if(t > se1[0] + delta) {
                            break;
                        }
                    }
                }
                if(degree_out[se2[1]].find(se1[1]) != degree_out[se2[1]].end()) {
                    for(int t : degree_out[se2[1]][se1[1]]) {
                        if(t < se1[0] && t >= se2[0] - delta) {
                            ++tried[0][se1[2]][se2[2]][1];
                        } else if(t >= se1[0] && t <= se2[0]) {
                            ++tried[1][se1[2]][se2[2]][1];
                        } else if(t >= se2[0] && t <= se1[0] + delta) {
                            ++tried[2][se1[2]][se2[2]][1];
                        }
                        if(t > se1[0] + delta) {
                            break;
                        }
                    }
                }
            }
        }
        counted[i] = true;
    }
}
void CountMethod::countStarNum(vector<vector<int>>& edges,
                    int& node_num,
                    int& delta,
                    int star[3][2][2][2],
                    int pair[2][2][2]) {
    
    map<int, vector<vector<int>>> mp_starEdges;

    for(vector<int> edge: edges) {
        mp_starEdges[edge[0]].push_back({edge[2], edge[1], 1});
        mp_starEdges[edge[1]].push_back({edge[2], edge[0], 0});
    }
    int starEdges_num = mp_starEdges.size();
    for(int i = 0; i < starEdges_num; ++i) {
        sort(mp_starEdges[i].begin(), mp_starEdges[i].end(), cmp);
    }

    for(int i = 0; i < node_num; ++i) {
        auto starEdges =  mp_starEdges[i];
        int len = starEdges.size();

        unordered_map<int, int> nbrNum0;
        unordered_map<int, int> nbrNum1;
        for(int i=0;i<len;++i) {
            int start_nbr = starEdges[i][1];
            int start_dir = starEdges[i][2];
            int sum[2] = {0};
            for(int j=i+1;j<len;++j) {
                if(starEdges[j][0]>starEdges[i][0]+delta) break;
                int end_nbr = starEdges[j][1];
                int end_dir = starEdges[j][2];
                if(end_nbr==start_nbr)
                {
                    star[1][start_dir][0][end_dir] += (sum[0]-nbrNum0[end_nbr]);
                    star[1][start_dir][1][end_dir] += (sum[1]-nbrNum1[end_nbr]);
                    pair[start_dir][0][end_dir] += nbrNum0[start_nbr];
                    pair[start_dir][1][end_dir] += nbrNum1[start_nbr];
                } else {
                    star[0][start_dir][0][end_dir] += nbrNum0[end_nbr];
                    star[0][start_dir][1][end_dir] += nbrNum1[end_nbr];
                    star[2][start_dir][0][end_dir] += nbrNum0[start_nbr];
                    star[2][start_dir][1][end_dir] += nbrNum1[start_nbr];
                }
            if(end_dir==0) ++nbrNum0[end_nbr];
            else ++nbrNum1[end_nbr];
            ++sum[end_dir];
            }
            nbrNum0.clear();
            nbrNum1.clear();
        }
    }
    mp_starEdges.clear();
}

void CountMethod::getAllMatrix(bool flag_plus, 
                    int tried[3][2][2][2],
                    int star[3][2][2][2],
                    int pair[2][2][2],
                    int matrix[6][6]) {
    if(flag_plus) {
        matrix[2][5] += tried[2][0][0][0] + tried[0][1][0][1] + tried[1][1][1][1];
        matrix[3][5] += tried[2][0][0][1] + tried[0][1][1][1] + tried[1][1][0][1];
        matrix[0][5] += tried[2][0][1][0] + tried[0][0][0][1] + tried[1][1][1][0];
        matrix[1][5] += tried[2][0][1][1] + tried[0][0][1][1] + tried[1][1][0][0];
        matrix[1][4] += tried[1][0][1][1] + tried[0][1][0][0] + tried[2][1][0][0];
        matrix[3][4] += tried[1][0][0][1] + tried[0][1][1][0] + tried[2][1][0][1];
        matrix[0][4] += tried[1][0][1][0] + tried[0][0][0][0] + tried[2][1][1][0];
        matrix[2][4] += tried[1][0][0][0] + tried[0][0][1][0] + tried[2][1][1][1];

        matrix[5][4] += (pair[0][1][0] + pair[1][0][1]) / 2;
        matrix[4][5] += (pair[0][1][1] + pair[1][0][0]) / 2;
        matrix[4][4] += (pair[0][0][0] + pair[1][1][1]) / 2;
        matrix[5][5] += (pair[0][0][1] + pair[1][1][0]) / 2;

        matrix[3][1] += star[1][0][0][0];
        matrix[3][0] += star[1][0][0][1];
        matrix[2][1] += star[1][0][1][0];
        matrix[2][0] += star[1][0][1][1];
        matrix[3][2] += star[1][1][0][1];
        matrix[3][3] += star[1][1][0][0];
        matrix[2][2] += star[1][1][1][1];
        matrix[2][3] += star[1][1][1][0];
        matrix[1][0] += star[0][0][0][1];
        matrix[1][1] += star[0][0][0][0];
        matrix[1][2] += star[0][0][1][1];
        matrix[1][3] += star[0][0][1][0];
        matrix[0][0] += star[0][1][0][1];
        matrix[0][1] += star[0][1][0][0];
        matrix[0][2] += star[0][1][1][1];
        matrix[0][3] += star[0][1][1][0];
        matrix[4][3] += star[2][1][0][1];
        matrix[5][3] += star[2][1][0][0];
        matrix[4][2] += star[2][1][1][1];
        matrix[5][2] += star[2][1][1][0];
        matrix[4][0] += star[2][0][1][1];
        matrix[5][0] += star[2][0][1][0];
        matrix[4][1] += star[2][0][0][1];
        matrix[5][1] += star[2][0][0][0];
    } else {
        matrix[2][5] -= tried[2][0][0][0] + tried[0][1][0][1] + tried[1][1][1][1];
        matrix[3][5] -= tried[2][0][0][1] + tried[0][1][1][1] + tried[1][1][0][1];
        matrix[0][5] -= tried[2][0][1][0] + tried[0][0][0][1] + tried[1][1][1][0];
        matrix[1][5] -= tried[2][0][1][1] + tried[0][0][1][1] + tried[1][1][0][0];
        matrix[1][4] -= tried[1][0][1][1] + tried[0][1][0][0] + tried[2][1][0][0];
        matrix[3][4] -= tried[1][0][0][1] + tried[0][1][1][0] + tried[2][1][0][1];
        matrix[0][4] -= tried[1][0][1][0] + tried[0][0][0][0] + tried[2][1][1][0];
        matrix[2][4] -= tried[1][0][0][0] + tried[0][0][1][0] + tried[2][1][1][1];

        matrix[5][4] -= (pair[0][1][0] + pair[1][0][1]) / 2;
        matrix[4][5] -= (pair[0][1][1] + pair[1][0][0]) / 2;
        matrix[4][4] -= (pair[0][0][0] + pair[1][1][1]) / 2;
        matrix[5][5] -= (pair[0][0][1] + pair[1][1][0]) / 2;

        matrix[3][1] -= star[1][0][0][0];
        matrix[3][0] -= star[1][0][0][1];
        matrix[2][1] -= star[1][0][1][0];
        matrix[2][0] -= star[1][0][1][1];
        matrix[3][2] -= star[1][1][0][1];
        matrix[3][3] -= star[1][1][0][0];
        matrix[2][2] -= star[1][1][1][1];
        matrix[2][3] -= star[1][1][1][0];
        matrix[1][0] -= star[0][0][0][1];
        matrix[1][1] -= star[0][0][0][0];
        matrix[1][2] -= star[0][0][1][1];
        matrix[1][3] -= star[0][0][1][0];
        matrix[0][0] -= star[0][1][0][1];
        matrix[0][1] -= star[0][1][0][0];
        matrix[0][2] -= star[0][1][1][1];
        matrix[0][3] -= star[0][1][1][0];
        matrix[4][3] -= star[2][1][0][1];
        matrix[5][3] -= star[2][1][0][0];
        matrix[4][2] -= star[2][1][1][1];
        matrix[5][2] -= star[2][1][1][0];
        matrix[4][0] -= star[2][0][1][1];
        matrix[5][0] -= star[2][0][1][0];
        matrix[4][1] -= star[2][0][0][1];
        matrix[5][1] -= star[2][0][0][0];
    }
}

void CountMethod::getStarEdgesMap(map<int, vector<vector<int>>>& mp_starEdges, 
                     vector<vector<int>>& edges) {
    for(vector<int> edge: edges) {
        mp_starEdges[edge[0]].push_back({edge[2], edge[1], 1});
        mp_starEdges[edge[1]].push_back({edge[2], edge[0], 0});
    }
    int starEdges_num = mp_starEdges.size();
    for(int i = 0; i < starEdges_num; ++i) {
        sort(mp_starEdges[i].begin(), mp_starEdges[i].end(), cmp);
    }
}

void CountMethod::getDegreeOut(vector<map<int, vector<int>>>& degree_out,
                   map<int, vector<vector<int>>>& mp_starEdges) {
    
    int starEdges_num = mp_starEdges.size();
    degree_out.resize(starEdges_num);
    for(int n = 0; n < starEdges_num; ++n) {
        for(vector<int> starEdge: mp_starEdges[n]) {
            if(starEdge[2] == 1) {
                degree_out[n][starEdge[1]].push_back(starEdge[0]);
            }
        }
    }
}

void CountMethod::countTriedNumWithNode(vector<vector<int>>& starEdges,
                           vector<bool>& counted,
                           vector<bool>& purn,
                           int& delta,
                           int tried[3][2][2][2],
                           vector<map<int, vector<int>>>& degree_out) {
        int len = starEdges.size();
        for(int j = 0; j < len; ++j) {
            vector<int> se1 = starEdges[j];
            if(counted[se1[1]]) {
                continue;
            }
            for(int k = j + 1; k < len; ++k) {
                vector<int> se2 = starEdges[k];

                if(counted[se2[1]]) {
                    continue;
                }

                if(se2[0] - se1[0] > delta) {
                    break;
                }
                if(se2[1] == se1[1]) {
                    continue;
                }
                if(degree_out[se1[1]].find(se2[1]) != degree_out[se1[1]].end()) {
                    for(int t : degree_out[se1[1]][se2[1]]) {
                        if(t < se1[0] && t >= se2[0] - delta) {
                            ++tried[0][se1[2]][se2[2]][0];
                        } else if(t >= se1[0] && t <= se2[0]) {
                            ++tried[1][se1[2]][se2[2]][0];
                        } else if(t >= se2[0] && t <= se1[0] + delta) {
                            ++tried[2][se1[2]][se2[2]][0];
                        }
                        if(t > se1[0] + delta) {
                            break;
                        }
                    }
                }
                if(degree_out[se2[1]].find(se1[1]) != degree_out[se2[1]].end()) {
                    for(int t : degree_out[se2[1]][se1[1]]) {
                        if(t < se1[0] && t >= se2[0] - delta) {
                            ++tried[0][se1[2]][se2[2]][1];
                        } else if(t >= se1[0] && t <= se2[0]) {
                            ++tried[1][se1[2]][se2[2]][1];
                        } else if(t >= se2[0] && t <= se1[0] + delta) {
                            ++tried[2][se1[2]][se2[2]][1];
                        }
                        if(t > se1[0] + delta) {
                            break;
                        }
                    }
                }
            }
        }
}



void CountMethod::countStarNumWithNode(vector<vector<int>>& starEdges,
                    int& node_num,
                    int& delta,
                    int star[3][2][2][2],
                    int pair[2][2][2]) {
        int len = starEdges.size();

        unordered_map<int, int> nbrNum0;
        unordered_map<int, int> nbrNum1;
        for(int i=0;i<len;++i) {
            int start_nbr = starEdges[i][1];
            int start_dir = starEdges[i][2];
            int sum[2] = {0};
            for(int j=i+1;j<len;++j) {
                if(starEdges[j][0]>starEdges[i][0]+delta) break;
                int end_nbr = starEdges[j][1];
                int end_dir = starEdges[j][2];
                if(end_nbr==start_nbr)
                {
                    star[1][start_dir][0][end_dir] += (sum[0]-nbrNum0[end_nbr]);
                    star[1][start_dir][1][end_dir] += (sum[1]-nbrNum1[end_nbr]);
                    pair[start_dir][0][end_dir] += nbrNum0[start_nbr];
                    pair[start_dir][1][end_dir] += nbrNum1[start_nbr];
                } else {
                    star[0][start_dir][0][end_dir] += nbrNum0[end_nbr];
                    star[0][start_dir][1][end_dir] += nbrNum1[end_nbr];
                    star[2][start_dir][0][end_dir] += nbrNum0[start_nbr];
                    star[2][start_dir][1][end_dir] += nbrNum1[start_nbr];
                }
            if(end_dir==0) ++nbrNum0[end_nbr];
            else ++nbrNum1[end_nbr];
            ++sum[end_dir];
            }
            nbrNum0.clear();
            nbrNum1.clear();
        }
}

