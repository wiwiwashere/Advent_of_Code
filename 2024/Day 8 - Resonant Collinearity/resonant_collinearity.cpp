#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <unordered_set>
#include <unordered_map>
using namespace std;

vector<string> getGrid(const char* filepath)
{
    ifstream file(filepath);
    vector<string> Grid;

    string line;
    // Read and parse the file
    string pos;
    while (getline(file, line))
    {
        Grid.push_back(line);
    }
    return Grid;
}

// Helper to represent a position, self-created
struct Position {
    int x, y;
    Position(int x, int y) : x(x), y(y) {}
    bool operator==(const Position &other) const {
        return x == other.x && y == other.y;
    }
};

// Hash function for Position, allows us to use the Position struct as a key in an unordered_set
struct PositionHash {
    size_t operator()(const Position &pos) const {
        return hash<int>()(pos.x) ^ (hash<int>()(pos.y) << 1);
    }
};

// Parse the input grid to extract antenna positions by frequency
unordered_map<char, vector<Position>> parseMap(const vector<string> &grid) {
    unordered_map<char, vector<Position>> antennas;
    for (int y = 0; y < grid.size(); ++y) {
        for (int x = 0; x < grid[y].size(); ++x) {
            char ch = grid[y][x];
            if (ch != '.') {
                antennas[ch].emplace_back(x, y);
                //key is the antenna, value is a list of positions that the antenna exists
            }
        }
    }
    return antennas;
}

// Generate all unique antinodes for the given antennas, part 2
unordered_set<Position, PositionHash> generateAntinodes(const unordered_map<char, vector<Position>> &antennas, int width, int height, int part) {
    unordered_set<Position, PositionHash> antinodes;

    for (const auto &[freq, positions] : antennas) {
        int n = positions.size();

        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                const auto &[x1, y1] = positions[i];
                const auto &[x2, y2] = positions[j];

                if (part == 2)
                {
                    antinodes.insert(Position{x1, y1});
                    antinodes.insert(Position{x2, y2});
                }

                // Calculate deltas
                int dx = x2 - x1;
                int dy = y2 - y1;

                int x_coord = x1 - dx;
                int y_coord = y1 - dy;
                do{
                    // Check bounds and overlap for each potential antinode
                    if (x_coord >= 0 && x_coord < width && y_coord >= 0 && y_coord < height)
                    {
                        antinodes.insert(Position{x_coord, y_coord});
                    }
                    else {break;}
                    x_coord-=dx; y_coord-=dy;
                } while (part == 2);

                x_coord = x2 + dx; //(x2, y2) will be counted as position of antennas for part2
                y_coord = y2 + dy;
                do{
                    // Check bounds and overlap for each potential antinode
                    if (x_coord >= 0 && x_coord < width && y_coord >= 0 && y_coord < height)
                    {
                        antinodes.insert(Position{x_coord, y_coord});
                        //count++;
                        //cout << antinode1.y << ' ' << antinode1.x << endl;
                    }
                    else {break;}
                    x_coord+=dx; y_coord+=dy;
                } while (part == 2);
            }
        }
    }
    //cout << count << endl;
    return antinodes;
}

// Count unique antinode positions within the map
int countUniqueAntinodes(const vector<string> &grid, int part) {
    int height = grid.size();
    int width = grid[0].size();

    // Parse antennas
    auto antennas = parseMap(grid);

    // Generate antinodes, excluding overlapping antenna positions
    if (part == 1)
    {
        auto antinodes = generateAntinodes(antennas, width, height, 1);
        return antinodes.size(); // Return the total count of unique antinodes
    }
    if (part == 2)
    {
        auto antinodes_2 = generateAntinodes(antennas, width, height, 2);
        return antinodes_2.size(); // Return the total count of unique antinodes
    }
}


int main()
{
    vector<string> grid = getGrid("input_test.txt");
    cout << "Unique antinodes: " << countUniqueAntinodes(grid, 1) << endl;
    cout << "Unique antinodes: " << countUniqueAntinodes(grid, 2) << endl;
    vector<string> grid2 = getGrid("input.txt");
    cout << "Unique antinodes: " << countUniqueAntinodes(grid2, 1) << endl;
    cout << "Unique antinodes: " << countUniqueAntinodes(grid2, 2) << endl;
    return 0;
}
