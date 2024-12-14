#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <cmath>
#include <SFML/Graphics.hpp>

using namespace std;

struct Robot {
    int Px, Py; // Position
    int Vx, Vy; // Velocity
};

vector<Robot> getRobots(const char* filename)
{
    //x=column, y=row
    ifstream inputFile(filename);
    vector<Robot> Robots;
    string line;
    while (getline(inputFile, line))
    {
        istringstream ss(line);
        Robot robot;
        getline(ss, line); // Button A line
        sscanf(line.c_str(), "p=%d,%d v=%d,%d", &robot.Px, &robot.Py, &robot.Vx, &robot.Vy);
        Robots.push_back(robot);
    }
    return Robots;
}

// Function to wrap around the position if it goes out of bounds
int wrap(int value, int max) {
    if (value < 0) return (max + value) % max;
    return value % max;
}

// Function to determine which quadrant a robot belongs to based on position
int get_quadrant(int x, int y, int width, int height) {
    int mid_x = width / 2;
    int mid_y = height / 2;
    if (x == mid_x || y == mid_y) return -1; // Robots on the middle lines are ignored
    if (x < mid_x && y < mid_y) return 0;   // Top-left quadrant
    if (x >= mid_x && y < mid_y) return 1;  // Top-right quadrant
    if (x < mid_x && y >= mid_y) return 2;  // Bottom-left quadrant
    if (x >= mid_x && y >= mid_y) return 3; // Bottom-right quadrant
    return -1;
}

void afterseconds(vector<Robot>& Robots, int seconds, const int width, const int height)
{
    for (int i = 0; i < seconds; i++)
    {
        for (auto& robot : Robots)
        {
            robot.Px = wrap(robot.Px + robot.Vx, width);
            robot.Py = wrap(robot.Py + robot.Vy, height);
        }
    }
}

void countQuadrant(vector<int>& quadrants, vector<Robot> Robots, int width, int height)
{
    for (const auto robot : Robots)
    {
        int quad = get_quadrant(robot.Px, robot.Py, width, height);
        if (quad != -1) {
            quadrants[quad]++;
        }
    }
}

void print_grid(const vector<Robot> &robots, int width, int height) {
    vector<vector<char>> grid(height, vector<char>(width, ' '));
    for (const auto &robot : robots) {
        grid[robot.Py][robot.Px] = '#';
    }
    for (const auto &row : grid) {
        for (char cell : row) {
            cout << cell;
        }
        cout << endl;
    }
}

// Function to save the grid as a PNG
void save_grid_to_png(const std::vector<Robot>& robots, int width, int height, const std::string& filename) {
    // Create an SFML image (set size of the grid)
    sf::Image image;
    image.create(width, height, sf::Color::White);  // Background color is white

    // Draw robots on the image (represented by small black pixels)
    for (const auto& robot : robots) {
        if (robot.Px >= 0 && robot.Px < width && robot.Py >= 0 && robot.Py < height) {
            image.setPixel(robot.Px, robot.Py, sf::Color::Black);  // Set the pixel for the robot
        }
    }

    // Save the image as a PNG file
    if (image.saveToFile(filename)) {
        //std::cout << "Grid saved as " << filename << std::endl;
    } else {
        //std::cout << "Failed to save the grid as " << filename << std::endl;
    }
}




int main()
{
    vector<Robot> examples = getRobots("input_test.txt");
    afterseconds(examples, 100, 11,7);

    // Count robots in each quadrant
    vector<int> quadrant_counts_examples(4, 0);
    countQuadrant(quadrant_counts_examples, examples, 11, 7);

    // Calculate the safety factor
    unsigned long long safety_factor = 1;
    for (int count : quadrant_counts_examples) {
        cout << count << endl;
        safety_factor *= count;
    }

    // Output the result
    cout << "Safety Factor: " << safety_factor << endl;


    vector<Robot> robots = getRobots("input.txt");
    afterseconds(robots, 100, 101,103);

    // Count robots in each quadrant
    vector<int> quadrant_counts(4, 0);
    countQuadrant(quadrant_counts, robots, 101, 103);

    // Calculate the safety factor
    safety_factor = 1;
    for (int count : quadrant_counts) {
        safety_factor *= count;
    }

    // Output the result
    cout << "Safety Factor: " << safety_factor << endl;


    robots = getRobots("input.txt");
    //afterseconds(robots, 200, 101,103);
    int seconds = 7500; //this number came from trail and error (cuz aoc tells me if the number i inputed is too high/low)

    while (seconds < 10000)
    {
        seconds++;
        afterseconds(robots, 1, 101, 103);

        //save grid to png and eyeball it
        robots = getRobots("input.txt");
        afterseconds(robots, seconds, 101, 103);
        //cout << "seconds: " << seconds << endl;
        string filename = "sec" + to_string(seconds) + ".png";
        save_grid_to_png(robots,101, 103,filename);
    }


    return 0;
}
