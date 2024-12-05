#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype> // For std::isdigit
#include <algorithm> // For std::all_of
#include <regex>
using namespace std;



vector<pair<int,int>> getRules(const char* filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening: " << filepath << std::endl;
    }

    vector<pair<int,int>> rules;

    std::string line;
    // Read and parse the file

    string num;
    while (getline(file, line))
    {
        std::stringstream ss(line);
        int page1, page2;
        char delim;
        ss >> page1 >> delim >> page2;
        if (delim == '|') {
            rules.emplace_back(page1, page2);
        }
    }
    return rules;
}

vector<vector<int>> getUpdates(const char* filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening: " << filepath << std::endl;
    }

    vector<vector<int>> updates;

    std::string line;
    // Read and parse the file

    string num;
    char c;
    int index = 0;
    while (getline(file, line))
    {
        std::stringstream ss(line);
        updates.resize(index+1);
        //updates[index].resize(updates[index].size()+1);
        while (ss >> c)
        {
            if (c != ',')
            {
                num += c;
            }
            if (c == ',')
            {
                updates[index].push_back(stoi(num));
                num.clear();
            }

        }
        updates[index].push_back(stoi(num));
        num.clear();
        index++;
    }
    return updates;
}

/*
vector<pair<int,int>> get_ValidRules(vector<pair<int,int>> passin, vector<int> pages)
{
    vector<pair<int,int>> new_rules;
    for (auto pair : passin)
    {
        auto it = std::find(pages.begin(), pages.end(), pair.first);
        auto its = std::find(pages.begin(), pages.end(), pair.first);
        if (it != pages.end() && its != pages.end())
        {
            new_rules.push_back(pair);
        }
    }
    return new_rules;
}

vector<vector<int>> get_validUpdates(vector<pair<int,int>> passin, vector<vector<int>> pages)
{
    vector<vector<int>> valid_updates;
    bool is_valid = true;
    for (auto page : pages)
    {
        vector<pair<int,int>> current_rules = get_ValidRules(passin, page);
        for (auto pair : current_rules)
        {
            auto it = std::find(pages.begin(), pages.end(), pair.first);
            auto its = std::find(pages.begin(), pages.end(), pair.second);
            if (!(std::distance(pages.begin(), it) < std::distance(pages.begin(), its)))
            {
                is_valid = false;
                break;
            }
        }
        if (is_valid)
        {
            valid_updates.push_back(page);
        }
    }
    return valid_updates;
}
*/

bool isUpdateValid(const vector<int>& update, const vector<pair<int, int>>& rules) {
    for (const auto& rule : rules) {
        auto it1 = find(update.begin(), update.end(), rule.first);
        auto it2 = find(update.begin(), update.end(), rule.second);

        // If both pages are present, ensure the order is correct
        if (it1 != update.end() && it2 != update.end() && it1 > it2) {
            return false;
        }
    }
    return true;
}

void reorder_updates(vector<int>& updates, const vector<pair<int, int>>& rules)
{
    // Perform a sorting pass using the rules to enforce order
    auto comparator = [&](int a, int b) {
        for (const auto& rule : rules) {
            if (a == rule.first && b == rule.second) return true;  // a comes before b
            if (a == rule.second && b == rule.first) return false; // b comes before a
        }
        return false; // Default comparison if no rule applies
    };

    sort(updates.begin(), updates.end(), comparator);
}


int main()
{
    vector<pair<int,int>> all_rules = getRules("input_test_rules.txt");
    vector<vector<int>> all_updates = getUpdates("input_test_updates.txt");
    /*
    for (auto& rule : all_rules)
    {
        cout << rule.first << " " << rule.second << endl;
    }
    */
    /*
    for (auto& update : all_updates)
    {
        for (auto& page: update)
        {
            cout << page << " ";
        }
        cout << endl;
    }
    */
    int middle_sum = 0;
    int reordered_middle_sum = 0;

    for (auto& update : all_updates) {
        if (isUpdateValid(update, all_rules)) {
            // Get the middle page for valid updates
            int middle_index = update.size() / 2;
            middle_sum += update[middle_index];
        } else {
            // Reorder the update
            reorder_updates(update, all_rules);

            // Get the middle page after reordering
            int middle_index = update.size() / 2;
            reordered_middle_sum += update[middle_index];
        }
    }
    cout << middle_sum << endl;
    cout << reordered_middle_sum << endl;

    vector<pair<int,int>> act_all_rules = getRules("input_rules.txt");
    vector<vector<int>> act_all_updates = getUpdates("input_updates.txt");

    middle_sum = 0;
    reordered_middle_sum = 0;

    for (auto& update : act_all_updates) {
        if (isUpdateValid(update, act_all_rules)) {
            // Get the middle page for valid updates
            int middle_index = update.size() / 2;
            middle_sum += update[middle_index];
        } else {
            // Reorder the update
            reorder_updates(update, act_all_rules);

            // Get the middle page after reordering
            int middle_index = update.size() / 2;
            reordered_middle_sum += update[middle_index];
        }
    }

    cout << middle_sum << endl;
    cout << reordered_middle_sum << endl;

    return 0;
}
