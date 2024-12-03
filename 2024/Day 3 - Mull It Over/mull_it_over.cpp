#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype> // For std::isdigit
#include <algorithm> // For std::all_of
#include <regex>
using namespace std;

bool isNumber(const std::string& str) {
    return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
}

int is_valid(const char* filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening: " << filepath << std::endl;
    }

    auto accum = 0;

    std::string line;
    // Read and parse the file

    char str;
    bool next = false;
    char prev = '\0';
    string num;
    int number_max = 3;
    int valid_1 = 0;
    int valid_2 = 0;
    int product = 0;
    while (getline(file, line)) {
        std::stringstream ss(line);
        while (ss >> str) {
            if (str == 'm')
            {
                next = true;
                prev = str;
                continue;
            }
            if (str != 'm' && next == false)
            {
                continue;
            }
            if (next && prev == 'm' && str == 'u')
            {
                next = true;
                prev = str;
                continue;
            }
            if (next && prev == 'm' && str != 'u')
            {
                next = false;
                prev = '\0';
                continue;
            }
            if (next && prev == 'u' && str == 'l')
            {
                next = true;
                prev = str;
                continue;
            }
            if (next && prev == 'u' && str != 'l')
            {
                next = false;
                prev = '\0';
                continue;
            }
            if (next && prev == 'l' && str == '(')
            {
                next = true;
                prev = str;
                continue;
            }
            if (next && prev == 'l' && str != '(')
            {
                next = false;
                prev = '\0';
                continue;
            }
            if (next && prev == '(' && str != ',' && number_max != 0)
            {
                num += str;
                number_max--;
                if (isNumber(num) != true)
                {
                    next = false;
                    number_max = 3;
                    num.clear();
                }
                continue;
            }
            if (next && prev == '(' && str != ',' && number_max <= 0)
            {
                num.clear();
                number_max = 3;
                next = false;
                prev = '\0';
                continue;
            }
            if (next && prev == '(' && str == ',')
            {
                valid_1 = stoi(num);
                num.clear();
                number_max = 3;
                prev = str;
                continue;
            }
            if (next && prev == ',' && str != ')' && number_max != 0)
            {
                num += str;
                number_max--;
                if (isNumber(num) != true)
                {
                    next = false;
                    number_max = 3;
                    num.clear();
                    valid_1 = 0;
                }
                continue;
            }
            if (next && prev == ',' && str != ')' && number_max <= 0)
            {
                num.clear();
                number_max = 3;
                next = false;
                prev = '\0';
                continue;
            }
            if (next && prev == ',' && str == ')')
            {
                valid_2 = stoi(num);
                num.clear();
                number_max = 3;
                prev = '\0';
                product = valid_1 * valid_2;
                //cout << "product of " << valid_1 << ", " << valid_2 << " = " << product << endl;
                accum += product;
                next = false;
                valid_2 = 0;
                valid_1 = 0;
            }
        }
    }
    return accum;
}

/*
int is_valid2(const char* filepath) {
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Error opening: " << filepath << endl;
        return -1; // Return -1 to indicate an error opening the file
    }

    int accum = 0;
    string line;
    while (getline(file, line)) {
        size_t start = 0;
        while ((start = line.find("mul(", start)) != string::npos) {
            size_t end = line.find(")", start);
            if (end != string::npos) {
                string instruction = line.substr(start + 4, end - start - 4); // Extract content inside mul( and )
                stringstream ss(instruction);
                int num1, num2;
                char comma;

                if (ss >> num1 >> comma >> num2 && comma == ',') {
                    accum += num1 * num2; // Calculate and add the product
                }
                start = end + 1; // Move past the current "mul()" to look for the next one
            } else {
                break; // No closing parenthesis found, exit the loop
            }
        }
    }
    return accum;
}
*/

/*
int is_valid3(const char* filepath)
{
    std::regex mul_regex(R"(mul\((\d+),(\d+)\))");
    std::ifstream file(filepath);
    std::string line;
    int accum = 0;

    while (getline(file, line)) {
        std::smatch match;
        auto begin = std::sregex_iterator(line.begin(), line.end(), mul_regex);
        auto end = std::sregex_iterator();

        for (auto it = begin; it != end; ++it) {
            int x = std::stoi((*it)[1].str());
            int y = std::stoi((*it)[2].str());
            int product = x * y;
            accum += product;
            //std::cout << "Parsed mul(" << x << ", " << y << ") -> " << x * y << std::endl;
            //cout << "product of " << x << ", " << y << " = " << product << endl;
        }
    }
    return accum;

    std::cout << "Total: " << accum << std::endl;
}
*/

int enable(const char* filepath)
{
    std::regex mul_regex(R"(mul\((\d+),(\d+)\))");
    regex do_regex(R"(do\(\))");
    regex dont_regex(R"(don't\(\))");
    std::ifstream file(filepath);
    std::string line;
    int accum = 0;
    bool is_do = true;

    while (getline(file, line)) {
        if (regex_search(line, do_regex)) {
            is_do = true;
            //cout << "do() found. Multiplications enabled." << endl;
        }
        if (regex_search(line, dont_regex)) {
            is_do = false;
            //cout << "don't() found. Multiplications disabled." << endl;
        }

        std::smatch match;
        auto begin = std::sregex_iterator(line.begin(), line.end(), mul_regex);
        auto end = std::sregex_iterator();

        for (auto it = begin; it != end; ++it) {
            if (is_do == true)
            {
                int x = std::stoi((*it)[1].str());
                int y = std::stoi((*it)[2].str());
                accum += x * y;
            }
            //std::cout << "Parsed mul(" << x << ", " << y << ") -> " << x * y << std::endl;
        }
    }
    return accum;
}

