#include <iostream>
using namespace std;
#include <sstream>
#include <fstream>
#include <vector>


bool is_inc(vector<int> report)
{
    for (size_t i = 1; i < report.size(); ++i)
    {
        int diff = report[i] - report[i - 1];
        {
            if (diff <= 0)
            {
                return false;
            }
            if (diff > 3)
            {
                return false;
            }
        }
    }
    return true;
}

bool is_dec(vector<int> report)
{
    for (size_t i = 1; i < report.size(); ++i)
    {
        int diff = report[i] - report[i - 1];
        {
            if (diff >= 0)
            {
                return false;
            }
            if (diff < -3)
            {
                return false;
            }
        }
    }
    return true;
}

int safe(const char* filepath)
{
    int num_safe = 0;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening: " << filepath << std::endl;
    }

    std::vector<std::vector<int>> reports;
    std::string line;
    std::vector<int> report;

    // Read and parse the file
    while (getline(file, line)) {
        std::stringstream ss(line);
        int level;
        while (ss >> level) {
            report.push_back(level);
        }
        if (!report.empty()) {
            reports.push_back(report);
            report.clear();
        }
    }

    for (const auto& report : reports) {
        if (is_inc(report) || is_dec(report)) {
            num_safe++;
            for (int i = 0; i < report.size(); ++i)
            {
                cout << report[i] << " ";
            }
            cout << endl;
        }
    }

    return num_safe;
}

bool is_safe_remove(vector<int> report)
{
    vector<int> temp;
    for (int i = 0; i < report.size(); i++){
        for (int j = 0; j < report.size(); j++)
        {
            if (j != i)
            {
                temp.push_back(report[j]);
            }
        }
        if (is_inc(temp) || is_dec(temp))
        {
            return true;
        }
        temp.clear();
    }
    return false;
}

int dampener(const char* filepath)
{
    int num_safe = 0;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening: " << filepath << std::endl;
    }

    std::vector<std::vector<int>> reports;
    std::string line;
    std::vector<int> report;

    // Read and parse the file
    while (getline(file, line)) {
        std::stringstream ss(line);
        int level;
        while (ss >> level) {
            report.push_back(level);
        }
        if (!report.empty()) {
            reports.push_back(report);
            report.clear();
        }
    }

    for (const auto& report : reports) {
        if (is_inc(report) || is_dec(report) || is_safe_remove(report))
        {
            num_safe++;
        }
    }
    return num_safe;
}

int main()
{
    //cout << safe("reports_test.txt") << endl;
    //cout << dampener("reports_test.txt") << endl;
    cout << safe("reports_input.txt") << endl;
    cout << dampener("reports_input.txt") << endl;
}
