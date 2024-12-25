#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <regex>
#include <algorithm>
using namespace std;

void getInput(const char* filename, vector<unsigned long long>& numbers)
{
    ifstream file(filename);
    string line;
    while (getline(file, line))
    {
        unsigned long long num;
        istringstream ss(line);
        while (ss >> num)
        {
            numbers.push_back(num);
        }
    }
}

auto getNext(unsigned long long secret)
{
    secret ^= (secret * 64);
    secret %= 16777216;

    // Step 2: Divide by 32, round down, mix, and prune
    secret ^= (secret / 32);
    secret %= 16777216;

    // Step 3: Multiply by 2048, mix, and prune
    secret ^= (secret * 2048);
    secret %= 16777216;

    return secret;
}

// Custom hash function for vector<int>
struct VectorHash {
    std::size_t operator()(const std::vector<int>& vec) const {
        std::size_t hash = 0;
        for (int num : vec) {
            hash ^= std::hash<int>()(num) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};


// Function to find the best sequence of price changes
pair<vector<int>, int> findBestSequence(const vector<auto>& initialSecrets) {
    vector<int> bestSequence;
    int maxBananas = 0;

    // To track all sequences and their total bananas
    unordered_map<vector<int>, int, VectorHash> sequenceTotals;

    // All possible 4-change sequences
    for (int secret : initialSecrets)
    {
        vector<int> prices;

        // Generate prices and price changes
        for (int i = 0; i < 2000; ++i) {
            secret = getNext(secret);
            prices.push_back(secret % 10);
        }

        vector<int> changes;
        for (size_t i = 1; i < prices.size(); ++i) {
            changes.push_back(prices[i] - prices[i - 1]);
        }

        unordered_map<vector<int>, int, VectorHash> sequenceTotals_local;
        // Find the first occurrence of the sequence
        for (size_t i = 0; i + 3 < changes.size(); ++i) {
            vector<int> seq(changes.begin() + i, changes.begin() + i + 4);
            //vector<int> test = {-2,3,-3,3};
            if (sequenceTotals_local.count(seq) == 0) {  // Only count first occurrence for a buyer
                sequenceTotals_local[seq] += prices[i+4]; // Price after sequence
                // if (seq == test)
                // {
                //     //cout << "here" << endl;
                //     //cout << sequenceTotals_local[seq] << ", ";
                // }
                sequenceTotals[seq] += sequenceTotals_local[seq]; // Price after sequence
            }
        }
    }

    // Find the sequence with the maximum bananas
    for (const auto& [seq, total] : sequenceTotals) {
        if (total > maxBananas) {
            maxBananas = total;
            bestSequence = seq;
        }
    }

    return {bestSequence, maxBananas};
}

int main()
{
    vector<unsigned long long> numbers;
    getInput("input.txt", numbers);
    unsigned long long sum = 0;
    for (auto num: numbers)
    {
        for (int i = 0; i < 2000; i++)
        {
            num = getNext(num);
        }
        //cout << num << endl;
        sum += num;
    }
    cout << sum << endl;


    // Input the initial secret numbers
    vector<unsigned long long> initialSecrets = {1, 2, 3, 2024}; // Replace with your puzzle input
    initialSecrets = numbers;

    // Find the best sequence and maximum bananas
    auto [bestSequence, maxBananas] = findBestSequence(initialSecrets);

    // Output the result
    cout << endl;
    cout << "Best sequence: ";
    for (int change : bestSequence) {
        cout << change << " ";
    }
    cout << endl;
    cout << "Maximum bananas: " << maxBananas << endl;
}
