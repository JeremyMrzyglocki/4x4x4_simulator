#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <map>
#include <chrono> 
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <random>


using namespace std;
using namespace std::chrono; 

// Move application and rotation
string apply_move(const string& state, const string& move);
string get_canonical_rotation(const string& cubestate);

// Move functions
string R(const string&); string R2(const string&); string R3(const string&);
string L(const string&); string L2(const string&); string L3(const string&);
string F(const string&); string F2(const string&); string F3(const string&);
string B(const string&); string B2(const string&); string B3(const string&);
string U(const string&); string U2(const string&); string U3(const string&);
string D(const string&); string D2(const string&); string D3(const string&);

string r(const string&); string r2(const string&); string r3(const string&);
string l(const string&); string l2(const string&); string l3(const string&);
string f(const string&); string f2(const string&); string f3(const string&);
string b(const string&); string b2(const string&); string b3(const string&);
string u(const string&); string u2(const string&); string u3(const string&);
string d(const string&); string d2(const string&); string d3(const string&);

string x_rotation(const string&);
string x2_rotation(const string&);
string x3_rotation(const string&);
string y_rotation(const string&);
string y2_rotation(const string&);
string y3_rotation(const string&);
string z_rotation(const string&);
string z2_rotation(const string&);
string z3_rotation(const string&);

// Mirror functions
string m_mirror(const string&);
string s_mirror(const string&);
string e_mirror(const string&);

// Letter normalization
string fixLetterOrder(const string&);

// Letter swap
string letter_swap(string, char, char);

ofstream log_file("log.txt", ios::out); // Log file for all printed outputs

const int MAX_MOVES = 20;
int move_histogram[MAX_MOVES] = {0};
int solution_counter = 0;  // Global counter to track the number of solutions found
int dropout_counter = 0;

// Global variable to store updated states
unordered_set<string> updated_states0, updated_states1, updated_states2, updated_states3, updated_states4,updated_states5,
updated_states6, updated_states7, updated_states8, updated_states9, updated_states10, updated_states11, updated_states12;

const vector<string> MOVES = {
    "R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
    "U", "U2", "U'", "D", "D2", "D'", "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'",
    "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"
};


// Precompute factorials for fast lookup (128-bit)
vector<__uint128_t> factorial_cache(25, 1);
void precompute_factorials() {
    for (int i = 1; i < 25; ++i) {
        factorial_cache[i] = factorial_cache[i - 1] * i;
    }
}

void log_message(const string &message) {
    cout << message << endl;
    if (log_file.is_open()) {
        log_file << message << endl;
    }
}

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

string uint128_to_string(__uint128_t value) {
    string result;
    while (value) {
        result = char('0' + (value % 10)) + result;
        value /= 10;
    }
    return result.empty() ? "0" : result;
}

// not using this function anymore, as it only eliminates 10% (as a factor, not base of exponential function) of cases while adding 4x as much time in generation
bool is_already_flagged_or_mirrored(const string &canonical_state, const string &filename, int depth, const map<char, int> &letter_counts) {

    vector<string> all_mirrors = {
        canonical_state,
        fixLetterOrder(m_mirror(canonical_state)),
        fixLetterOrder(s_mirror(canonical_state)),
        fixLetterOrder(e_mirror(canonical_state))
    };

    for (const string &variant : all_mirrors) {
        string without_a = variant.substr(1);
        __uint128_t index = multinomial_rank(without_a, letter_counts);

        ifstream input_file(filename, ios::binary);
        if (!input_file) {
            cerr << "Error opening file: " << filename << endl;
            return false;
        }

        streampos file_position = index * sizeof(uint32_t);
        input_file.seekg(file_position, ios::beg);
        uint32_t packed_value;
        input_file.read(reinterpret_cast<char*>(&packed_value), sizeof(uint32_t));
        input_file.close();

        uint32_t flag = packed_value & 0xF;  // Get the lower 4 bits (flag)
        if (flag <= depth) {
            return true;  // One of the mirrored states is already flagged with equal or lower depth
        }
    }
    return false;
}

// this function is needed because the two editors / VSC-extensions for opening .bin-files that I had tried, were not good. So I inserted a "reader"-function.
void binary_viewer(const string &filename, int num_entries) {
    ifstream input_file(filename, ios::binary);
    if (!input_file) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    cout << "\nReading first " << num_entries << " entries from " << filename << "...\n";

    for (int i = 0; i < num_entries; ++i) {
        uint32_t packed_value;
        input_file.read(reinterpret_cast<char*>(&packed_value), sizeof(uint32_t));
        if (input_file.eof()) break;

        // Extract 31-bit number and 4-bit flag
        uint32_t value_part = packed_value >> 4;   // Right shift to remove the flag
        uint32_t flag_part = packed_value & 0xF;   // Mask last 4 bits for flag

        // Convert to binary representation
        string binary_representation = bitset<31>(value_part).to_string();
        string flag_representation = bitset<4>(flag_part).to_string();

        cout << "Binary: " << binary_representation << " | Flag: " << flag_representation << " | Decimal: " << value_part << endl;
    }
    input_file.close();
}


// Function to update flag in binary file at a specific index
// With "Flags", I am refering the depth that has been found. As I hypothesize God's Number being at about 12, there are three states left, that can be expressed
// by 4 bits. 1111=15, right now stand for "uninitialized".
void update_flag_in_file(const string &filename, uint32_t index, uint32_t new_flag) {
    const int bit_length_1 = 31;  // Number of bits for value
    const int bit_length_2 = 4;   // Number of bits for flag
    const int entry_size = 4;     // Each entry is stored in 4 bytes (32 bits)

    // Calculate position in file
    streampos file_position = index * entry_size;

    fstream file(filename, ios::binary | ios::in | ios::out);
    if (!file) {
        log_message("Error opening file: " + filename);
        return;
    }

    file.seekg(file_position, ios::beg);
    uint32_t entry;
    file.read(reinterpret_cast<char*>(&entry), sizeof(uint32_t));

    if (file.eof()) {
        log_message("Error: Index out of range (attempted index: " + to_string(index) + ")"); file.close();
        return;
    }
    // Extract the value part (first 31 bits)
    uint32_t value_part = entry >> bit_length_2;
    uint32_t current_flag = entry & 0xF;  // Extract the last 4 bits (flag)
    stringstream log_info;

    if (current_flag == 0b1111) {
        uint32_t updated_entry = (value_part << bit_length_2) | (new_flag & 0xF);
        file.seekp(file_position, ios::beg);
        file.write(reinterpret_cast<const char*>(&updated_entry), sizeof(uint32_t));
    }
    file.close();
    
}

