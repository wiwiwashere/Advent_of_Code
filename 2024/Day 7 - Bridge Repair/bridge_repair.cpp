#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype> // For std::isdigit
#include <algorithm> // For std::all_of
#include <cmath>
#include <regex>
#include <array>
using namespace std;

vector<pair<long long, vector<long long>>> getEquation(const char* filepath)
{
    ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening: " << filepath << std::endl;
    }

    vector<pair<long long, vector<long long>>> equations;

    string line;
    // Read and parse the file

    string num;
    while (getline(file, line))
    {
        std::stringstream ss(line);
        long long result, variable;
        char delim;
        vector<long long> right_hand;
        ss >> result >> delim;
        if (delim == ':') {
            while (ss >> variable)
            right_hand.emplace_back(variable);
        }
        equations.emplace_back(result, right_hand);
    }
    return equations;
}

vector<vector<char>> generateOperatorsIterative(int max_num_operator) {
    vector<vector<char>> allOperators;
    int numCombinations = pow(2, max_num_operator); // 2^maxDepth combinations

    for (size_t i = 0; i < numCombinations; i++) {
        vector<char> current;
        int mask = i;
        for (size_t j = 0; j < max_num_operator; j++) {
            current.push_back((mask % 2 == 0) ? '+' : '*');
            mask /= 2;
        }
        allOperators.push_back(current);
    }
    return allOperators;
}


long long evaluateExpression(const vector<long long>& numbers, const vector<char>& operators) {
    long long result = numbers[0];
    for (size_t i = 0; i < operators.size(); i++) {
        if (operators[i] == '+') {
            result += numbers[i + 1];
        } else if (operators[i] == '*') {
            result *= numbers[i + 1];
        }
    }
    return result;
}

long long all_valid(vector<pair<long long, vector<long long>>> equation)
{
    long long valid_total = 0;
    for (auto [testValue, numbers] : equation)
    {
        int num_operator = numbers.size() - 1;
        vector<vector<char>> all_operators = generateOperatorsIterative(num_operator);
        for (const auto& operators : all_operators) {
            if (evaluateExpression(numbers, operators) == testValue) {

                //cout << "result: " << testValue << endl;
                valid_total += testValue;
                break;
            }
        }
    }
    return valid_total;
}

vector<vector<char>> generateOperatorsIterative_concate(int max_num_operator) {
    vector<vector<char>> allOperators;
    long numCombinations = pow(3, max_num_operator); // 2^maxDepth combinations
    vector<char> operators = {'+', '*', '|'};
    for (size_t i = 0; i < numCombinations; i++) {
        vector<char> current;
        int mask = i;
        for (size_t j = 0; j < max_num_operator; j++) {
            current.push_back(operators[mask % 3]);
            mask /= 3;
        }
        allOperators.push_back(current);
    }
    return allOperators;
}

long long evaluateExpression_concate(const vector<long long>& numbers, const vector<char>& operators) {
    long long result = numbers[0];
    for (size_t i = 0; i < operators.size(); i++) {
        if (operators[i] == '+') {
            result += numbers[i + 1];
        } else if (operators[i] == '*') {
            result *= numbers[i + 1];
        } else if (operators[i] == '|') {
            result = stoll(to_string(result) + to_string(numbers[i + 1]));
        }
    }
    return result;
}

long long all_valid_concate(vector<pair<long long, vector<long long>>> equation)
{
    long long valid_total = 0;
    for (auto [testValue, numbers] : equation)
    {
        int num_operator = numbers.size() - 1;
        vector<vector<char>> all_operators = generateOperatorsIterative_concate(num_operator);
        //cout << all_operators.size() << endl;
        for (const auto& operators : all_operators) {
            long long result = evaluateExpression_concate(numbers, operators);
            if (result == testValue) {

                //cout << "result: " << testValue << endl;
                valid_total += testValue;
                break;
            }
        }
    }
    return valid_total;
}


int main()
{
    //vector<vector<char>> all_opera = generateOperatorsIterative_concate(int(3));
    vector<pair<long long, vector<long long>>> all_equation = getEquation("input_test.txt");
    //cout << all_valid(all_equation) << endl;
    vector<pair<long long, vector<long long>>> new_equation = getEquation("input.txt");
    cout << all_valid(new_equation) << endl;
    //cout << all_valid_concate(all_equation) << endl;
    cout << all_valid_concate(new_equation) << endl;

    return 0;
}