int enable_own(const char* filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error opening: " << filepath << std::endl;
    }

    auto accum = 0;

    std::string line;
    // Read and parse the file

    char str;
    bool next = false;
    char prev = '\0';
    string num;
    int number_max = 3;
    int valid_1 = 0;
    int valid_2 = 0;
    int product = 0;
    bool is_do = true;
    bool next_enable = false;
    bool tracking_dont = false;
    while (getline(file, line)) {
        std::stringstream ss(line);
        while (ss >> str) {
            if (str == 'd')
            {
                next_enable = true;
                prev = 'd';
                continue;
            }
            if (next_enable && str == 'o' && prev == 'd')
            {
                prev = str;
                continue;
            }
            if (next_enable && str != 'o' && prev == 'd')
            {
                next_enable = false;
                prev = '\0';
                continue;
            }
            if (next_enable && str == 'n' && prev == 'o')
            {
                tracking_dont = true;
                prev = 'n';
                continue;
            }
            if (!tracking_dont && next_enable && str == '(' && prev == 'o')
            {
                prev = str;
                continue;
            }
            if (!tracking_dont && next_enable && str != '(' && prev == 'o')
            {
                next_enable = false;
                prev = '\0';
                continue;
            }
            if (!tracking_dont && next_enable && str == ')' && prev == '(')
            {
                prev = '\0';
                next_enable = false;
                is_do = true;
                continue;
            }
            if (!tracking_dont && next_enable && str != ')' && prev == '(')
            {
                next_enable = false;
                prev = '\0';
                continue;
            }
            if (tracking_dont && next_enable && str == '\'' && prev == 'n')
            {
                prev = str;
                continue;
            }
            if (tracking_dont && next_enable && str != '\'' && prev == 'n')
            {
                next_enable = false;
                tracking_dont = false;
                continue;
            }
            if (tracking_dont && next_enable && str == 't' && prev == '\'')
            {
                prev = str;
                continue;
            }
            if (tracking_dont && next_enable && str != 't' && prev == '\'')
            {
                next_enable = false;
                continue;
                tracking_dont = false;
            }
            if (tracking_dont && next_enable && str == '(' && prev == 't')
            {
                prev = str;
                continue;
            }
            if (tracking_dont && next_enable && str != '(' && prev == 't')
            {
                next_enable = false;
                continue;
                tracking_dont = false;
            }
            if (tracking_dont && next_enable && str == ')' && prev == '(')
            {
                prev = '\0';
                is_do = false;
                next_enable = false;
                tracking_dont = false;
            }
            if (tracking_dont && next_enable && str != ')' && prev == '(')
            {
                prev = '\0';
                next_enable = false;
                tracking_dont = false;
            }

            if (str == 'm')
            {
                next = true;
                prev = str;
                continue;
            }
            if (str != 'm' && next == false)
            {
                continue;
            }
            if (next && prev == 'm' && str == 'u')
            {
                next = true;
                prev = str;
                continue;
            }
            if (next && prev == 'm' && str != 'u')
            {
                next = false;
                prev = '\0';
                continue;
            }
            if (next && prev == 'u' && str == 'l')
            {
                next = true;
                prev = str;
                continue;
            }
            if (next && prev == 'u' && str != 'l')
            {
                next = false;
                prev = '\0';
                continue;
            }
            if (next && prev == 'l' && str == '(')
            {
                next = true;
                prev = str;
                continue;
            }
            if (next && prev == 'l' && str != '(')
            {
                next = false;
                prev = '\0';
                continue;
            }
            if (next && prev == '(' && str != ',' && number_max != 0)
            {
                num += str;
                number_max--;
                if (isNumber(num) != true)
                {
                    next = false;
                    number_max = 3;
                    num.clear();
                }
                continue;
            }
            if (next && prev == '(' && str != ',' && number_max <= 0)
            {
                num.clear();
                number_max = 3;
                next = false;
                prev = '\0';
                continue;
            }
            if (next && prev == '(' && str == ',')
            {
                valid_1 = stoi(num);
                num.clear();
                number_max = 3;
                prev = str;
                continue;
            }
            if (next && prev == ',' && str != ')' && number_max != 0)
            {
                num += str;
                number_max--;
                if (isNumber(num) != true)
                {
                    next = false;
                    number_max = 3;
                    num.clear();
                    valid_1 = 0;
                }
                continue;
            }
            if (next && prev == ',' && str != ')' && number_max <= 0)
            {
                num.clear();
                number_max = 3;
                next = false;
                prev = '\0';
                continue;
            }
            if (next && prev == ',' && str == ')')
            {
                valid_2 = stoi(num);
                num.clear();
                number_max = 3;
                prev = '\0';
                product = valid_1 * valid_2;
                //cout << "product of " << valid_1 << ", " << valid_2 << " = " << product << endl;
                if (is_do)
                {
                    accum += product;
                    //cout << "product of " << valid_1 << ", " << valid_2 << " = " << product << endl;

                }
                else
                {
                    product = 0;
                }
                next = false;
                valid_2 = 0;
                valid_1 = 0;
            }
        }
    }
    return accum;
}

int main()
{
    cout << is_valid("input.txt") << endl;
    //cout << is_valid2("input.txt") << endl;
    //cout << is_valid3("input.txt") << endl;
    //cout << enable("input.txt") << endl;
    cout << enable_own("input.txt") << endl;
    //cout << enable_own("input_test.txt") << endl;
    //cout << is_valid("input_test.txt") << endl;
}