void binary_generator(__uint128_t start_val, __uint128_t end_val, const string &filename) {
    auto start_time = high_resolution_clock::now();
    
    ofstream output_file(filename, ios::binary);
    if (!output_file) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    
    int bit_length_1 = 31;  // 31 bits for the main value
    int bit_length_2 = 4;   // 4 bits reserved for flags
    int total_bits = bit_length_1 + bit_length_2; // 35 bits total

    cout << "Using " << total_bits << " bits per entry ("
         << bit_length_1 << " for value, " << bit_length_2 << " for flags)." << endl;
    
    for (__uint128_t i = start_val; i <= end_val; ++i) {
        uint32_t value_part = static_cast<uint32_t>(i) & ((1U << bit_length_1) - 1);  // Store 31-bit number
        uint32_t flag_part = 15;  // Store 4-bit flag

        uint32_t packed_value = (value_part << bit_length_2) | (flag_part & 0xF); // Shift value left, add 4-bit flag
        
        output_file.write(reinterpret_cast<const char*>(&packed_value), sizeof(uint32_t));
    }
    
    output_file.close();
    
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    cout << "Binary generation time: " << duration.count() << " ms" << endl;
}


// Function to update depth 0: Flag starting state with 0000
void depth_0_updater(const string &filename) {
    log_message("\n🔄 Updating depth 0 starting state...");
    __uint128_t index = multinomial_rank("aaabbbbccccbbbbccccaaaa", {{'a', 7}, {'b', 8}, {'c', 8}}); // the letters here are the starting_cubestate without the first a

    update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0000);
    log_message("Flag at index " + uint128_to_string(index) + " set to 0000.");

    updated_states0.insert("aaabbbbccccbbbbccccaaaa"); // the "solved"-state without the first a is saved

    ofstream file("updated_states_depth_0.txt", ios::out);
    if (!file) {log_message("❌ Error opening file: updated_states_depth_0.txt"); return;}

    for (const auto &state : updated_states0) {
        file << state << endl;
    }
    file.close();
    log_message("📁 Depth 0 states saved to updated_states_depth_0.txt");
}

void depth_updater(
    int depth,
    const string &filename,
    const unordered_set<string> &previous_depth_states,
    unordered_set<string> &current_depth_states,
    const string &output_txt_filename
) {
    log_message("\n🔄 Generating depth " + to_string(depth) + " states...");

    unordered_set<string> prefixed_states;
    for (const auto &state : previous_depth_states) {
        prefixed_states.insert("a" + state);  // Add back the leading 'a'
    }
    
    for (const auto &state : prefixed_states) {
        for (const auto &move : MOVES) {
            string moved = apply_move(state, move);
            string canonical = get_canonical_rotation(moved);
            string fixed_state = canonical.substr(1);  // remove leading 'a'

            // Optional: skip flagged/mirrored states here if re-enabled
            //if (is_already_flagged_or_mirrored(canonical, filename, depth, {{'a', 7}, {'b', 8}, {'c', 8}})) continue;

            current_depth_states.insert(fixed_state);
        }
    }

    log_message("📝 Updating binary file with " + to_string(current_depth_states.size()) + " depth " + to_string(depth) + " states...");

    for (const auto &state : current_depth_states) {
        __uint128_t index = multinomial_rank(state, {{'a', 7}, {'b', 8}, {'c', 8}});
        update_flag_in_file(filename, static_cast<uint32_t>(index), static_cast<uint32_t>(depth));
    }

    ofstream file(output_txt_filename);
    if (!file) {
        log_message("❌ Failed to open file: " + output_txt_filename);
        return;
    }

    for (const auto &state : current_depth_states) {
        file << state << endl;
    }

    file.close();
    log_message("📁 Depth " + to_string(depth) + " states saved to " + output_txt_filename);
}


// 
// Cubestate-changing functions
//


void cycle(char& a, char& b, char& c, char& d) {
    char temp = d;
    d = c;
    c = b;
    b = a;
    a = temp;
}

void swap(char& a, char& b) {
    char temp = a;
    a = b;
    b = temp;
}

// Function to apply a move sequence to a cube state
string apply_move(const string& state, const string& move) {
    //cout << "Applying move: " << move << endl;
    if (move == "R") return R(state);
    if (move == "R2") return R2(state);
    if (move == "R'") return R3(state);
    if (move == "L") return L(state);
    if (move == "L2") return L2(state);
    if (move == "L'") return L3(state);
    if (move == "F") return F(state);
    if (move == "F2") return F2(state);
    if (move == "F'") return F3(state);
    if (move == "B") return B(state);
    if (move == "B2") return B2(state);
    if (move == "B'") return B3(state);
    if (move == "U") return U(state);
    if (move == "U2") return U2(state);
    if (move == "U'") return U3(state);
    if (move == "Uw") return u(state);
    if (move == "Uw2") return u2(state);
    if (move == "Uw'") return u3(state);
    if (move == "D") return D(state);
    if (move == "D2") return D2(state);
    if (move == "D'") return D3(state);
    if (move == "Dw") return d(state);
    if (move == "Dw2") return d2(state);
    if (move == "Dw'") return d3(state);
    if (move == "Rw") return r(state);
    if (move == "Rw2") return r2(state);
    if (move == "Rw'") return r3(state);
    if (move == "Lw") return l(state);
    if (move == "Lw2") return l2(state);
    if (move == "Lw'") return l3(state);
    if (move == "Fw") return f(state);
    if (move == "Fw2") return f2(state);
    if (move == "Fw'") return f3(state);
    if (move == "Bw") return b(state);
    if (move == "Bw2") return b2(state);
    if (move == "Bw'") return b3(state);

    if (move == "y") return y_rotation(state);
    if (move == "y2") return y2_rotation(state);
    if (move == "y'") return y3_rotation(state);
    if (move == "x") return x_rotation(state);
    if (move == "x2") return x2_rotation(state);
    if (move == "x'") return x3_rotation(state);
    if (move == "z") return z_rotation(state);
    if (move == "z2") return z2_rotation(state);
    if (move == "z'") return z3_rotation(state);
    return state;  // Return unchanged state if move is not recognized
}

// Function to translate a scramble into a sequence of moves applied to the cube state
string translate_scramble_to_moves(const string& scramble) {
    stringstream ss(scramble);
    string move;
    string current_state = "aaaabbbbccccbbbbccccaaaa";  // HTR-solved cube state

    while (ss >> move) {
        current_state = apply_move(current_state, move);
    }
    return current_state;
}

