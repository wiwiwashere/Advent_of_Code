#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> // For std::all_of
#include <regex>
using namespace std;

// Function to read and parse the disk map from a file
vector<int> getBlocks(const char* filepath) {
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Error opening: " << filepath << endl;
        throw runtime_error("File not found or could not be opened");
    }

    string line;
    vector<int> blocks;
    while (getline(file, line)) {
        for (char ch : line) {
            blocks.push_back(ch - '0');
        }
    }

    // Generate the initial state of the disk
    vector<int> state;
    int fileID = 0;
    for (size_t i = 0; i < blocks.size(); ++i) {
        if (i % 2 == 0) { // File length
            for (int j = 0; j < blocks[i]; ++j) {
                state.push_back(fileID);
            }
            fileID++;
        } else { // Free space length
            for (int j = 0; j < blocks[i]; ++j) {
                state.push_back(-1); // -1 represents free space
            }
        }
    }
    return state;
}

// Function to compact the disk by moving the last non-free block to the first free space
void compactDisk(vector<int>& initial_blocks) {
    bool moved;
    do {
        moved = false;
        int lastNonFree = -1;
        int firstFree = -1;

        // Find the last non-free block
        for (int i = initial_blocks.size() - 1; i >= 0; --i) {
            if (initial_blocks[i] != -1) {
                lastNonFree = i;
                break;
            }
        }

        // Find the first free space
        for (int i = 0; i < initial_blocks.size(); ++i) {
            if (initial_blocks[i] == -1) {
                firstFree = i;
                break;
            }
        }

        // Swap if both positions are found and valid
        if (lastNonFree != -1 && firstFree != -1 && lastNonFree > firstFree) {
            swap(initial_blocks[lastNonFree], initial_blocks[firstFree]);
            moved = true;
        }
    } while (moved);
}

void compactDisk_byBlcoks(vector<int>& initial_blocks) {
    // Start moving files from the highest ID to the lowest
    for (int fileID = *max_element(initial_blocks.begin(), initial_blocks.end()); fileID >= 0; fileID--) {
        // Find the starting position of the current file
        size_t start = 0;
        while (start < initial_blocks.size() && initial_blocks[start] != fileID) {
            ++start;
        }

        // If the file was not found, continue to the next file
        if (start >= initial_blocks.size()) continue;

        // Find the end position of the current file
        size_t end = start;
        while (end < initial_blocks.size() && initial_blocks[end] == fileID) {
            ++end;
        }

        // Determine the length of the current file
        size_t fileLength = end - start;

        // Find a suitable span of free space to move the file to
        for (size_t i = 0; i <= initial_blocks.size() - fileLength; ++i) {
            // Check if there is a span of free space of at least 'fileLength' starting at 'i'
            bool canMove = true;
            for (size_t j = 0; j < fileLength; ++j) {
                if (initial_blocks[i + j] != -1 || i > start) {
                    canMove = false;
                    break;
                }
            }

            // If a suitable space is found, move the file
            if (canMove) {
                // Shift the file to the leftmost position
                for (size_t j = 0; j < fileLength; ++j) {
                    initial_blocks[i + j] = fileID;
                    initial_blocks[start + j] = -1;
                }
                break;
            }
        }
    }
}

// Function to calculate the checksum of the disk state
int calc_sum(const vector<int>& compacted_block) {
    unsigned long long sum = 0;
    for (size_t i = 0; i < compacted_block.size(); ++i) {
        if (compacted_block[i] != -1) { // Skip free spaces
            //cout << "Adding value: " << compacted_block[i] << " * " << i << " = " << (compacted_block[i] * i) << endl;
            sum += compacted_block[i] * i;
        } else {
            //cout << "Skipping free space at index " << i << endl;
        }
    }
    cout << "Final checksum: " << sum << endl;
    return sum;
}

int main()
{
    vector<int> initial_blocks = getBlocks("input_test.txt");
    compactDisk(initial_blocks);
    cout << calc_sum(initial_blocks) << endl;
    initial_blocks = getBlocks("input_test.txt");
    compactDisk_byBlcoks(initial_blocks);
    // for (size_t i = 0; i < initial_blocks.size(); ++i)
    // {
    //     cout << initial_blocks[i] << endl;
    // }
    cout << calc_sum(initial_blocks) << endl;
    vector<int> input_blocks = getBlocks("input.txt");
    compactDisk(input_blocks);
    cout << calc_sum(input_blocks) << endl;
    input_blocks = getBlocks("input.txt");
    compactDisk_byBlcoks(input_blocks);
    cout << calc_sum(input_blocks) << endl;
}
