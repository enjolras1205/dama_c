#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>
#include "dfs.h"

using namespace std;

using Board = vector<vector<int>>;

// 默认方向变更的数组
vector<pair<int, int>> pairwise() {
    return {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
}

vector<vector<pair<int, int>>> explore_direction(Board &chess, int i, int j, int qx, int qy, bool lady, const string &color, int n, int m, vector<vector<pair<int, int>>> &best_paths, int nx, int ny);

vector<vector<pair<int, int>>> explore_capture(Board &chess, int i, int j, int qx, int qy, bool lady, const string &color, int n, int m, vector<vector<pair<int, int>>> &best_paths, int nx, int ny, int rx, int ry) {
    if (rx >= 0 && ry >= 0 && rx < n && ry < m && chess[rx][ry] == 0) {
        int temp = chess[nx][ny];
        chess[nx][ny] = 0; // Remove opponent's piece
        chess[rx][ry] = chess[i][j]; // Move own piece
        chess[i][j] = 0; // Clear current position

        vector<vector<pair<int, int>>> current_paths = dfs(chess, rx, ry, lady, color, n, m, qx, qy);

        // Restore state
        chess[i][j] = chess[rx][ry];
        chess[rx][ry] = 0;
        chess[nx][ny] = temp;

        for (const auto &path : current_paths) {
            best_paths.push_back({{i, j}});
            best_paths.back().insert(best_paths.back().end(), path.begin(), path.end());
        }

        // Continue capturing in the same direction
        return explore_capture(chess, i, j, qx, qy, lady, color, n, m, best_paths, nx, ny, rx + qx, ry + qy);
    }
    return best_paths;
}

vector<vector<pair<int, int>>> explore_direction(Board &chess, int i, int j, int qx, int qy, bool lady, const string &color, int n, int m, vector<vector<pair<int, int>>> &best_paths, int nx, int ny) {
    if (nx >= 0 && ny >= 0 && nx < n && ny < m) {
        if ((color == "white" && (chess[nx][ny] == 3 || chess[nx][ny] == 4)) ||
            (color == "black" && (chess[nx][ny] == 1 || chess[nx][ny] == 2))) {
            return explore_capture(chess, i, j, qx, qy, lady, color, n, m, best_paths, nx, ny, nx + qx, ny + qy);
        } else if (chess[nx][ny] == 0) {
            // Continue moving in this direction
            return explore_direction(chess, i, j, qx, qy, lady, color, n, m, best_paths, nx + qx, ny + qy);
        }
    }
    return best_paths;
}

vector<vector<pair<int, int>>> dfs(Board &chess, int i, int j, bool lady, const string &color, int n, int m, int pre_x, int pre_y) {
    vector<vector<pair<int, int>>> best_paths;

    if (lady) {
        for (const auto &[qx, qy] : pairwise()) {
            if (qx * -1 == pre_x && qy * -1 == pre_y) {
                continue;
            } else {
                int nx = i + qx;
                int ny = j + qy;
                best_paths = explore_direction(chess, i, j, qx, qy, lady, color, n, m, best_paths, nx, ny);
            }
        }
    } else {
        for (const auto &[qx, qy] : pairwise()) {
            if ((color == "white" && qx == -1) || (color == "black" && qx == 1)) {
                continue;
            }
            int nx = i + qx;
            int ny = j + qy;
            if (nx >= 0 && ny >= 0 && nx < n && ny < m && 
                (((chess[nx][ny] == 3 || chess[nx][ny] == 4) && chess[i][j] == 1) || 
                 ((chess[nx][ny] == 1 || chess[nx][ny] == 2) && chess[i][j] == 3))) {
                
                int rx = nx + qx;
                int ry = ny + qy;

                if (rx >= 0 && ry >= 0 && rx < n && ry < m && chess[rx][ry] == 0) {
                    int temp = chess[nx][ny];
                    chess[nx][ny] = 0;
                    chess[rx][ry] = chess[i][j];
                    chess[i][j] = 0;

                    vector<vector<pair<int, int>>> current_paths = dfs(chess, rx, ry, lady, color, n, m, qx, qy);

                    // Restore state
                    chess[i][j] = chess[rx][ry];
                    chess[nx][ny] = temp;
                    chess[rx][ry] = 0;

                    for (const auto &path : current_paths) {
                        best_paths.push_back({{i, j}});
                        best_paths.back().insert(best_paths.back().end(), path.begin(), path.end());
                    }
                }
            }
        }
    }

    if (best_paths.empty()) {
        best_paths.push_back({{i, j}});
    }

    return best_paths;
}

vector<vector<pair<int, int>>> move_list(Board &turkish_flag, bool white) {
    int n = turkish_flag.size();
    int m = turkish_flag[0].size();
    vector<vector<pair<int, int>>> best_paths_overall;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (white) {
                if (turkish_flag[i][j] == 1 || turkish_flag[i][j] == 2) {
                    vector<vector<pair<int, int>>> paths;
                    if (turkish_flag[i][j] == 1) {
                        paths = dfs(turkish_flag, i, j, false, "white", n, m, 0, 0);
                    } else {
                        paths = dfs(turkish_flag, i, j, true, "white", n, m, 0, 0);
                    }

                    if (paths != vector<vector<pair<int, int>>>(1, vector<pair<int, int>>(1, {i, j}))) {
                        for (const auto &path : paths) {
                            if (best_paths_overall.empty() || path.size() > best_paths_overall[0].size()) {
                                best_paths_overall = {path};
                            } else if (path.size() == best_paths_overall[0].size()) {
                                best_paths_overall.push_back(path);
                            }
                        }
                    }
                }
            } else {
                if (turkish_flag[i][j] == 3 || turkish_flag[i][j] == 4) {
                    vector<vector<pair<int, int>>> paths;
                    if (turkish_flag[i][j] == 3) {
                        paths = dfs(turkish_flag, i, j, false, "black", n, m, 0, 0);
                    } else {
                        paths = dfs(turkish_flag, i, j, true, "black", n, m, 0, 0);
                    }

                    if (paths != vector<vector<pair<int, int>>>(1, vector<pair<int, int>>(1, {i, j}))) {
                        for (const auto &path : paths) {
                            if (best_paths_overall.empty() || path.size() > best_paths_overall[0].size()) {
                                best_paths_overall = {path};
                            } else if (path.size() == best_paths_overall[0].size()) {
                                best_paths_overall.push_back(path);
                            }
                        }
                    }
                }
            }
        }
    }

    return best_paths_overall;
}