// Function to fix letter order, very important function
string fixLetterOrder(const string& combination) {
    string combination_updated = combination;
    if (combination_updated[0] != 'a') {
        combination_updated = letter_swap(combination_updated, combination_updated[0], 'a');
    }
    for (size_t i = 0; i < combination_updated.size(); ++i) {
        if (combination_updated[i] != 'a') {
            if (combination_updated[i] == 'c') {
                combination_updated = letter_swap(combination_updated, 'c', 'b');
            }
            break;
        }
    }
    return combination_updated;
}

// Function to swap letters
string letter_swap(string combination, char from, char to) {
    string combination_new = "";
    for (char ch : combination) {
        if (ch == from) combination_new += to;
        else if (ch == to) combination_new += from;
        else combination_new += ch;
    }
    return combination_new;
}


// Perform an "x-rotation" on the combination
string x_rotation(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[4], combination_new[7], combination_new[6], combination_new[5]);    // Cycle 4 -> 7 -> 6 -> 5 -> 4
    cycle(combination_new[12], combination_new[13], combination_new[14], combination_new[15]);    // Cycle 12 -> 13 -> 14 -> 15 -> 12
    cycle(combination_new[0], combination_new[18], combination_new[20], combination_new[8]);    // Cycle 0 -> 18 -> 20 -> 8 -> 0
    cycle(combination_new[1], combination_new[19], combination_new[21], combination_new[9]);    // Cycle 1 -> 19 -> 21 -> 9 -> 1
    cycle(combination_new[2], combination_new[16], combination_new[22], combination_new[10]);    // Cycle 2 -> 16 -> 22 -> 10 -> 2
    cycle(combination_new[3], combination_new[17], combination_new[23], combination_new[11]);    // Cycle 3 -> 17 -> 23 -> 11 -> 3
    return combination_new;
}

string x2_rotation(const string& combination) {string combination_new = x_rotation(combination);  return x_rotation(combination_new);}
string x3_rotation(const string& combination) {return x_rotation(x2_rotation(combination));}

// Perform a "y-rotation" on the combination
string y_rotation(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[0], combination_new[1], combination_new[2], combination_new[3]);    // Cycle 0 -> 1 -> 2 -> 3 -> 0
    cycle(combination_new[4], combination_new[16], combination_new[12], combination_new[8]);    // Cycle 4 -> 16 -> 12 -> 8 -> 4
    cycle(combination_new[5], combination_new[17], combination_new[13], combination_new[9]);    // Cycle 5 -> 17 -> 13 -> 9 -> 5
    cycle(combination_new[6], combination_new[18], combination_new[14], combination_new[10]);    // Cycle 6 -> 18 -> 14 -> 10 -> 6
    cycle(combination_new[7], combination_new[19], combination_new[15], combination_new[11]);    // Cycle 7 -> 19 -> 15 -> 11 -> 7
    cycle(combination_new[20], combination_new[23], combination_new[22], combination_new[21]);    // Cycle 20 -> 23 -> 22 -> 21 -> 20
    return combination_new;

}
string y2_rotation(const string& combination) {string combination_new = y_rotation(combination);  return y_rotation(combination_new);}
string y3_rotation(const string& combination) {string combination_new = y2_rotation(combination); return y_rotation(combination_new);}


// Perform a "z-rotation" on the combination
string z_rotation(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[0], combination_new[13], combination_new[22], combination_new[7]);    // Cycle 0 -> 13 -> 22 -> 7 -> 0
    cycle(combination_new[1], combination_new[14], combination_new[23], combination_new[4]);    // Cycle 1 -> 14 -> 23 -> 4 -> 1
    cycle(combination_new[2], combination_new[15], combination_new[20], combination_new[5]);    // Cycle 2 -> 15 -> 20 -> 5 -> 2
    cycle(combination_new[3], combination_new[12], combination_new[21], combination_new[6]);    // Cycle 3 -> 12 -> 21 -> 6 -> 3
    cycle(combination_new[8], combination_new[9], combination_new[10], combination_new[11]);    // Cycle 8 -> 9 -> 10 -> 11 -> 8
    cycle(combination_new[16], combination_new[19], combination_new[18], combination_new[17]);  // Cycle 16 -> 19 -> 18 -> 17 -> 16
    return combination_new;
}
string z2_rotation(const string& combination) {string combination_new = z_rotation(combination);return z_rotation(combination_new);}
string z3_rotation(const string& combination) {return z_rotation(z2_rotation(combination));}

// I did introduce mirrors here, but they are only used in my version 1 solver. Here, I made some experiments and while adding them reduces the states in each depth
// by 10% (not scaling exponentially sadly), it is also 4x-ing the amount of time needed to update the lookup-table. I will not use mirror-symetry as of right now.

string m_mirror(const string& combination) {
    string combination_new = combination;

    // 0 1, 2 3, 4 13, 5 12, 6 15, 7 14, 8 9, 10 11, 16 17, 18 19 + D-layer swaps
    swap(combination_new[0], combination_new[1]);
    swap(combination_new[2], combination_new[3]);
    swap(combination_new[4], combination_new[13]);
    swap(combination_new[5], combination_new[12]);
    swap(combination_new[6], combination_new[15]);
    swap(combination_new[7], combination_new[14]);
    swap(combination_new[8], combination_new[9]);
    swap(combination_new[10], combination_new[11]);
    swap(combination_new[16], combination_new[17]);
    swap(combination_new[18], combination_new[19]);
    swap(combination_new[20], combination_new[21]);
    swap(combination_new[22], combination_new[23]);
    return combination_new;
}

string s_mirror(const string& combination) {
    string combination_new = combination;

    // 0 3, 1 2, 4 5, 6 7, 8 17, 9 16, 10 19, 11 18, 12 13, 14 15 + D-layer swaps
    swap(combination_new[0], combination_new[3]);
    swap(combination_new[1], combination_new[2]);
    swap(combination_new[4], combination_new[5]);
    swap(combination_new[6], combination_new[7]);
    swap(combination_new[8], combination_new[17]);
    swap(combination_new[9], combination_new[16]);
    swap(combination_new[10], combination_new[19]);
    swap(combination_new[11], combination_new[18]);
    swap(combination_new[12], combination_new[13]);
    swap(combination_new[14], combination_new[15]);
    swap(combination_new[20], combination_new[23]);
    swap(combination_new[21], combination_new[22]);
    return combination_new;

}

