#include <iostream>
#include <fstream>
#include <regex>
#include <SFML/Graphics.hpp>
#include <set>
using namespace std;

// Directions mapping
int Dx[] = {0, 0, -1, 1}; // Up, Down, Left, Right
int Dy[] = {-1, 1, 0, 0};
const char directions[] = {'^', 'v', '<', '>'};

// Helper function to find direction index
int getDirectionIndex(char move) {
    for (int i = 0; i < 4; i++) {
        if (directions[i] == move) return i;
    }
    return -1;
}

void printGrid(vector<string> grid)
{
    for (int i = 0; i < grid.size(); i++)
    {
        cout << grid[i] << endl;
    }
}

// // Function to save the grid as a PNG
// void save_grid_to_png(const vector<string>& grid, const string& filename) {
//     // Define cell size for visualization (e.g., each grid cell becomes a 10x10 block of pixels)
//     const int cellSize = 10;
//     const int width = grid[0].size() * cellSize; // Calculate image width
//     const int height = grid.size() * cellSize;  // Calculate image height
//
//     // Create an SFML image
//     sf::Image image;
//     image.create(width, height, sf::Color::White); // Start with a white background
//
//     // Map grid characters to colors
//     auto getColor = [](char c) {
//         switch (c) {
//         case '#': return sf::Color::Black;    // Walls
//         case '@': return sf::Color::Red;      // Robot
//         case 'O': return sf::Color::Blue;     // Boxes
//         case '[': case ']': return sf::Color::Green; // Box parts
//         case '.': return sf::Color::White;    // Empty spaces
//         default: return sf::Color::Magenta;   // Undefined characters
//         }
//     };
//
//     // Fill the image based on the grid
//     for (int y = 0; y < grid.size(); y++) {
//         for (int x = 0; x < grid[y].size(); x++) {
//             sf::Color color = getColor(grid[y][x]);
//             // Draw the cell as a block of pixels
//             for (int dy = 0; dy < cellSize; dy++) {
//                 for (int dx = 0; dx < cellSize; dx++) {
//                     image.setPixel(x * cellSize + dx, y * cellSize + dy, color);
//                 }
//             }
//         }
//     }
//
//     // Save the image as a PNG file
//     if (image.saveToFile(filename)) {
//         cout << "Grid saved as " << filename << endl;
//     } else {
//         cout << "Failed to save the grid as " << filename << endl;
//     }
// }


pair<int, int> getStart(vector<string> warehouse)
{
    // Find initial position of the robot
    for (int i = 0; i < warehouse.size(); i++) {
        for (int j = 0; j < warehouse[0].size(); j++) {
            if (warehouse[i][j] == '@') {
                return make_pair(i, j);
            }
        }
    }
    return make_pair(-1, -1);
}

// void handlebox(vector<string> grid, vector<pair<char, pair<int, int>>>& boxes, int bx, int by, int dir) {
//     int dx = Dx[dir]; int dy = Dy[dir];
//     if (grid[by][bx] == '[') {
//         boxes.emplace_back(make_pair('[', make_pair(bx, by)));
//         boxes.emplace_back(make_pair(']', make_pair(bx+1, by)));
//         handlebox(grid, boxes, bx + dx, by + dy, dir);
//         if (dx != -1)
//         {
//             handlebox(grid, boxes, bx + 1 + dx, by + dy, dir);
//         }
//         //handlebox(grid, boxes, bx + 1, by, dir);
//     }
//     else if (grid[by][bx] == ']') {
//         boxes.emplace_back(make_pair(']', make_pair(bx, by)));
//         boxes.emplace_back(make_pair('[', make_pair(bx - 1, by)));
//         // Recurse for the next left coordinate and further down
//         if (dir == 1 || dir == 3) { // Specifically handle down or up directions
//             handlebox(grid, boxes, bx - 1, by + dy, dir); // Check the left coordinate down
//         }
//         if (dx != 1)
//         {
//             handlebox(grid, boxes, bx - 1 + dx, by + dy, dir);
//         }
//         handlebox(grid, boxes, bx + dx, by + dy, dir);
//     }
//     else if (grid[by][bx] == '#')
//     {
//         boxes.clear();
//     }
// }


