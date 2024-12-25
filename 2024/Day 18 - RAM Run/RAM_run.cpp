#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <set>
#include <regex>
using namespace std;

vector<pair<int, int>> direction = {{-1,0},{0,-1},{1,0},{0,1}}; //up, left, down, right

void getFallenBytes(const char* filename, vector<pair<int,int> >& fallen, int first_few)
{
    ifstream infile(filename);
    string line;
    fallen.clear();
    while(getline(infile,line))
    {
        istringstream ss(line);
        int row, column;
        getline(ss,line);
        sscanf(line.c_str(),"%d,%d",&column,&row);
        fallen.emplace_back(row,column);
        if (fallen.size() == first_few)
        {
            break;
        }
    }
}

void printGrid(vector<vector<char>>& grid)
{
    for (auto row: grid)
    {
        for (auto column: row)
        {
            cout << column << "";
        }
        cout << endl;
    }
}

struct state
{
    int x, y, score;

    bool operator>(const state& s) const
    {
        return score > s.score;
    }
};

bool isValid(int x, int y, const vector<pair<int,int> >& fallen, int grid_size)
{
    auto it = find(fallen.begin(), fallen.end(), pair<int,int>(x,y));
    return x>=0 && y>=0 && x<grid_size && y<grid_size && it == fallen.end();
}

int getShortestPath(vector<vector<char>>& grid, vector<pair<int,int>> fallen_bytes, int grid_size)
{
    //modify first
    // for (auto p: fallen_bytes)
    // {
    //     grid[p.first][p.second] = '#';
    // }
    // //printGrid(grid);

    queue<state> q;
    set<pair<int,int>> visited;

    q.push({0, 0, 0});
    visited.insert({0, 0});

    state current;

    while(!q.empty())
    {
        for (int i = 0; i < q.size(); i++)
        {
            current = q.front();
            q.pop();

            if (current.x == grid_size - 1 && current.y == grid_size - 1)
            {
                return current.score;
            }

            for (const auto& dir: direction)
            {
                int nx = current.x + dir.first;
                int ny = current.y + dir.second;
                if (isValid(nx,ny,fallen_bytes,grid_size) && visited.find({nx,ny}) == visited.end())
                {
                    q.push({nx,ny, current.score + 1});
                    visited.insert({nx,ny});
                }
            }
        }
    }
    return -1;
}

pair<int,int> getFirstBlock(vector<pair<int,int>>& fallen_bytes, int grid_size, vector<vector<char>>& grid)
{
    for (int i = 1; i < grid_size*grid_size; i++)
    {
        //cout << i << " ";
        getFallenBytes("input.txt",fallen_bytes,i);
        if (getShortestPath(grid, fallen_bytes, grid_size) == -1)
        {
            cout << "here" << endl;
            return fallen_bytes[i-1];
        }
    }
}

int main()
{
    vector<pair<int,int> > fallen_bytes;
    vector<vector<char>> grid(71, vector<char>(71, '.'));
    getFallenBytes("input.txt",fallen_bytes, 1024);
    cout << getShortestPath(grid, fallen_bytes, 71) << endl;
    pair<int,int> first_block = getFirstBlock(fallen_bytes, 71, grid);
    cout << first_block.second << ", " << first_block.first << endl;
    return 0;
}
