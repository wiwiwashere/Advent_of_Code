#include <iostream>
#include <regex>
#include <set>
#include <queue>
#include <fstream>
using namespace std;

// Function to read and parse the disk map from a file
vector<vector<int>> getMap(const char* filepath) {
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Error opening: " << filepath << endl;
        throw runtime_error("File not found or could not be opened");
    }

    string line;
    vector<string> maps;
    while (getline(file, line)) {
        maps.push_back(line);
    }

    // Function to parse the input map into a 2D vector
    vector<vector<int>> topomap;
    for (const string& row : maps) {
        vector<int> path;
        for (char ch : row) {
            path.push_back(ch - '0'); // Convert char to int
        }
        topomap.push_back(path);
    }
    return topomap;
}

// Find all trailheads (positions with height 0)
vector<pair<int, int>> findTrailheads(const vector<vector<int>>& topomap) {
    vector<pair<int, int>> trailheads;
    for (int r = 0; r < topomap.size(); ++r) {
        for (int c = 0; c < topomap[0].size(); ++c) {
            if (topomap[r][c] == 0) {
                trailheads.emplace_back(r, c);
            }
        }
    }
    return trailheads;
}

//find all reachable '9's from a trailhead
set<pair<int, int>> all_reachable9(const vector<vector<int>>& topomap, pair<int, int> start) {
    int rows = topomap.size();
    int cols = topomap[0].size();
    set<pair<int, int>> reachableNines; //this set contains the position of 9s that're reachable from the start point
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    queue<pair<int, int>> q;

    q.push(start);

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop(); //remove the position accessing rn from the queue of to_access

        if (visited[r][c]) continue;
        visited[r][c] = true;

        int currentHeight = topomap[r][c];
        if (currentHeight == 9) {
            reachableNines.insert({r, c}); //since it is a set so we dont account for duplicate
        }

        // Check adjacent cells (up, down, left, right)
        vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (auto [dr, dc] : directions) {
            int nr = r + dr, nc = c + dc; //new row, new column
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !visited[nr][nc]) { //if not out of bound and hasnt check this route before
                if (topomap[nr][nc] == currentHeight + 1) { //if it does increment by 1
                    q.push({nr, nc}); //new possibility of a possible reach9 route
                }
            }
        }
    }

    return reachableNines;
}

// Recursive function to find all hiking trails
void findTrails(const vector<vector<int>>& topomap, int r, int c, set<vector<pair<int, int>>>& trails, vector<pair<int, int>> currentTrail) {
    int rows = topomap.size();
    int cols = topomap[0].size();

    currentTrail.emplace_back(r, c);

    if (topomap[r][c] == 9) {
        trails.insert(currentTrail); // Add this completed trail to the set
        return;
    }

    vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    for (auto [dr, dc] : directions) {
        int nr = r + dr, nc = c + dc;
        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
            if (topomap[nr][nc] == topomap[r][c] + 1) {
                findTrails(topomap, nr, nc, trails, currentTrail);
            }
        }
    }
}

// Calculate total scores for all trailheads
int calculateScores(const vector<vector<int>>& topomap, int part) {
    vector<pair<int, int>> trailheads = findTrailheads(topomap);
    int totalScore = 0;

    if (part == 1)
    {
        for (auto trailhead : trailheads) {
            set<pair<int, int>> reachableNines = all_reachable9(topomap, trailhead);
            totalScore += reachableNines.size();
        }
    }
    if (part == 2)
    {
        for (auto trailhead : trailheads) {
            set<vector<pair<int, int>>> trails; // Use a set to ensure unique trails
            vector<pair<int, int>> currentTrail;
            findTrails(topomap, trailhead.first, trailhead.second, trails, currentTrail);
            totalScore += trails.size();
        }
    }

    return totalScore;
}


int main()
{
    vector<vector<int>> map = getMap("input_test.txt");
    cout << calculateScores(map,1) << endl;
    cout << calculateScores(map,2) << endl;
    vector<vector<int>> inputmap = getMap("input.txt");
    cout << calculateScores(inputmap,1) << endl;
    cout << calculateScores(inputmap,2) << endl;
    return 0;
}