string e_mirror(const string& combination) {
    string combination_new = combination;

    //  0 23, 1 22, 2 21, 3 20, 4 7, 5 6, 8 11, 9 10, 12 15, 13 14, 16 19, 17 18 + D-layer swaps
    swap(combination_new[0], combination_new[23]);
    swap(combination_new[1], combination_new[22]);
    swap(combination_new[2], combination_new[21]);
    swap(combination_new[3], combination_new[20]);
    swap(combination_new[4], combination_new[7]);
    swap(combination_new[5], combination_new[6]);
    swap(combination_new[8], combination_new[11]);
    swap(combination_new[9], combination_new[10]);
    swap(combination_new[12], combination_new[15]);
    swap(combination_new[13], combination_new[14]);
    swap(combination_new[16], combination_new[19]);
    swap(combination_new[17], combination_new[18]);
    return combination_new;

}

string R(const string& combination) {
    string combination_new = combination; // Cycle 12 -> 13 -> 14 -> 15 -> 12
    cycle(combination_new[12], combination_new[13], combination_new[14], combination_new[15]);
    return combination_new;
}

string R2(const string& combination) {string combination_new = R(combination);  return R(combination_new);}
string R3(const string& combination) {return R(R2(combination));}

//note that uncapitalized r is not a slice move, but equivalent to Rw in some literature. This applies to all uncapiatalized moves

string r(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[12], combination_new[13], combination_new[14], combination_new[15]);
    cycle(combination_new[1], combination_new[19], combination_new[21], combination_new[9]);
    cycle(combination_new[2], combination_new[16], combination_new[22], combination_new[10]);
    return combination_new;
}

string r2(const string& combination) {string combination_new = r(combination); return r(combination_new);}
string r3(const string& combination) {return r(r2(combination));}

string L(const string& combination) {
    string combination_new = combination;     // Cycle 4 -> 5 -> 6 -> 7 -> 4
    cycle(combination_new[4], combination_new[5], combination_new[6], combination_new[7]);
    return combination_new;
}

string L2(const string& combination) {
    string combination_new = L(combination);  
    return L(combination_new);               
}

string L3(const string& combination) {return L(L2(combination));}

string l(const string& combination) {
    string combination_new = r(combination);
    return x3_rotation(combination_new);           
}

string l2(const string& combination) {
    string combination_new = r2(combination);
    return x2_rotation(combination_new);           
}

string l3(const string& combination) {
    string combination_new = r3(combination);
    return x_rotation(combination_new);           
}

string F(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[8], combination_new[9], combination_new[10], combination_new[11]);    // Cycle 8 -> 9 -> 10 -> 11 -> 8
    return combination_new;
}

string F2(const string& combination) {
    string combination_new = F(combination);
    return F(combination_new);               
}

string F3(const string& combination) { 
    return F(F2(combination));
}

string f(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[2], combination_new[15], combination_new[20], combination_new[5]);    // Cycle 2 -> 15 -> 20 -> 5 -> 2
    cycle(combination_new[3], combination_new[12], combination_new[21], combination_new[6]);    // Cycle 3 -> 12 -> 21 -> 6 -> 3  
    cycle(combination_new[8], combination_new[9], combination_new[10], combination_new[11]);    // Cycle 8 -> 9 -> 10 -> 11 -> 8
    return combination_new;
}
        
string f2(const string& combination) {string combination_new = f(combination); return f(combination_new);}
string f3(const string& combination) {return f(f2(combination));}

string B(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[16], combination_new[17], combination_new[18], combination_new[19]);     // Cycle 16 -> 17 -> 18 -> 19 -> 16
    return combination_new;
}

string B2(const string& combination) {
    string combination_new = B(combination); 
    return B(combination_new);               
}

string B3(const string& combination) { 
    return B(B2(combination));
}

string b(const string& combination) {
    string combination_new = f(combination);
    return z3_rotation(combination_new);           
}

string b2(const string& combination) {
    string combination_new = f2(combination);
    return z2_rotation(combination_new);           
}

string b3(const string& combination) {
    string combination_new = f3(combination);
    return z_rotation(combination_new);           
}


string U(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[0], combination_new[1], combination_new[2], combination_new[3]);     // Cycle 0 -> 1 -> 2 -> 3 -> 0
    return combination_new;
}

string U2(const string& combination) {
    string combination_new = U(combination); 
    return U(combination_new);               
}

string U3(const string& combination) {
    return U(U2(combination));
}

string u(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[0], combination_new[1], combination_new[2], combination_new[3]);     // Cycle 0 -> 1 -> 2 -> 3 -> 0
    cycle(combination_new[4], combination_new[16], combination_new[12], combination_new[8]);    // Cycle 4 -> 16 -> 12 -> 8 -> 4
    cycle(combination_new[5], combination_new[17], combination_new[13], combination_new[9]);    // Cycle 5 -> 17 -> 13 -> 9 -> 5
    return combination_new;
}

string u2(const string& combination) {
    string combination_new = u(combination);  // First transformation
    return u(combination_new);               // Apply u again and return
}

string u3(const string& combination) {
    return u(u2(combination));
}

string D(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[20], combination_new[21], combination_new[22], combination_new[23]);     // Cycle 20 -> 21 -> 22 -> 23 -> 20
    return combination_new;
}

string D2(const string& combination) {
    string combination_new = D(combination);  // First transformation
    return D(combination_new);               // Apply D again and return
}

string D3(const string& combination) { // Perform D3 (D applied three times)
    return D(D2(combination));
}

string d(const string& combination) {
    string combination_new = u(combination);
    return y3_rotation(combination_new);           
}

string d2(const string& combination) {
    string combination_new = u2(combination);
    return y2_rotation(combination_new);           
}

string d3(const string& combination) {
    string combination_new = u3(combination);
    return y_rotation(combination_new);           
}


