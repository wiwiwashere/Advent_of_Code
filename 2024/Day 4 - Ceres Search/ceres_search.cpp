#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype> // For std::isdigit
#include <algorithm> // For std::all_of
#include <regex>
#include <string>
using namespace std;

vector<pair<int, int>> directions = {
    {0, 1},   // Horizontal right
    {0, -1},  // Horizontal left
    {1, 0},   // Vertical down
    {-1, 0},  // Vertical up
    {1, 1},   // Diagonal down-right
    {-1, -1}, // Diagonal up-left
    {1, -1},  // Diagonal down-left
    {-1, 1}   // Diagonal up-right

    //x is row and y is column
};

vector<pair<int, int>> directions_diagonally = {
    {1, 1},   // Diagonal down-right
    {-1, -1}, // Diagonal up-left
    {1, -1},  // Diagonal down-left
    {-1, 1}   // Diagonal up-right

    //x is row and y is column
};

vector<string> get2dgrid(const char* filepath)
{
    vector<string> res;
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Error opening: " << filepath << endl;
    }

    string line;
    // Read and parse the file

    string str;

    while (getline(file, line)) {
        std::stringstream ss(line);
        int i = 0;
        while (ss >> str) {
            res.push_back(str);
        }
    }
    return res;
}

string check = "XMAS";
string check2 = "MAS";

bool check_str(const vector<string>& grid, int x, int y, int dx, int dy, const string& word) {
    //x & y represent the coordinate of the current positon
    //dx & dy represent the change in direction
    //word is the word to search for
    //this is checking the case that x,y is the start of the word
    int rows = grid.size();
    int cols = grid[0].size();
    int word_len = word.length();

    for (int i = 0; i < word_len; ++i) {
        int nx = x + i * dx; //new x coordinates
        int ny = y + i * dy; //new y coordinates

        // Check bounds and character match
        if (nx < 0 || nx >= rows || ny < 0 || ny >= cols || grid[nx][ny] != word[i]) {
            return false;
        }
    }
    //cout << "at: " << x << ", " << y << endl;
    //cout << "direction: " << dx  << ", " << dy << endl;
    return true;
}

int countXMAS(const char* filepath)
{
    vector<string> matrix = get2dgrid(filepath);
    int count = 0;
    for (int x = 0; x < matrix.size(); ++x) {
        for (int y = 0; y < matrix[0].size(); ++y) {
            for (const auto& dir : directions) {
                int dx = dir.first;
                int dy = dir.second;
                if (check_str(matrix, x, y, dx, dy, check)) {
                    count++;
                }
            }
        }
    }
    return count;
}

bool check_x_shape(const vector<string>& grid, int x, int y) {
    //x & y represent the coordinate of the current positon
    //dx & dy represent the change in direction
    //word is the word to search for
    //this is checking the case that x,y is the start of the word
    int rows = grid.size();
    int cols = grid[0].size();

    // Check bounds for diagonals
    if (x - 1 < 0 || x + 1 >= rows || y - 1 < 0 || y + 1 >= cols) return false;

    // Define diagonals
    char topLeft = grid[x - 1][y - 1];
    char topRight = grid[x - 1][y + 1];
    char center = grid[x][y];
    char bottomLeft = grid[x + 1][y - 1];
    char bottomRight = grid[x + 1][y + 1];

    // Check for valid "X-MAS" pattern
    return center == 'A' &&
           ((topLeft == 'M' && bottomRight == 'S') || (topLeft == 'S' && bottomRight == 'M')) && ((bottomLeft == 'M' && topRight == 'S') || (bottomLeft == 'S' && topRight == 'M'));   // Backward MAS

    /*
    vector<pair<int, int>> direction_to_check = directions_diagonally;

    bool left_side = false;
    bool right_side = false;

    for (int i = 0; i < directions_diagonally.size(); i++) {
        int dx = directions_diagonally[i].first;
        int dy = directions_diagonally[i].second;

        // Compute the starting coordinates for the current diagonal
        int start_x = x + dx;
        int start_y = y + dy;

        // Check if the starting position is in bounds
        if (start_x >= 0 && start_x < rows && start_y >= 0 && start_y < cols) {
            if (check_str(grid, start_x, start_y, dx, dy, check2)) {
                if (i < 2) left_side = true;  // top-left or bottom-right
                else right_side = true;      // top-right or bottom-left
            }
        }
    }
    return (left_side && right_side);
    */
}

int countXshapeMAS(const char* filepath)
{
    int count = 0;
    vector<string> matrix = get2dgrid(filepath);
    for (int x = 0; x < matrix.size(); ++x)
    {
        for (int y = 0; y < matrix[0].size(); ++y)
        {
            if (check_x_shape(matrix, x,y))
            {
                count++;
            }
        }
    }
    return count;
}

int main()
{
    cout << "total:" << countXMAS("input.txt")<< endl;
    cout << "x_shape: " << countXshapeMAS("input.txt") << endl;
    cout << "total:" << countXMAS("input_test.txt")<< endl;
    cout << "x_shape: " << countXshapeMAS("input_test.txt") << endl;
    return 0;
}
