#include <iostream>
#include <fstream>
#include <tuple>
#include <algorithm>
#include <regex>
#include <queue>

using namespace std;

const int directions[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

vector<string> getGardens(const char* filename)
{
    ifstream inputFile(filename);
    vector<string> gardens;
    string line;
    while (getline(inputFile, line))
    {
        gardens.emplace_back(line);
    }
    return gardens;
}

// BFS to find all cells in the region
vector<pair<int, int>> bfs(int startX, int startY, const vector<string>& garden, vector<vector<bool>>& visited) {
    vector<pair<int, int>> region; //a list of all the cells with their coordinates in the same region
    queue<pair<int, int>> q;
    char plantType = garden[startX][startY];

    q.push({startX, startY});
    visited[startX][startY] = true;

    int area = 0;
    int sides = 0;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        region.push_back({x, y});

        for (auto [dx, dy] : directions) { //check 4 neighbors
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < garden.size() && ny >= 0 && ny < garden[0].size() &&
                !visited[nx][ny] && garden[nx][ny] == plantType) { //not out of bound and not checked before and same planttype
                visited[nx][ny] = true;
                q.push({nx, ny});
                }
        }
    }
    return region;
}

// Calculate the perimeter of the region
auto calculatePerimeter(const vector<pair<int, int>>& region, const vector<string>& garden) {
    auto perimeter = 0;
    for (const auto& [x, y] : region) {
        for (auto [dx, dy] : directions) {
            int nx = x + dx, ny = y + dy;
            // Count as perimeter if out of bounds or different plant type
            if (nx < 0 || nx >= garden.size() || ny < 0 || ny >= garden[0].size() || garden[nx][ny] != garden[x][y]) {
                perimeter++;
            }
        }
    }
    return perimeter;
}

// Function to check if a cell is valid
bool isValid(int r, int c, int rows, int cols) {
    return r >= 0 && r < rows && c >= 0 && c < cols;
}

int walkBoundary(const vector<string>& grid, vector<vector<bool>>& visited, int startR, int startC, char type) {
    int rows = grid.size(), cols = grid[0].size();
    int sides = 0;

    // Start from the first edge cell
    int r = startR, c = startC;
    int dir = 0; // Initial direction (North)

    do {

        visited[r][c] = true;

        // Check current direction
        int nr = r + directions[dir][0];
        int nc = c + directions[dir][1];

        if (!isValid(nr, nc, rows, cols) || grid[nr][nc] != type) {
            if (type == 'C')
            {
                cout << "currently at: " << "[" << r << "," << c << "]" << endl;
                cout << "checking direction: " << "row: " << nr << " col: " << nc <<endl;
            }
            // If we hit a boundary, this is a "side"
                sides++;
            if (type == 'C')
            {
                cout << sides << endl;
            }
                //cout << sides << endl;
                //cout << endl;

            // Turn right (change direction clockwise)
            dir = (dir + 1) % 4;
        } else {
            // Move forward in the current direction
            r = nr;
            c = nc;
            if (type == 'C')
            {
                cout << "currently at: " << "[" << r << "," << c << "]" << endl;
            }
            // Turn left if the cell on the left is still part of the boundary
            int leftDir = (dir + 3) % 4;
            int lr = r + directions[leftDir][0];
            int lc = c + directions[leftDir][1];
            if (isValid(lr, lc, rows, cols) && grid[lr][lc] == type) {
                dir = leftDir; // Turn left
                sides++;
                visited[lr][lc] = true;
            }
        }
    } while (r != startR || c != startC || dir != 0); // Stop when back at start

    return sides;
}