// Function to generate move-group restrictions. This was used in the version_1 solver to not perform move-sequences like "R R' R2". I could also add
// this kind of filtering in this version_2 solver
map<string, vector<string>> get_move_groups() {
    return {
        {"R", {"R", "R2", "R'"}}, {"R2", {"R", "R2", "R'"}}, {"R'", {"R", "R2", "R'"}},
        {"Rw", {"Rw", "Rw2", "Rw'"}}, {"Rw2", {"Rw", "Rw2", "Rw'"}}, {"Rw'", {"Rw", "Rw2", "Rw'"}},
        {"L", {"L", "L2", "L'"}}, {"L2", {"L", "L2", "L'"}}, {"L'", {"L", "L2", "L'"}},
        {"Lw", {"Lw", "Lw2", "Lw'"}}, {"Lw2", {"Lw", "Lw2", "Lw'"}}, {"Lw'", {"Lw", "Lw2", "Lw'"}},
        {"F", {"F", "F2", "F'"}}, {"F2", {"F", "F2", "F'"}}, {"F'", {"F", "F2", "F'"}},
        {"Fw", {"Fw", "Fw2", "Fw'"}}, {"Fw2", {"Fw", "Fw2", "Fw'"}}, {"Fw'", {"Fw", "Fw2", "Fw'"}},
        {"B", {"B", "B2", "B'"}}, {"B2", {"B", "B2", "B'"}}, {"B'", {"B", "B2", "B'"}},
        {"Bw", {"Bw", "Bw2", "Bw'"}}, {"Bw2", {"Bw", "Bw2", "Bw'"}}, {"Bw'", {"Bw", "Bw2", "Bw'"}},
        {"U", {"U", "U2", "U'"}}, {"U2", {"U", "U2", "U'"}}, {"U'", {"U", "U2", "U'"}},
        {"Uw", {"Uw", "Uw2", "Uw'"}}, {"Uw2", {"Uw", "Uw2", "Uw'"}}, {"Uw'", {"Uw", "Uw2", "Uw'"}},
        {"D", {"D", "D2", "D'"}}, {"D2", {"D", "D2", "D'"}}, {"D'", {"D", "D2", "D'"}},
        {"Dw", {"Dw", "Dw2", "Dw'"}}, {"Dw2", {"Dw", "Dw2", "Dw'"}}, {"Dw'", {"Dw", "Dw2", "Dw'"}}
    };
}


string get_canonical_rotation(const string &cubestate) {
    vector<string> rotations = {"x", "x2", "x'", "y", "y2", "y'", "z", "z2", "z'"};
    string min_state = fixLetterOrder(cubestate); // Start with the fixed original state

    for (const string &rotation : rotations) {
        string rotated_state = apply_move(cubestate, rotation);
        string fixed_rotated_state = fixLetterOrder(rotated_state);

        if (fixed_rotated_state < min_state) {
            min_state = fixed_rotated_state;
        }
    }
    return min_state;
}

// Returns the lexicographically smallest rotated version of a cubestate along with the applied rotation
pair<string, string> get_canonical_rotation_with_rotation(const string &cubestate) {
    vector<string> rotations = {"x", "x2", "x'", "y", "y2", "y'", "z", "z2", "z'"};
    string best_state = fixLetterOrder(cubestate); // Start with the fixed original state
    string best_rotation = "";

    for (const string &rotation : rotations) {
        string rotated_state = apply_move(cubestate, rotation);
        string fixed_rotated_state = fixLetterOrder(rotated_state);

        if (fixed_rotated_state < best_state) {
            best_state = fixed_rotated_state;
            best_rotation = rotation;
        }
    }

    return {best_state, best_rotation};
}


bool recursive_expand_detailed(vector<tuple<string, vector<string>, vector<string>>> &parent_states, 
                      const string &filename, uint32_t current_flag, 
                      map<char, int> &letter_counts, int depth, 
                      vector<vector<string>> &successful_paths) {
    
    if (current_flag == 0) {
        log_message("\n🎯 Reached depth 0. ✅ Solution found!");
        return true;
    }

const vector<string>& moves = MOVES;

    log_message("\n\n🌳 [Depth " + to_string(depth) + "] Expanding with Flag = " + to_string(current_flag));
    log_message("🌿 Number of parent states: " + to_string(parent_states.size()));

    uint32_t new_min_flag = UINT32_MAX;
    vector<tuple<string, vector<string>, vector<string>>> good_paths_next;

    int parent_count = 0;
    for (const auto &[parent_state, history, rotation_history] : parent_states) {
        ++parent_count;
        log_message("\n🌲 Parent #" + to_string(parent_count) + ": " + parent_state);
        log_message("  ├─ Path so far: " + (history.empty() ? "(none)" : history.back()));
        log_message("  └─ Rotations: " + (rotation_history.empty() ? "(none)" : rotation_history.back()));

        int move_index = 0;
        for (const string &move : MOVES) {
            ++move_index;
            log_message("    ├─ 🔀 Move #" + to_string(move_index) + ": " + move);

            string new_state = "a" + parent_state;
            new_state = apply_move(new_state, move);
            log_message("    │   ├─ State after move: " + new_state);

            auto [canonical_state, applied_rotation] = get_canonical_rotation_with_rotation(new_state);
            string fixed_state = canonical_state.substr(1);

            log_message("    │   ├─ Canonical: " + canonical_state);
            log_message("    │   ├─ Applied Rotation: " + applied_rotation);
            log_message("    │   └─ Final fixed state: " + fixed_state);

            __uint128_t index = multinomial_rank(fixed_state, letter_counts);

            uint32_t flag = 0;
            ifstream input_file(filename, ios::binary);
            if (!input_file) {
                log_message("❌ Error opening file: " + filename);
                return false;
            }

            streampos file_position = index * sizeof(uint32_t);
            input_file.seekg(file_position, ios::beg);
            input_file.read(reinterpret_cast<char*>(&flag), sizeof(uint32_t));
            input_file.close();

            uint32_t extracted_flag = flag & 0xF;
            log_message("    │   📦 Flag from file: " + to_string(extracted_flag));

            if (extracted_flag < new_min_flag) {
                log_message("    │   🆕 New minimum flag found: " + to_string(extracted_flag));
                new_min_flag = extracted_flag;
                good_paths_next.clear();
            }

            if (extracted_flag == new_min_flag) {
                log_message("    │   ✅ Matching min flag, storing path.");

                vector<string> new_history = history;
                new_history.push_back(move);

                vector<string> new_rotation_history = rotation_history;
                new_rotation_history.push_back(applied_rotation);

                good_paths_next.push_back({fixed_state, new_history, new_rotation_history});
            }

            if (extracted_flag == 0) {
                log_message("    🎉 FOUND GOAL STATE! Building solution path...");

                vector<string> new_history = history;
                new_history.push_back(move);

                vector<string> new_rotation_history = rotation_history;
                new_rotation_history.push_back(applied_rotation);

                vector<string> solution_path;
                for (size_t i = 0; i < new_history.size(); ++i) {
                    solution_path.push_back(new_history[i] + " → " + new_rotation_history[i]);
                }

                successful_paths.push_back(solution_path);

                string final_solution;
                for (size_t i = 0; i < new_history.size(); ++i) {
                    final_solution += new_rotation_history[i] + " " + new_history[i] + " ";
                }
                final_solution.pop_back();
                final_solution += " (depth=" + to_string(new_history.size()) + ")";

                log_message("    ✅ Final Solution: " + final_solution);
            }
        }
    }

    log_message("\n🔁 Recursing to next depth with min flag: " + to_string(new_min_flag));
    return recursive_expand_detailed(good_paths_next, filename, new_min_flag, letter_counts, depth + 1, successful_paths);
}


