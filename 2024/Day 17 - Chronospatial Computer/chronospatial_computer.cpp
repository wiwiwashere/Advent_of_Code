#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ranges>
#include <cmath>
using namespace std;

// Helper function to retrieve the operand value
long long unsigned int getOperandValue(auto operand, auto regA, auto regB, auto regC) {
    if (operand < 4) return operand; // Literal values 0-3
    if (operand == 4) return regA;   // Register A
    if (operand == 5) return regB;   // Register B
    if (operand == 6) return regC;   // Register C
    throw invalid_argument("Invalid operand value");
}

// Main function to execute the program
vector<int> compute(vector<int> program, size_t regA, size_t regB, size_t regC, bool is_part1) {
    int instructionPointer = 0;
    vector<int> output;

    while (instructionPointer < program.size()) {
        int opcode = program[instructionPointer];
        int operand = program[instructionPointer + 1];
        instructionPointer += 2;

        switch (opcode) {
        case 0: { // adv
                int denominator = pow(2, getOperandValue(operand, regA, regB, regC));
                if (denominator != 0) regA /= denominator;
                break;
        }
        case 1: { // bxl
                regB ^= operand;
                break;
        }
        case 2: { // bst
                regB = getOperandValue(operand, regA, regB, regC) % 8;
                break;
        }
        case 3: { // jnz
                if (regA != 0) instructionPointer = operand;
                break;
        }
        case 4: { // bxc
                regB ^= regC;
                break;
        }
        case 5: { // out
                output.push_back(getOperandValue(operand, regA, regB, regC) % 8);
                if (!is_part1 && output[output.size() - 1] != program[output.size() - 1])
                {
                    instructionPointer = program.size();
                }
                break;
        }
        case 6: { // bdv
                int denominator = pow(2, getOperandValue(operand, regA, regB, regC));
                if (denominator != 0) regB = regA / denominator;
                break;
        }
        case 7: { // cdv
                int denominator = pow(2, getOperandValue(operand, regA, regB, regC));
                if (denominator != 0) regC = regA / denominator;
                break;
        }
        default:
            throw invalid_argument("Invalid opcode");
        }
    }

    if (is_part1)
    {
        // Convert the output vector to a comma-separated string
        stringstream outputStream;
        for (size_t i = 0; i < output.size(); ++i) {
            if (i > 0) outputStream << ",";
            outputStream << output[i];
        }
        cout << outputStream.str() << endl;
    }

    return output;
}

// Function to find the lowest positive value of register A
auto findLowestRegisterA(const vector<int>& program) {
    for (size_t regA = 7823410521923; ; regA+=8) {
        // Execute the program with the current regA
        vector<int> output = compute(program, regA, 0, 0, false);
        //output = compute(program, regA, 0, 0, true);
        // cout << "checking: " << regA << endl;
        // for (auto out: output)
        // {
        //     cout << out;
        // }
        // cout << endl;

        // Check if the output matches the program
        if (output.size() == program.size() && output == program) {
            return regA;
        }
    }
}

int main()
{
    int A = 729, B = 0, C = 0;
    vector<int> prog = {0,1,5,4,3,0};
    vector ans = compute(prog, A, B, C, true);
    //cout << ans << endl;
    A = 66752888;
    prog = {2,4,1,7,7,5,1,7,0,3,4,1,5,5,3,0};
    ans = compute(prog, A, B, C, true);

    //vector<long long> results = find(prog);
    cout << findLowestRegisterA(prog);
    //cout << ans << endl;

    return 0;
}