// // Function to perform flood fill to identify the entire region
// void floodFill(const vector<string>& grid, vector<vector<bool>>& regionMap, int r, int c, char regionType) {
//     int rows = grid.size(), cols = grid[0].size();
//     regionMap[r][c] = true; // Mark current cell as part of the region
//
//     for (auto dir : directions) {
//         int nr = r + dir[0], nc = c + dir[1];
//         if (isValid(nr, nc, rows, cols) && !regionMap[nr][nc] && grid[nr][nc] == regionType) {
//             floodFill(grid, regionMap, nr, nc, regionType);
//         }
//     }
// }
//
// // Function to find and count the boundary sides of a region
// int countBoundarySides(const vector<string>& grid, vector<vector<bool>>& regionMap, int rows, int cols, char regionType) {
//     int sides = 0;
//
//     // Traverse the entire grid to find boundary cells
//     for (int r = 0; r < rows; r++) {
//         for (int c = 0; c < cols; c++) {
//             if (regionMap[r][c] && grid[r][c] == regionType) { // If this cell is part of the region
//                 // Check its neighbors to identify boundary cells
//                 for (auto dir : directions) {
//                     int nr = r + dir[0], nc = c + dir[1];
//                     if (isValid(nr, nc, rows, cols) && (grid[nr][nc] != regionType || !regionMap[nr][nc])) {
//                         // If neighbor is different region or outside, count this cell as a boundary
//                         sides++;
//                         break; // No need to check further neighbors for this region cell
//                     }
//                 }
//             }
//         }
//     }
//
//     return sides;
// }


int sides(const vector<pair<int,int>>& region) {
    int n = 0;

    // Iterate through each element in the region
    for (const auto& p : region) {
        int x = p.first;
        int y = p.second;

        // Define the shifts for the four directions
        const tuple<int, int, int, int, int, int> shifts[] = {
            //x = row, y = column
            {x + 1, y, x, y - 1, x + 1, y - 1}, // Down, Left-down
            {x - 1, y, x, y - 1, x - 1, y - 1}, // Up, Left-up
            {x, y + 1, x - 1, y, x - 1, y + 1}, // Right, Down-right
            {x, y - 1, x - 1, y, x - 1, y - 1}  // Left, Down-left
        };

        // Check the perimeter condition for each shift
        //If (x1, y1) (an adjacent diagonal cell) is in the region, but (x2, y2) (a further diagonal cell) is not in the region,
        //then the edge is considered "enclosed" and does not contribute to the boundary.
        for (const auto& [nx, ny, x1, y1, x2, y2] : shifts) {
            // Check if (nx, ny) is not in the region using std::find
            if (find(region.begin(), region.end(), make_pair(nx, ny)) == region.end() &&
                !(find(region.begin(), region.end(), make_pair(x1, y1)) != region.end() &&
                  find(region.begin(), region.end(), make_pair(x2, y2)) == region.end())) {
                n++;
                  }
        }
    }
    return n;
}

// Main function to calculate total fencing cost
auto calculateFenceCost(const vector<string>& garden, bool ifSides) {
    int rows = garden.size();
    int cols = garden[0].size();
    vector<vector<bool>> visited(rows, vector<bool>(cols, false)); //2d vector all initialized to false
    auto totalCost = 0;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!visited[i][j]) {
                // Find the region starting from this cell
                auto region = bfs(i, j, garden, visited);
                if (!ifSides)
                {
                    auto area = region.size();
                    auto perimeter = calculatePerimeter(region, garden);
                    totalCost += area * perimeter;
                }
                if (ifSides)
                {
                    // floodFill(garden, visited, i, j, garden[i][j]);
                    vector<vector<bool>> visit(rows, vector<bool>(cols, false));
                    //auto sides = walkBoundary(garden, visit, i, j, garden[i][j]);
                    auto sides2 = sides(region);
                    int area = region.size();
                    cout << "region: " << garden[i][j] << ", area: " << area << ", sides: " << sides << "sides2: " << sides2 << endl;
                    totalCost += area * sides2;
                    cout << "total cost: " << area * sides2 << endl;
                }
                // // After counting, deactivate the region by resetting regionMap for these cells
                // for (int rr = 0; rr < rows; rr++) {
                //     for (int cc = 0; cc < cols; cc++) {
                //         if (visited[rr][cc] && garden[rr][cc] == garden[i][j]) {
                //             visited[rr][cc] = false; // Deactivate the region
                //         }
                //     }
                // }
            }
        }
    }
    return totalCost;
}


int main()
{
    vector<string> garden = getGardens("input_test.txt");
    cout << calculateFenceCost(garden, false) << endl;
    garden = getGardens("input_test.txt");
    cout << calculateFenceCost(garden, true) << endl;
    vector<string> input_garden = getGardens("input.txt");
    cout << calculateFenceCost(input_garden, false) << endl;
    input_garden = getGardens("input.txt");
    cout << calculateFenceCost(input_garden, true) << endl;
    return 0;
}