#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
using namespace std;

void getInput(const char* filename, vector<vector<int>>& locks, vector<vector<int>>& keys)
{
    ifstream file(filename);
    string line;
    vector<string> pins;
    vector<int> temp;
    while (getline(file, line))
    {
        pins.push_back(line);
        if (line.empty())
        {
            for (int i = 0; i < pins[0].size(); i++)
            {
                int count = 0;
                for (int j = 0; j < pins.size(); j++)
                {
                    if (pins[j][i] == '#')
                    {
                        count++;
                    }
                }
                temp.push_back(count-1);
            }
            if (pins[0][0] == '#')
            {
                locks.push_back(temp);
            }
            else
            {
                keys.push_back(temp);
            }
            pins.clear();
            temp.clear();
        }
    }
}

int countUniquePairs(vector<vector<int>> locks, vector<vector<int>> keys)
{
    int count = 0;
    for (auto lock : locks)
    {
        for (auto key : keys)
        {
            bool is_unique = true;
            for (int i = 0; i < lock.size(); i++)
            {
                if (lock[i]+key[i]+2 > 7)
                {
                    is_unique = false;
                    break;
                }
            }
            if (is_unique) {count++;}
        }
    }
    return count;
}

int main()
{
    vector<vector<int>> locks;
    vector<vector<int>> keys;
    getInput("input.txt", locks, keys);
    cout << countUniquePairs(locks, keys) << endl;
    return 0;
}