void solve_scramble_detailed(const string &scramble, const string &filename, bool print_only_first_solution = true) {
    log_message("\n🔍 Solving scramble: " + scramble);

    string solved_state = "aaaabbbbccccbbbbccccaaaa";
    string scrambled_state = translate_scramble_to_moves(scramble);
    log_message("\n🔄 Scrambled State: " + scrambled_state);

    vector<string> rotations = {"x", "x2", "x'", "y", "y2", "y'", "z", "z2", "z'"};
    vector<tuple<string, string, string>> all_rotations_for_this_scramble;

    for (const string &rotation : rotations) {
        string rotated_state = apply_move(scrambled_state, rotation);
        string fixed_rotated_state = fixLetterOrder(rotated_state);
        all_rotations_for_this_scramble.push_back({rotation, rotated_state, fixed_rotated_state});
    }

    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};
    uint32_t min_flag = UINT32_MAX;
    vector<tuple<string, vector<string>, vector<string>>> good_paths;

    for (const auto &[rotation, state, fixed_state] : all_rotations_for_this_scramble) {
        string canonical_state = fixed_state.substr(1);
        __uint128_t index = multinomial_rank(canonical_state, letter_counts);

        uint32_t flag = 0;
        ifstream input_file(filename, ios::binary);
        if (!input_file) {
            log_message("❌ Error opening file: " + filename);
            return;
        }

        streampos file_position = index * sizeof(uint32_t);
        input_file.seekg(file_position, ios::beg);
        input_file.read(reinterpret_cast<char*>(&flag), sizeof(uint32_t));
        input_file.close();

        uint32_t extracted_flag = flag & 0xF;

        if (extracted_flag < min_flag) {
            min_flag = extracted_flag;
            good_paths.clear();
        }
        if (extracted_flag == min_flag) {
            good_paths.push_back({canonical_state, {rotation}, {rotation}});
        }
    }

    vector<vector<string>> successful_paths;
    recursive_expand_detailed(good_paths, filename, min_flag, letter_counts, 1, successful_paths);

    log_message("\n🏆 SUCCESSFUL SOLUTIONS:");
    bool first_solution_logged = false;

    for (const auto &path : successful_paths) {
        string formatted_solution = "";
        bool is_first_entry = true;

        string initial_rotation = path[0].substr(0, 2);
        formatted_solution = initial_rotation;

        for (const auto &step : path) {
            size_t arrow_pos = step.find(" → ");
            if (arrow_pos != string::npos) {
                string move = step.substr(0, arrow_pos);
                string rotation = step.substr(arrow_pos + 4);

                if (is_first_entry) {
                    is_first_entry = false;
                } else {
                    formatted_solution += move + " " + rotation + " ";
                }
            }
        }

        if (!formatted_solution.empty()) {
            formatted_solution.pop_back();
        }

        if (!print_only_first_solution || !first_solution_logged) {
            log_message("Final Solution: " + formatted_solution);
        }

        if (!first_solution_logged) {
            first_solution_logged = true;

            // Count non-rotation moves
            istringstream iss(formatted_solution);
            int move_count = 0;
            string token;
            vector<string> rotation_prefixes = {"x", "x2", "x'", "y", "y2", "y'", "z", "z2", "z'"};

            while (iss >> token) {
                if (find(rotation_prefixes.begin(), rotation_prefixes.end(), token) != rotation_prefixes.end()) {
                    continue;
                }
                move_count++;
            }

            if (move_count >= MAX_MOVES) move_count = MAX_MOVES - 1;
            move_histogram[move_count]++;
        }

        if (print_only_first_solution) {
            break; // ✅ Stop after printing the first solution
        }
    }
}

bool recursive_expand_fast(
    vector<tuple<string, vector<string>, vector<string>>> &parent_states,
    const string &filename,
    uint32_t current_flag,
    map<char, int> &letter_counts,
    int depth,
    vector<vector<string>> &successful_paths,
    bool only_first_solution,
    unordered_set<string> &visited_states
) {
    if (current_flag == 0) {
        log_message("\n🎯 Reached depth 0. ✅ Solution found!");
        return true;
    }

    uint32_t new_min_flag = UINT32_MAX;
    vector<tuple<string, vector<string>, vector<string>>> good_paths_next;

    for (const auto &[parent_state, history, rotation_history] : parent_states) {
        for (const string &move : MOVES) {
            string new_state = "a" + parent_state;
            new_state = apply_move(new_state, move);

            auto [canonical_state, applied_rotation] = get_canonical_rotation_with_rotation(new_state);
            string fixed_state = canonical_state.substr(1);

            if (visited_states.count(fixed_state)) continue; // what makes the "fast"-version different from full-branch version
            visited_states.insert(fixed_state);

            __uint128_t index = multinomial_rank(fixed_state, letter_counts);

            uint32_t flag = 0;
            ifstream input_file(filename, ios::binary);
            if (!input_file) {
                log_message("❌ Error opening file: " + filename);
                return false;
            }

            streampos file_position = index * sizeof(uint32_t);
            input_file.seekg(file_position, ios::beg);
            input_file.read(reinterpret_cast<char*>(&flag), sizeof(uint32_t));
            input_file.close();

            uint32_t extracted_flag = flag & 0xF;

            if (current_flag <= 8 && extracted_flag >= current_flag) { // might go higher here
                continue; // ✂️ Enforce strictly decreasing rule
            }

            if (extracted_flag < new_min_flag) {
                new_min_flag = extracted_flag;
                good_paths_next.clear();
            }

            if (extracted_flag == new_min_flag) {
                vector<string> new_history = history;
                new_history.push_back(move);

                vector<string> new_rotation_history = rotation_history;
                new_rotation_history.push_back(applied_rotation);

                good_paths_next.push_back({fixed_state, new_history, new_rotation_history});

                if (extracted_flag == 0) {
                    vector<string> solution_path;
                    for (size_t i = 0; i < new_history.size(); ++i) {
                        solution_path.push_back(new_history[i] + " → " + new_rotation_history[i]);
                    }
                    successful_paths.push_back(solution_path);

                    if (only_first_solution) return true;
                }
            }
        }
    }

    //log_message("\n🔁 Recursing to next depth with min flag: " + to_string(new_min_flag));

    if (good_paths_next.empty()) {
        log_message("⛔ No valid path forward (strict depth rule). Scramble will be skipped.");        
        return false;
    }

    if (only_first_solution && new_min_flag != 15 && !good_paths_next.empty()) {
        vector<tuple<string, vector<string>, vector<string>>> single_path = { good_paths_next[0] };
        return recursive_expand_fast(single_path, filename, new_min_flag, letter_counts, depth + 1, successful_paths, true, visited_states);
    }

    return recursive_expand_fast(good_paths_next, filename, new_min_flag, letter_counts, depth + 1, successful_paths, only_first_solution, visited_states);
}