void handlebox(const vector<string>& grid, vector<pair<char, pair<int, int>>>& boxes, int bx, int by, int dir) {
    // Define a set to track visited positions
    set<pair<int, int>> visited;
    // Stack for iterative traversal (position + direction)
    stack<tuple<int, int, int>> to_visit;
    to_visit.push({bx, by, dir});

    while (!to_visit.empty()) {
        auto [x, y, d] = to_visit.top();
        to_visit.pop();

        // Skip already visited positions
        if (visited.count({x, y})) continue;

        // Mark current position as visited
        visited.insert({x, y});
        int dx = Dx[d], dy = Dy[d];

        if (grid[y][x] == '[') {
            // Add box parts
            boxes.emplace_back(make_pair('[', make_pair(x, y)));
            boxes.emplace_back(make_pair(']', make_pair(x + 1, y)));

            to_visit.push({x + 1, y, d});

            // Add new positions to process
            to_visit.push({x + dx, y + dy, d});
            if (dx != -1) {
                to_visit.push({x + 1 + dx, y + dy, d});
            }
        } else if (grid[y][x] == ']') {
            // Add box parts
            boxes.emplace_back(make_pair(']', make_pair(x, y)));
            boxes.emplace_back(make_pair('[', make_pair(x - 1, y)));

            to_visit.push({x - 1, y, d});

            if (dx != 1) {
                to_visit.push({x - 1 + dx, y + dy, d});
            }
            to_visit.push({x + dx, y + dy, d});
        } else if (grid[y][x] == '#') {
            // Clear boxes if hitting a wall
            boxes.clear();
            return;
        }
    }
}

// Function to simulate robot movements and compute GPS sum
auto simulateRobot(vector<string>& warehouse, const string &moves, int part)
{
    int robotX = getStart(warehouse).first, robotY = getStart(warehouse).second;
    int rows = warehouse.size();
    int cols = warehouse[0].size();

    // Process each move
    for (char move : moves)
        {
            //cout << "move " << move << endl;
            //cout << endl;
            int dir = getDirectionIndex(move);
            if (dir == -1) continue; // Skip invalid moves

            int newX = robotX + Dx[dir];
            int newY = robotY + Dy[dir];

            // Check if robot can move
            if (warehouse[newY][newX] == '.') {
                // Move robot
                warehouse[robotY][robotX] = '.';
                warehouse[newY][newX] = '@';
                robotX = newX;
                robotY = newY;
            } else if (warehouse[newY][newX] == 'O' || warehouse[newY][newX] == '[' || warehouse[newY][newX] == ']') {
                // Attempt to push box
                int boxX = newX, boxY = newY;
                int boxNewX = boxX + Dx[dir], boxNewY = boxY + Dy[dir];

                // Check for consecutive boxes and empty space
                while (warehouse[boxNewY][boxNewX] == 'O' || warehouse[boxNewY][boxNewX] == '[' || warehouse[boxNewY][boxNewX] == ']') {
                    boxNewX += Dx[dir];
                    boxNewY += Dy[dir];
                }

                // If there's empty space after the boxes, move all boxes
                if (warehouse[boxNewY][boxNewX] == '.') {
                    // Move all boxes forward
                    while (boxX != boxNewX || boxY != boxNewY) {
                        if (part == 1)
                        {
                            warehouse[boxNewY][boxNewX] = 'O';
                        }
                        boxNewX -= Dx[dir];
                        boxNewY -= Dy[dir];
                    }
                    warehouse[boxY][boxX] = '.';

                    // Move the robot into the space of the first box
                    warehouse[newY][newX] = '@';
                    warehouse[robotY][robotX] = '.';
                    robotX = newX;
                    robotY = newY;
                }
            //printGrid(warehouse);
            //cout << endl;
        }
        //break;
    }

    // Calculate the GPS sum of all boxes
    auto gpsSum = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (warehouse[i][j] == 'O' || warehouse[i][j] == '[') {
                gpsSum += 100 * i + j;
            }
        }
    }

    return gpsSum;
}

