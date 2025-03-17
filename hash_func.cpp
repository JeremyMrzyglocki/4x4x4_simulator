#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <fstream>
// I will be using the `robin_hood` library for faster unordered_map and unordered_set
#include "robin_hood.h"


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
    unordered_map<__uint128_t, long long> histogram; // Store rank occurrences
    ofstream output_file("res_compact.txt");

    long limit = 1000000; 
    int total_permutations = 0;
    do {
        if (--limit == 0) break;
        __uint128_t rank = multinomial_rank(base_string, {{'a', 7}, {'b', 8}, {'c', 8}});
        histogram[rank]++;
        total_permutations++;


        //because writing 3 seperate letters into the txt-file is not space efficient, we will combine them into one char by this
        //list:
        char combined_letter = 'o';
        if (base_string.substr(0, 3) == "aaa") {
            char combined_letter = 'A';
        } else if (base_string.substr(0, 3) == "aab") {
            char combined_letter = 'B';
        } else if (base_string.substr(0, 3) == "aac") {
            char combined_letter = 'C';
        } else if (base_string.substr(0, 3) == "aba") {
            char combined_letter = 'D';
        } else if (base_string.substr(0, 3) == "abb") {
            char combined_letter = 'E';
        } else if (base_string.substr(0, 3) == "abc") {
            char combined_letter = 'F';
        } else if (base_string.substr(0, 3) == "aca") {
            char combined_letter = 'G';
        } else if (base_string.substr(0, 3) == "acb") {
            char combined_letter = 'H';
        } else if (base_string.substr(0, 3) == "acc") {
            char combined_letter = 'I';
        } else if (base_string.substr(0, 3) == "baa") {
            char combined_letter = 'J';
        } else if (base_string.substr(0, 3) == "bab") {
            char combined_letter = 'K';
        } else if (base_string.substr(0, 3) == "bac") {
            char combined_letter = 'L';
        } else if (base_string.substr(0, 3) == "bba") {
            char combined_letter = 'M';
        } else if (base_string.substr(0, 3) == "bbb") {
            char combined_letter = 'N';
        } else if (base_string.substr(0, 3) == "bbc") {
            char combined_letter = 'O';
        } else if (base_string.substr(0, 3) == "bca") {
            char combined_letter = 'P';
        } else if (base_string.substr(0, 3) == "bcb") {
            char combined_letter = 'Q';
        } else if (base_string.substr(0, 3) == "bcc") {
            char combined_letter = 'R';
        } else if (base_string.substr(0, 3) == "caa") {
            char combined_letter = 'S';
        } else if (base_string.substr(0, 3) == "cab") {
            char combined_letter = 'T';
        } else if (base_string.substr(0, 3) == "cac") {
            char combined_letter = 'U';
        } else if (base_string.substr(0, 3) == "cba") {
            char combined_letter = 'V';
        } else if (base_string.substr(0, 3) == "cbb") {
            char combined_letter = 'W';
        } else if (base_string.substr(0, 3) == "cbc") {
            char combined_letter = 'X';
        } else if (base_string.substr(0, 3) == "cca") {
            char combined_letter = 'Y';
        } else if (base_string.substr(0, 3) == "ccb") {
            char combined_letter = 'Z';
        } else if (base_string.substr(0, 3) == "ccc") {
            char combined_letter = 'z';
        }
        
        output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
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

void test_for_collisions_starting_from() {
    string base_string = "abbbbccccccccbbbbaaaaaa";
    unordered_map<__uint128_t, long long> histogram; // Store rank occurrences
    ofstream output_file("res_compact.txt");

    long limit = 1000000; 
    int total_permutations = 0;
    
    do {
        if (--limit == 0) break;
        __uint128_t rank = multinomial_rank(base_string, {{'a', 7}, {'b', 8}, {'c', 8}});
        histogram[rank]++;
        total_permutations++;

        //because writing 3 seperate letters into the txt-file is not space efficient, we will combine them into one char by this
        //list:
        char combined_letter = 'o';
        cout << base_string.substr(0, 3) << endl;
        if (base_string.substr(0, 3) == "aaa") {
            char combined_letter = 'A';
        } else if (base_string.substr(0, 3) == "aab") {
            char combined_letter = 'B';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "aac") {
            char combined_letter = 'C';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "aba") {
            char combined_letter = 'D';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "abb") {
            char combined_letter = 'E';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "abc") {
            char combined_letter = 'F';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "aca") {
            char combined_letter = 'G';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "acb") {
            char combined_letter = 'H';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "acc") {
            char combined_letter = 'I';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "baa") {
            char combined_letter = 'J';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "bab") {
            char combined_letter = 'K';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "bac") {
            char combined_letter = 'L';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "bba") {
            char combined_letter = 'M';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "bbb") {
            char combined_letter = 'N';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "bbc") {
            char combined_letter = 'O';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "bca") {
            char combined_letter = 'P';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "bcb") {
            char combined_letter = 'Q';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "bcc") {
            char combined_letter = 'R';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "caa") {
            char combined_letter = 'S';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "cab") {
            char combined_letter = 'T';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "cac") {
            char combined_letter = 'U';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "cba") {
            char combined_letter = 'V';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "cbb") {
            char combined_letter = 'W';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "cbc") {
            char combined_letter = 'X';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "cca") {
            char combined_letter = 'Y';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "ccb") {
            char combined_letter = 'Z';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        } else if (base_string.substr(0, 3) == "ccc") {
            char combined_letter = 'z';
            output_file << combined_letter << "" << uint128_to_string(rank) << "\n";
        }

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



__uint128_t multinomial_rank_debug(const string &s, map<char, int> letter_counts) {
    vector<char> sorted_chars = {'a', 'b', 'c'};
    __uint128_t rank = 0;

    cout << "Processing string: " << s << endl;

    for (char c : s) {
        cout << "  Processing char: " << c << " Remaining counts: ";
        for (auto &[k, v] : letter_counts) cout << k << ":" << v << " ";
        cout << endl;

        for (char smaller : sorted_chars) {
            if (smaller == c) break;
            if (letter_counts[smaller] > 0) {
                letter_counts[smaller]--;

                __uint128_t total_count = accumulate(letter_counts.begin(), letter_counts.end(), (__uint128_t)0,
                    []( __uint128_t sum, const pair<char, int>& p) { return sum + p.second; });

                __uint128_t permutations = factorial_cache[total_count] / 
                    accumulate(letter_counts.begin(), letter_counts.end(), (__uint128_t)1,
                        []( __uint128_t prod, const pair<char, int>& p) { return prod * factorial_cache[p.second]; });

                cout << "    Smaller letter: " << smaller << " → Added " << uint128_to_string(permutations) << " to rank" << endl;

                rank += permutations;
                letter_counts[smaller]++;
            }
        }
        letter_counts[c]--;
    }
    cout << "Final computed rank: " << uint128_to_string(rank) << endl;
    return rank;
}

int main() {
    auto start_time = high_resolution_clock::now();

    // **Ensure factorials are precomputed**
    precompute_factorials();
    
    //string start_string = "abbbcaaacbabbcacacbcbca";
    //string start_string = "aaaaaaabbbbbbbbcccccccc";
    test_for_collisions();
    //multinomial_rank_debug("aaaaaacbcbccbbbccbccba", {{'a', 7}, {'b', 8}, {'c', 8}});
    //multinomial_rank_debug("abbbcaaacbabbcacacbcbcc", {{'a', 7}, {'b', 8}, {'c', 8}});
    
    // Run the collision test
    //test_for_collisions();
    //cout << uint128_to_string(multinomial_rank("ccccccccbbbbbbbbaaaaaaaa", {{'a', 8}, {'b', 8}, {'c', 8}})) << endl;
    //cout << uint128_to_string(multinomial_rank("abaaaaaabbbbbbbcccccccc", {{'a', 7}, {'b', 8}, {'c', 8}})) << endl;


    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    cout << "Execution time: " << duration.count() << " ms" << endl;
    return 0;
}