#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <fstream>

using namespace std;
using namespace std::chrono;

// Precompute factorials for fast lookup (128-bit)
vector<__uint128_t> factorial_cache(25, 1);
void precompute_factorials() {
    for (int i = 1; i < 25; ++i) {
        factorial_cache[i] = factorial_cache[i - 1] * i;
    }
}

// Convert `__uint128_t` to a string for printing
string uint128_to_string(__uint128_t value) {
    string result;
    while (value) {
        result = char('0' + (value % 10)) + result;
        value /= 10;
    }
    return result.empty() ? "0" : result;
}

// Multinomial ranking function (fixed)
__uint128_t multinomial_rank(const string &s, map<char, int> letter_counts) {
    vector<char> sorted_chars = {'a', 'b', 'c'}; // Define possible characters in lexicographic order
    __uint128_t rank = 0;
    
    for (char c : s) {
        for (char smaller : sorted_chars) {
            if (smaller == c) break; // Stop when reaching the current character
            if (letter_counts[smaller] > 0) {
                letter_counts[smaller]--;

                // Fix type of `permutations` to `__uint128_t`
                __uint128_t permutations = factorial_cache[
                    accumulate(letter_counts.begin(), letter_counts.end(), 0,
                        [](int sum, const pair<char, int>& p) { return sum + p.second; })
                ] / accumulate(letter_counts.begin(), letter_counts.end(), (__uint128_t)1,
                        []( __uint128_t prod, const pair<char, int>& p) { return prod * factorial_cache[p.second]; });

                rank += permutations;
                letter_counts[smaller]++; // Restore count
            }
        }
        letter_counts[c]--; // Reduce count for the processed character
    }
    return rank;
}

// Function to test for collisions and save results to a file
void test_for_collisions() {
    string base_string = "aaaaaaabbbbbbbbcccccccc"; // 7 'a's, 8 'b's, 8 'c's
    unordered_map<__uint128_t, int> histogram; // Store rank occurrences
    ofstream output_file("23_2100mio.txt");

    int limit = 2100000000; 
    int total_permutations = 0;
    do {
        if (--limit == 0) break;
        __uint128_t rank = multinomial_rank(base_string, {{'a', 7}, {'b', 8}, {'c', 8}});
        histogram[rank]++;
        total_permutations++;

        // Only write the first 3 letters of the cube state
        output_file << base_string.substr(0, 3) << "" << uint128_to_string(rank) << "\n";
    } while (next_permutation(base_string.begin(), base_string.end()));

    // Check for collisions
    int collisions = 0;
    for (const auto &entry : histogram) {
        if (entry.second > 1) {
            output_file << "C" << uint128_to_string(entry.first) << "." << entry.second << "\n";
            collisions++;
        }
    }

    // Summary
    cout << "Total unique ranks generated: " << histogram.size() << endl;
    cout << "Total permutations checked: " << total_permutations << endl;
    cout << "Total collisions found: " << collisions << endl;
    output_file.close();
}

int main() {
    auto start_time = high_resolution_clock::now();

    // **Ensure factorials are precomputed**
    precompute_factorials();
    
    // Run the collision test
    //test_for_collisions();
    cout << uint128_to_string(multinomial_rank("ccccccccbbbbbbbbaaaaaaaa", {{'a', 8}, {'b', 8}, {'c', 8}})) << endl;
    //cout << uint128_to_string(multinomial_rank("abaaaaaabbbbbbbcccccccc", {{'a', 7}, {'b', 8}, {'c', 8}})) << endl;



    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    cout << "Execution time: " << duration.count() << " ms" << endl;
    return 0;
}