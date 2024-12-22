#include <iostream>
#include <regex>
#include <fstream>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <set>
using namespace std;
//
//

// // Directions and their clockwise/counter-clockwise rotations
// enum Direction { EAST, SOUTH, WEST, NORTH };
// //x=column y=row
// const int dx[] = {1, 0, -1, 0}; // Move East, South, West, North
// const int dy[] = {0, 1, 0, -1};
// const int turnCost = 1000;
//  // Directions: North, East, South, West
const vector<pair<int, int>> DIRECTIONS = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}}; //down,right,up,left
const vector<char> DIRECTION_NAMES = {'N', 'E', 'S', 'W'};

void printGrid(vector<string> grid)
{
    cout << endl;
    for (int i = 0; i < grid.size(); i++)
    {
        cout << grid[i] << endl;
    }
    cout << endl;
}

vector<string> getMap(const char* filename)
{
    vector<string> map;
    ifstream inputFile(filename);
    string line;
    while (getline(inputFile, line)) {map.emplace_back(line);}
    return map;
}

struct State {
    int x, y, dir, score; //current position, current direction, accumulated score

    bool operator>(const State& other) const {
        return score > other.score;
    }
    bool operator==(const State& other) const
    {
        return (x==other.x && y == other.y && dir == other.dir && score == other.score);
    }
};

string encode_state(int x, int y, int dir) {
    return to_string(x) + "," + to_string(y) + "," + to_string(dir);
};