void solve_scramble_fast(const string &scramble, const string &filename, bool print_only_first_solution = true) {

    log_message("\n🔍 Solving scramble: " + scramble);

    string scrambled_state = translate_scramble_to_moves(scramble);
    log_message("\n🔄 Scrambled State: " + scrambled_state);

    
    vector<string> rotations = {"x", "x2", "x'", "y", "y2", "y'", "z", "z2", "z'"};
    vector<tuple<string, string, string>> all_rotations_for_this_scramble;

    for (const string &rotation : rotations) {
        string rotated_state = apply_move(scrambled_state, rotation);
        string fixed_rotated_state = fixLetterOrder(rotated_state);
        all_rotations_for_this_scramble.push_back({rotation, rotated_state, fixed_rotated_state});
    }

    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};
    uint32_t min_flag = UINT32_MAX;
    vector<tuple<string, vector<string>, vector<string>>> good_paths;

    for (const auto &[rotation, state, fixed_state] : all_rotations_for_this_scramble) {
        string canonical_state = fixed_state.substr(1);
        __uint128_t index = multinomial_rank(canonical_state, letter_counts);

        uint32_t flag = 0;
        ifstream input_file(filename, ios::binary);
        if (!input_file) {
            log_message("❌ Error opening file: " + filename);
            return;
        }

        streampos file_position = index * sizeof(uint32_t);
        input_file.seekg(file_position, ios::beg);
        input_file.read(reinterpret_cast<char*>(&flag), sizeof(uint32_t));
        input_file.close();

        uint32_t extracted_flag = flag & 0xF;

        if (extracted_flag < min_flag) {
            min_flag = extracted_flag;
            good_paths.clear();
        }
        if (extracted_flag == min_flag) {
            good_paths.push_back({canonical_state, {rotation}, {rotation}});
        }
    }

    vector<vector<string>> successful_paths;
    unordered_set<string> visited_states;
    bool success = recursive_expand_fast(good_paths, filename, min_flag, letter_counts, 1, successful_paths, print_only_first_solution, visited_states);

    if (!success) {
        // ❌ Write failing scramble to scramble2.txt
        ofstream fail_file("scrambles_dropout.txt", ios::app);
        if (fail_file) {
            fail_file << scramble << "\n";
        } else {
            log_message("⚠️ Could not write to scrambles_dropout.txt");
        }
        return; // Stop here since no path was found
    }

    log_message("\n🏆 SUCCESSFUL SOLUTIONS:");

    // All of the following is just for logging:
    bool first_solution_logged = false;

    for (const auto &path : successful_paths) {
        string formatted_solution = "";
        bool is_first_entry = true;
        string initial_rotation = path[0].substr(0, 2);
        formatted_solution = initial_rotation;

        for (const auto &step : path) {
            size_t arrow_pos = step.find(" → ");
            if (arrow_pos != string::npos) {
                string move = step.substr(0, arrow_pos);
                string rotation = step.substr(arrow_pos + 4);
                if (is_first_entry) {
                    is_first_entry = false;
                } else {
                    formatted_solution += move + " " + rotation + " ";
                }
            }
        }

        if (!formatted_solution.empty()) {
            formatted_solution.pop_back();
        }

        if (!print_only_first_solution || !first_solution_logged) {
            log_message("Final Solution: " + formatted_solution);
        }

        if (!first_solution_logged) {
            first_solution_logged = true;

            // Count non-rotation moves
            istringstream iss(formatted_solution);
            int move_count = 0;
            string token;
            vector<string> rotation_prefixes = {"x", "x2", "x'", "y", "y2", "y'", "z", "z2", "z'"};

            while (iss >> token) {
                if (find(rotation_prefixes.begin(), rotation_prefixes.end(), token) != rotation_prefixes.end()) {
                    continue;
                }
                move_count++;
            }

            if (move_count >= MAX_MOVES) move_count = MAX_MOVES - 1;
            move_histogram[move_count]++;
            log_message("✅ Move Count (no rotations): " + to_string(move_count));
        }

        if (print_only_first_solution) break;
    }
}


void print_move_histogram() {
    log_message("\n📊 Move Count Histogram:");
    for (int i = 0; i < MAX_MOVES; ++i) {
        if (move_histogram[i] > 0) {
            cout << "Moves: " << i << " => " << move_histogram[i] << " times\n";
        }
    }
}


void solve_scrambles_detailed(const string &scramble_list_file, const string &binary_filename) {
    ifstream infile(scramble_list_file);
    if (!infile) {
        cerr << "❌ Error opening scramble list file: " << scramble_list_file << endl;
        return;
    }

    string line;
    int scramble_count = 0;

    while (getline(infile, line)) {
        if (line.empty()) continue;  // Skip blank lines

        scramble_count++;
        log_message("\n\n==========🧩 Solving scramble #" + to_string(scramble_count) + ": " + line + "==========\n");
        solve_scramble_detailed(line, binary_filename);
    }
    infile.close();
    log_message("\n✅ Finished solving " + to_string(scramble_count) + " scrambles.");
    print_move_histogram();
}


void solve_scrambles_fast(const string &scramble_list_file, const string &binary_filename) {
    ifstream infile(scramble_list_file);
    if (!infile) {
        cerr << "❌ Error opening scramble list file: " << scramble_list_file << endl;
        return;
    }

    string line;
    int scramble_count = 0;

    while (getline(infile, line)) {
        if (line.empty()) continue;  // Skip blank lines

        scramble_count++;
        log_message("\n\n=======================");
        log_message("🧩 Solving scramble #" + to_string(scramble_count) + ": " + line);
        log_message("=======================\n");

        solve_scramble_fast(line, binary_filename, false);
    }

    infile.close();
    log_message("\n✅ Finished solving " + to_string(scramble_count) + " scrambles.");
    print_move_histogram();
}

// Function to generate random state strings of 8x a/b/c
void random_state_string_generator(const string& output_filename, int count = 1) {
    ofstream outfile(output_filename);
    if (!outfile) {
        cerr << "❌ Error opening output file: " << output_filename << endl;
        return;
    }

    string base = "aaaaaaaabbbbbbbbcccccccc";
    random_device rd;
    mt19937 gen(rd());

    for (int i = 0; i < count; ++i) {
        string shuffled = base;
        shuffle(shuffled.begin(), shuffled.end(), gen);
        outfile << shuffled << "\n";
    }

    outfile.close();
    cout << "✅ " << count << " random states written to " << output_filename << endl;
}




