#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_map>
#include <cmath>
using namespace std;

// Function to simulate a single transformation
unordered_map<long long, long long> transformStones(const unordered_map<long long, long long>& stoneCounts) {
    unordered_map<long long, long long> newCounts;

    for (const auto& [stone, count] : stoneCounts) {
        if (stone == 0) {
            newCounts[1] += count;
        } else if (to_string(stone).length() % 2 == 0) {
            // Split the stone into two parts
            string stoneStr = to_string(stone);
            int mid = stoneStr.length() / 2;
            long long left = stoll(stoneStr.substr(0, mid));
            long long right = stoll(stoneStr.substr(mid));
            newCounts[left] += count;
            newCounts[right] += count;
        } else {
            // Multiply the stone by 2024
            newCounts[stone * 2024] += count;
        }
    }

    return newCounts;
}

// Function to calculate the total number of stones after a given number of blinks
long long countStonesAfterBlinks(const vector<long long>& initialStones, int blinks) {
    unordered_map<long long, long long> stoneCounts;
    for (long long stone : initialStones) { //convert the vector to a map
        stoneCounts[stone]++;
    }

    for (int i = 0; i < blinks; ++i) { //recursion
        stoneCounts = transformStones(stoneCounts);
    }

    long long totalStones = 0;
    for (const auto& [stone, count] : stoneCounts) {
        totalStones += count; //because the value for the key is the frequency that the number representing the stone occurs
    }

    return totalStones;
}

int main()
{
    vector<long long> example = {125, 17};
    vector<long long> input = {475449,2599064,213,0,2,65,5755,51149};

    int blinks = 25;
    auto totalStones = countStonesAfterBlinks(example, blinks);
    cout << "Total stones after " << blinks << " blinks: " << totalStones << endl;


    auto totalStone = countStonesAfterBlinks(input, blinks);
    cout << "Total stones after " << blinks << " blinks: " << totalStone << endl;

    blinks = 75;
    auto totalStoness = countStonesAfterBlinks(input, blinks);
    cout << "Total stones after " << blinks << " blinks: " << totalStoness << endl;

    return 0;
}
