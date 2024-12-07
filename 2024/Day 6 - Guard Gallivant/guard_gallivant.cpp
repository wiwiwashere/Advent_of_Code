#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <set>
using namespace std;

vector<vector<pair<int,int>>> getMap(const char* filepath) //get the map in a 2d vector
{
    ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening: " << filepath << std::endl;
    }

    vector<vector<pair<int,int>>> obs_walked; //map, 2d, the first int represent if it has obstraction (0 = no, 1 = yes), second int represent if walked (0 = no, 1 = yes)

    string line;
    // Read and parse the file
    char check;
    int index = 0;
    while (getline(file, line))
    {
        stringstream ss(line);
        while (ss >> check)
        {
            obs_walked.resize(index+1);
            if (check == '.')
            {
                obs_walked[index].emplace_back(0, 0); //no obstraction, not walked
            }
            else if (check == '#')
            {
                obs_walked[index].emplace_back(1, 0); //obstraction, not walked
            }
            else if (check == '^')
            {
                obs_walked[index].emplace_back(0, 1); //no obstraction, walked
            }
        }
        index++; //move to next row in the 2d vector
    }
    return obs_walked;
}

pair<int,int> getStart(const char* filepath) //find the start coordinate
{
    ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening: " << filepath << std::endl;
    }

    vector<vector<pair<int,int>>> obs_walked;

    string line;
    // Read and parse the file
    char check;
    int index = 0;
    int row, column = 0;
    while (getline(file, line))
    {
        int co = 0; //this keep track of the index of column in a row
        stringstream ss(line);
        while (ss >> check)
        {
            obs_walked.resize(index+1);
            if (check != '^')
            {
                co++;
            }
            else if (check == '^')
            {
                row = index;
                column = co;
                return pair<int,int>(row, column);
            }
        }
        index++;
    }
}

int predict(const char* filepath, vector<vector<pair<int, int>>>& obs)
{
    vector<pair<int, int>> directions = {{-1,0}, {0,1}, {1, 0}, {0,-1}}; //up, right, down, left

    int start_row = getStart(filepath).first;
    int start_col = getStart(filepath).second;

    int dir = 0;
    int dx = directions[dir].first;
    int dy = directions[dir].second;
    int new_row = start_row;
    int new_column = start_col;
    while (! (new_row < 0 || new_column < 0 || new_row >= obs.size() || new_column >= obs[0].size()))
    {
        obs[new_row][new_column].second = 1; //mark the current position as walked
        if (new_row+dx < 0 || new_row+dx >= obs.size() || new_column+dy < 0 || new_column+dy >= obs[0].size())
        {
            //if the next in current direction steps out of the map then it means that the guard find the way out
            break;
        }
        while (obs[new_row+dx][new_column+dy].first == 1) //keep changing direction until the next in current direction isn't an obstraction
        {
            dir++;
            if (dir == 4)
            {
                dir = 0;
            }
            dx = directions[dir].first;
            dy = directions[dir].second;
        }
        new_row+=dx;
        new_column+=dy;
    }

    int count = 0;
    for (auto row: obs) //loop through the map and check if the position is walked (1)
    {
        for (auto pos: row)
        {
            if (pos.second == 1)
            {
                count++;
            }
        }
    }
    return count;
}

bool causesLoop(const char* filepath, vector<vector<pair<int,int>>> obs, int obs_row, int obs_col) {
    vector<pair<int, int>> directions = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    set<pair<pair<int, int>, int>> visited;

    vector<vector<pair<int, int>>> copy = obs; //make a copy instead of modifying the original map

    int row = getStart(filepath).first;
    int col = getStart(filepath).second;

    copy[obs_row][obs_col].first = 1; //add a new obstraction in the given position (obs_row, obs_col)

    // Mark the current position as visited
    int dir = 0;
    int dx = directions[dir].first;
    int dy = directions[dir].second;
    int new_row = row;
    int new_column = col;

    while (true)
    {
        if (visited.contains({{new_row, new_column}, dir})) {
            return true; // Loop detected as the guard as been on this position and same facing before
        }
        visited.insert({{new_row, new_column}, dir});
        if (new_row+dx < 0 || new_row+dx >= copy.size() || new_column+dy < 0 || new_column+dy >= copy[0].size())
        {
            return false; //the guard is able to walk out of the map
        }
        while (copy[new_row+dx][new_column+dy].first == 1) //keep changing direction is the next in the current direction has an obstraction
        {
            dir++;
            if (dir == 4)
            {
                dir = 0;
            }
            dx = directions[dir].first;
            dy = directions[dir].second;
            if (visited.contains({{new_row, new_column}, dir})) {
                return true; // Loop detected
            }
            visited.insert({{new_row, new_column}, dir}); //add new pos with facing direction
        }
        new_row+=dx;
        new_column+=dy; //walk to next position
        if (new_row < 0 || new_row >= copy.size() || new_column < 0 || new_column >= copy[0].size())
        {
            return false;
        }
    }

}

int stuck(const char* filepath)
{
    int count = 0;
    vector<vector<pair<int, int>>> obs_walked = getMap(filepath);
    predict(filepath, obs_walked); //modify the map so the position that the guard would have walk is marked as walked
    for (int i = 0; i < obs_walked.size(); i++)
    {
        for (int j = 0; j < obs_walked[i].size(); j++)
        {
            if (pair(i,j) != getStart(filepath) && obs_walked[i][j].second == 1) //only check the position that the guard will walk if no new obstraction is added & it cant be the start position
            {
                if (causesLoop(filepath, obs_walked, i, j)) //if placing an obstraction on (i,j) cause the guard to stuck then this is a new option
                {
                    count++;
                }
            }
        }
    }
    return count; //total number of options
}

int main()
{
    vector<vector<pair<int,int>>> obs = getMap("input_test.txt");
    cout << predict("input_test.txt", obs) << endl;
    obs = getMap("input.txt");
    cout << predict("input.txt", obs) << endl;
    cout << stuck("input_test.txt") << endl;
    cout << stuck("input.txt") << endl;
    return 0;
}