void solve_state_string_fast(const string &state_string, const string &filename, bool print_only_first_solution = true) {
    log_message("\n🔍 Solving raw state string: " + state_string);

    // Prepend 'a' and begin rotations
    string scrambled_state = "a" + state_string;
    vector<string> rotations = {"x", "x2", "x'", "y", "y2", "y'", "z", "z2", "z'"};
    vector<tuple<string, string, string>> all_rotations_for_this_state;

    for (const string &rotation : rotations) {
        string rotated_state = apply_move(scrambled_state, rotation);
        string fixed_rotated_state = fixLetterOrder(rotated_state);
        all_rotations_for_this_state.push_back({rotation, rotated_state, fixed_rotated_state});
    }

    map<char, int> letter_counts = {{'a', 8}, {'b', 8}, {'c', 8}};
    uint32_t min_flag = UINT32_MAX;
    vector<tuple<string, vector<string>, vector<string>>> good_paths;

    for (const auto &[rotation, state, fixed_state] : all_rotations_for_this_state) {
        string canonical_state = fixed_state.substr(1);
        __uint128_t index = multinomial_rank(canonical_state, letter_counts);

        uint32_t flag = 0;
        ifstream input_file(filename, ios::binary);
        if (!input_file) {
            log_message("❌ Error opening file: " + filename);
            return;
        }

        streampos file_position = index * sizeof(uint32_t);
        input_file.seekg(file_position, ios::beg);
        input_file.read(reinterpret_cast<char*>(&flag), sizeof(uint32_t));
        input_file.close();

        uint32_t extracted_flag = flag & 0xF;

        if (extracted_flag < min_flag) {
            min_flag = extracted_flag;
            good_paths.clear();
        }
        if (extracted_flag == min_flag) {
            good_paths.push_back({canonical_state, {rotation}, {rotation}});
        }
    }

    vector<vector<string>> successful_paths;
    unordered_set<string> visited_states;
        
if (min_flag == 0) {
    log_message("\n🎯 Reached depth 0. ✅ Already solved!");

    for (const auto &[canonical_state, history, rotation_history] : good_paths) {
        vector<string> solution_path;
        for (size_t i = 0; i < history.size(); ++i) {
            solution_path.push_back(history[i] + " → " + rotation_history[i]);
        }
        successful_paths.push_back(solution_path);
    }
}
else {
    bool success = recursive_expand_fast(good_paths, filename, min_flag, letter_counts, 1, successful_paths, print_only_first_solution, visited_states);
}

    log_message("\n🏆 SUCCESSFUL SOLUTIONS:");
    bool first_solution_logged = false;

    for (const auto &path : successful_paths) {
        string formatted_solution = "";
        bool is_first_entry = true;
        string initial_rotation = path[0].substr(0, 2);
        formatted_solution = initial_rotation;

        for (const auto &step : path) {
            size_t arrow_pos = step.find(" → ");
            if (arrow_pos != string::npos) {
                string move = step.substr(0, arrow_pos);
                string rotation = step.substr(arrow_pos + 4);
                if (is_first_entry) {
                    is_first_entry = false;
                } else {
                    formatted_solution += move + " " + rotation + " ";
                }
            }
        }

        if (!formatted_solution.empty()) {
            formatted_solution.pop_back();
        }

        if (!print_only_first_solution || !first_solution_logged) {
            log_message("Final Solution: " + formatted_solution);
        }

        if (!first_solution_logged) {
            first_solution_logged = true;

            // Count non-rotation moves
            istringstream iss(formatted_solution);
            int move_count = 0;
            string token;
            vector<string> rotation_prefixes = {"x", "x2", "x'", "y", "y2", "y'", "z", "z2", "z'"};

            while (iss >> token) {
                if (find(rotation_prefixes.begin(), rotation_prefixes.end(), token) != rotation_prefixes.end()) continue;
                move_count++;
            }

            if (move_count >= MAX_MOVES) move_count = MAX_MOVES - 1;
            move_histogram[move_count]++;
            log_message("✅ Move Count (no rotations): " + to_string(move_count));
        }

        if (print_only_first_solution) break;
    }
}


// Main function
int main() {
   auto start_time = high_resolution_clock::now();
   precompute_factorials();

    string bin_filename = "2100mio_d9.bin";
    // 1)

    // Uncomment this until "depth_7_updater" for updating the bin-file with the depths. You can also uncomment depth_8 if you PC is fast. This depth takes
    // about 40mins to run on my Macbook Air with M1-chip. Be aware that the code gets quite buggy when the scramble needs to apply moves to reach a depth where it 

    //binary_generator(0, 2100000000, "2100mio_d9.bin"); 
    //binary_viewer(bin_filename, 1000);
    //depth_0_updater(bin_filename);
    //depth_updater(1, bin_filename, updated_states0, updated_states1, "updated_states_depth_1_file_not_necessary.txt");
    //depth_updater(2, bin_filename, updated_states1, updated_states2, "updated_states_depth_2_file_not_necessary.txt");
    //depth_updater(3, bin_filename, updated_states2, updated_states3, "updated_states_depth_3_file_not_necessary.txt");
    //depth_updater(4, bin_filename, updated_states3, updated_states4, "updated_states_depth_4_file_not_necessary.txt");
    //depth_updater(5, bin_filename, updated_states4, updated_states5, "updated_states_depth_5_file_not_necessary.txt");
    //depth_updater(6, bin_filename, updated_states5, updated_states6, "updated_states_depth_6_file_not_necessary.txt");
    //depth_updater(7, bin_filename, updated_states6, updated_states7, "updated_states_depth_7_file_not_necessary.txt");
    //depth_updater(8, bin_filename, updated_states7, updated_states8, "updated_states_depth_8_file_not_necessary.txt");
    //....
    
    // IMPORTANT: Comment part 1) after you have generated and updated the lookup table.

    // 2) here you can try out the main-solver
    //solve_scrambles_fast("scrambles.txt", bin_filename);

    random_state_string_generator("random_states.txt", 5);

    ifstream infile("random_states.txt");
    string state;
    while (getline(infile, state)) {
        solve_state_string_fast(state, "2100mio_d9.bin");
    }



    auto end_time = high_resolution_clock::now(); // code for timer
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    log_message("Execution time: " + to_string(duration.count()) + " ms");
    log_file.close();
    return 0;
}