int find_lowest_score(const vector<string>& maze, vector<string>& free_to_edit) {
    int rows = maze.size();
    int cols = maze[0].size();
    int start_x, start_y, end_x, end_y;

    // Find the start ('S') and end ('E') positions; x=row, y=column
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (maze[i][j] == 'S') {
                start_x = i;
                start_y = j;
            } else if (maze[i][j] == 'E') {
                end_x = i;
                end_y = j;
            }
        }
    }

    // Priority queue for BFS (min-heap based on score)
    priority_queue<State, vector<State>, greater<State>> pq; //ensures that the state with the lowest score is processed first.
    pq.push({start_x, start_y, 1, 0}); //initialize bfs queue, facing East
    vector<State> open;

    //stored visited state as x,y,dir
    unordered_set<string> visited;

    visited.insert(encode_state(start_x, start_y, 1));

    unordered_map<string, vector<State>> parent;
    parent[encode_state(start_x, start_y, 1)].push_back({start_x, start_y, 1, 0}); // Start state has no parent
    vector<int> scores;
    State current;
    bool is_found = false;
    int score, last_x, last_y, last_dir;

    while (!pq.empty()) {
        current = pq.top();
        open.emplace_back(current);
        pq.pop();

        // Check if we reached the end
        if (current.x == end_x && current.y == end_y) {
            if (!is_found)
            {
                score = current.score;
                last_x = current.x;
                last_y = current.y;
                last_dir = current.dir;
                is_found = true;
            }
            scores.emplace_back(current.score);
            continue; // End of BFS; move to backtracking
        }

        // Move forward
        int nx = current.x + DIRECTIONS[current.dir].first;
        int ny = current.y + DIRECTIONS[current.dir].second;
        if (nx >= 0 && ny >= 0 && nx < rows && ny < cols && maze[nx][ny] != '#') {
            string next_state = encode_state(nx, ny, current.dir);
            if (visited.find(next_state) == visited.end()) {
                visited.insert(next_state);
                parent[next_state].push_back(current); // Record parent
                pq.push({nx, ny, current.dir, current.score + 1});
            }
        }

        // Rotate clockwise and counterclockwise
        for (int rotation = -1; rotation <= 1; rotation += 2) {
            int new_dir = (current.dir + rotation + 4) % 4;
            string next_state = encode_state(current.x, current.y, new_dir);
            if (visited.find(next_state) == visited.end()) {
                visited.insert(next_state);
                parent[next_state].push_back(current); // Record parent
                pq.push({current.x, current.y, new_dir, current.score + 1000});
            }
        }
    }

    for (auto key : parent)
    {
        if (key.second.size() > 1)
        {
            cout << "here" << endl;
        }
    }

    // Mark the end point
    free_to_edit[last_x][last_y] = 'O';
    bool mul = false;

    while (score >= 0) {
        string current_state = encode_state(last_x, last_y, last_dir);
        int last_score = score;
        if (parent[current_state].size() > 1)
        {
            mul = true;
            cout << "here" << endl;
        }
        for (auto state: parent[current_state])
        {
            State parent_state = state; // Retrieve parent state

            // Update path
            if (free_to_edit[parent_state.x][parent_state.y] != 'O')
            {
                free_to_edit[parent_state.x][parent_state.y] = 'O';
            }
            last_x = parent_state.x;
            last_y = parent_state.y;
            last_dir = parent_state.dir;

            // Adjust score
            if (parent_state.dir == last_dir) {
                score -= 1; // Forward movement
            } else {
                score -= 1000; // Rotation
            }
        }
        if (mul)
        {
            if (score == 0)
            {
                score = last_score;
                mul = false;
            }
        }


        //printGrid(free_to_edit); // Optional: Debugging output
    }

    return scores[0];

    // while (!pq.empty())
    // {
    //     State current = pq.top();
    //     open.emplace_back(current);
    //     pq.pop();
    //
    //     // Check if we reached the end
    //     if (current.x == end_x && current.y == end_y)
    //     {
    //         int score = current.score;
    //         int last_x = current.x;
    //         int last_y = current.y;
    //         // int last_x = current.x - DIRECTIONS[current.dir].first;
    //         // int last_y = current.y - DIRECTIONS[current.dir].second;
    //
    //         while (score >= 0)
    //         {
    //             for (int i = 0; i < DIRECTIONS.size(); i++)
    //             {
    //                 int subtract = 1;
    //                 if (i != current.dir)
    //                 {
    //                     subtract = 1000;
    //                 }
    //                 cout << "before" << endl;
    //                 cout << score-subtract << endl;
    //                 cout << last_x-DIRECTIONS[i].first << " " << last_y-DIRECTIONS[i].second << endl;
    //                 auto it = find(open.begin(), open.end(), State(last_x-DIRECTIONS[i].first, last_y-DIRECTIONS[i].second, i, score-subtract));
    //                 cout << "after" << endl;
    //                 if (it != open.end())
    //                 {
    //                     cout << "here" << endl;
    //                     last_x = (last_x) - DIRECTIONS[it->dir].first;
    //                     last_y = (last_y) - DIRECTIONS[it->dir].second;
    //                     free_to_edit[last_x][last_y] = 'O';
    //                     printGrid(free_to_edit);
    //                     score-=subtract;
    //                     break;
    //                 }
    //             }
    //         }
    //         return current.score;
    //     }
    //
    //         // Move forward
    //         int nx = current.x + DIRECTIONS[current.dir].first;
    //         int ny = current.y + DIRECTIONS[current.dir].second;
    //         if (nx >= 0 && ny >= 0 && nx < rows && ny < cols && maze[nx][ny] != '#') {
    //             string next_state = encode_state(nx, ny, current.dir);
    //             if (visited.find(next_state) == visited.end()) {
    //                 visited.insert(next_state);
    //                 pq.push({nx, ny, current.dir, current.score + 1});
    //             }
    //         }
    //
    //         // Rotate clockwise and counterclockwise
    //         for (int rotation = -1; rotation <= 1; rotation += 2) {
    //             int new_dir = (current.dir + rotation + 4) % 4; // Ensure valid direction index
    //             string next_state = encode_state(current.x, current.y, new_dir);
    //             if (visited.find(next_state) == visited.end()) {
    //                 visited.insert(next_state);
    //                 pq.push({current.x, current.y, new_dir, current.score + 1000});
    //             }
    //         }
    // }

    return -1; // Should never reach here
}

int main() {
    vector<string> example = getMap("input_test.txt");
    vector<string> after_path = example;

    cout << "Lowest Score: " << find_lowest_score(example, after_path) << endl;

    printGrid(after_path);

    int count = 0;
    for (auto str: after_path)
    {
        for (auto letter: str)
        {
            if (letter == 'O')
            {
                count++;
            }
        }
    }
    cout << count << endl;
    //cout << "Best Tiles: " << find_best_path_tiles(example) << endl;

    // vector<string> map = getMap("input.txt");
    // vector<string> path = map;
    //
    // cout << "Lowest Score: " << find_lowest_score(map, path) << endl;

    return 0;
}