// Function to simulate the robot's movements and handle box pushes
auto simulateRobot2(vector<string>& warehouse, const string& moves) {
    int robotX, robotY;
    int index = 1;
    tie(robotY, robotX) = getStart(warehouse);
    string filename = "";

    for (char move : moves) {
        //cout << "move:" << move << endl;
        //filename+="move";
        if (move == 'v')
        {
            filename+="DOWN";
        }
        else if (move == '^')
        {
            filename+="UP";
        }
        else if (move == '<')
        {
            filename+="LEFT";
        }
        else if (move == '>')
        {
            filename+="RIGHT";
        }
        //cout << endl;
        int dir = getDirectionIndex(move);
        if (dir == -1) continue; // Skip invalid moves

        int newX = robotX + Dx[dir];
        int newY = robotY + Dy[dir];

        // Check if the robot can move
        if (warehouse[newY][newX] == '.') {
            // Move the robot
            warehouse[robotY][robotX] = '.';
            warehouse[newY][newX] = '@';
            robotX = newX;
            robotY = newY;
        }
        else if (warehouse[newY][newX] == '[' || warehouse[newY][newX] == ']') {
            // Identify boxes to be moved
            //cout << warehouse[newY][newX] << endl;
            vector<pair<char, pair<int, int>>> boxesToMove;
            handlebox(warehouse, boxesToMove, newX, newY, dir);

            // First pass: Remove boxes
            for (const auto& box : boxesToMove) {
                warehouse[box.second.second][box.second.first] = '.';
            }

            // Second pass: Move boxes to new positions
            for (const auto& box : boxesToMove) {
                int boxNewX = box.second.first + Dx[dir];
                int boxNewY = box.second.second + Dy[dir];
                warehouse[boxNewY][boxNewX] = box.first;
            }

            // Move robot into the space of the first box
            warehouse[robotY][robotX] = '.';
            if(!boxesToMove.empty())
            {
                warehouse[newY][newX] = '@';
                robotX = newX;
                robotY = newY;
            }
            else
            {
                warehouse[robotY][robotX] = '@';
            }
        }
        filename+=to_string(index);
        index++;
        filename += ".png";
        //save_grid_to_png(warehouse, filename);
        filename.clear();
        //printGrid(warehouse);
        //cout << endl;
    }

    // Calculate the GPS sum of all boxes
    auto gpsSum = 0;
    for (int i = 0; i < warehouse.size(); i++) {
        for (int j = 0; j < warehouse[0].size(); j++) {
            if (warehouse[i][j] == 'O' || warehouse[i][j] == '[') {
                gpsSum += 100 * i + j;
            }
        }
    }

    return gpsSum;
}

// Function to resize the warehouse map
vector<string> resizeWarehouse(const vector<string>& warehouse) {
    vector<string> resized;
    for (const auto& row : warehouse) {
        string newRow;
        for (char cell : row) {
            if (cell == '#') newRow += "##";
            else if (cell == 'O') newRow += "[]";
            else if (cell == '.') newRow += "..";
            else if (cell == '@') newRow += "@.";
        }
        resized.push_back(newRow);
    }
    return resized;
}

int main()
{
    vector<string> map;
    string movements;
    ifstream inputFile("input.txt");
    string line;
    bool is_move_line = false;
    while (getline(inputFile, line))
    {
        if (line.empty())
        {
            is_move_line = true;
            continue;
        }
        if (!is_move_line) {map.emplace_back(line);}
        if (is_move_line)
        {
            movements += line;
        }
    }

    vector<string> double_map = resizeWarehouse(map);
    //printGrid(double_map);
    cout << simulateRobot(map, movements, 1) << endl;

    cout << simulateRobot2(double_map, movements) << endl;
    //printGrid(double_map);
    return 0;
}
