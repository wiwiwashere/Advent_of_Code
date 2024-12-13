#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <cmath>

using namespace std;

struct Machine {
    long long Ax, Ay, Bx, By; // Button A and B effects on X and Y
    long long Px, Py;         // Prize location
};

vector<Machine> getMachines(const char* filename)
{
    ifstream inputFile(filename);
    vector<Machine> Claw_Machines;
    string line;
    int index = 0;
    while (getline(inputFile, line))
    {
        if (line.empty()) continue;
        istringstream ss(line);
        Machine machine;
        std::getline(ss, line); // Button A line
        sscanf(line.c_str(), "Button A: X+%lld, Y+%lld", &machine.Ax, &machine.Ay);

        std::getline(ss, line); // Button B line
        sscanf(line.c_str(), "Button B: X+%lld, Y+%lld", &machine.Bx, &machine.By);

        std::getline(ss, line); // Prize line
        sscanf(line.c_str(), "Prize: X=%lld, Y=%lld", &machine.Px, &machine.Py);
        index++;
        if (index == 3)
        {
            Claw_Machines.push_back(machine);
            index = 0;
        }
    }
    return Claw_Machines;
}

// // Extended Euclidean Algorithm to solve ax + by = gcd(a, b)
// long long gcdExtended(long long a, long long b, long long& x, long long& y) {
//     if (b == 0) {
//         x = 1;
//         y = 0;
//         return a;
//     }
//     long long gcd = gcdExtended(b, a % b, x, y);
//     long long temp = x;
//     x = y;
//     y = temp - (a / b) * y;
//     return gcd;
// }
//
// bool solveDiophantine(long long a, long long b, long long c, long long& x, long long& y) {
//     // Step 1: Find GCD and the extended coefficients
//
//     long long gcd = gcdExtended(a, b, x, y);
//
//     // Step 2: If GCD does not divide c, no solution
//     if (c % gcd != 0) return false;
//
//     // Step 3: Scale the solution
//     x = x * (c / gcd);
//     y = y * (c / gcd);
//
//     // Step 4: Find the general solution and adjust k to ensure non-negative x and y
//     long long b_div_gcd = b / gcd;
//     long long a_div_gcd = a / gcd;
//
//     // Adjust x to be non-negative by finding the appropriate k
//     long long kx = (x < 0) ? (-x + b_div_gcd - 1) / b_div_gcd : 0;  // minimum k to make x >= 0
//     long long ky = (y < 0) ? (-y + a_div_gcd - 1) / a_div_gcd : 0;  // minimum k to make y >= 0
//
//     // Choose the larger of the two k values (to satisfy both constraints)
//     long long k = std::max(kx, ky);
//
//     // Adjust x and y using k
//     x += k * b_div_gcd;
//     y -= k * a_div_gcd;
//
//     // Check if we now have non-negative x and y
//     if (x < 0 || y < 0) return false;
//
//     return true;
// }

auto tokenUsed(vector<Machine> Claw_Machines, int part)
{
    unsigned long long totalCost = 0;

    // Adjust the prize positions for the conversion error
    const long long OFFSET = 10000000000000;
    if (part == 2)
    {
        for (auto& machine : Claw_Machines) {
            machine.Px += OFFSET;
            machine.Py += OFFSET;
        }
    }

    // for (const auto& machine : Claw_Machines) {
    //     int minCost = std::numeric_limits<int>::max();
    //     bool canWin = false;
    //
    //     // Try all combinations of x and y within 0 to 100
    //     if (part == 1)
    //     {
    //         for (int x = 0; x <= 100; x++) {
    //             for (int y = 0; y <= 100; y++) {
    //                 int xPos = x * machine.Ax + y * machine.Bx;
    //                 int yPos = x * machine.Ay + y * machine.By;
    //
    //                 if (xPos == machine.Px && yPos == machine.Py) {
    //                     canWin = true;
    //                     int cost = 3 * x + y;
    //                     minCost = std::min(minCost, cost);
    //                     break;
    //                 }
    //             }
    //         }
    //         if (canWin) {
    //             ++totalPrizes;
    //             totalCost += minCost;
    //         }
    //     }
    //     else
    //     {
    //         auto idet = machine.Ax * machine.By - machine.Ay * machine.Bx;
    //         auto r1 = machine.By * machine.Px - machine.Bx * machine.Py;
    //         auto r2 = -machine.Ay * machine.Px + machine.Ax * machine.Py;
    //         if (r1 % idet == 0 and r2 % idet == 0) {
    //             totalCost += (r1 * 3 + r2) / idet;
    //             cout << "r1" << r1 << endl;
    //             cout << "r2" << r2 << endl;
    //             cout << totalCost << endl;
    //         }
    //     }
    for (const auto& m : Claw_Machines) {
        auto a = m.Ax, b = m.Ay, c = m.Bx, d = m.By, t1 = m.Px, t2 = m.Py;

        long long idet = a * d - b * c;
        long long r1 = d * t1 - c * t2;
        long long r2 = -b * t1 + a * t2;

        if (r1 % idet == 0 && r2 % idet == 0) {
            totalCost += (r1 * 3 + r2) / idet;
        }
    }
    return totalCost;
}

int main()
{
    vector<Machine> examples = getMachines("input_test.txt");
    cout << tokenUsed(examples,1) << endl;
    cout << tokenUsed(examples,2) << endl;
    vector<Machine> input = getMachines("input.txt");
    cout << tokenUsed(input,1) << endl;
    cout << tokenUsed(input,2) << endl;
    return 0;
}
