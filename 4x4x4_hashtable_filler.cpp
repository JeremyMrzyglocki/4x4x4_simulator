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


using namespace std;
using namespace std::chrono; 

volatile bool solution_found = false;
ofstream log_file("log.txt", ios::out); // Log file for all printed outputs

// Function prototypes
string fixLetterOrder(const string& combination);
string letter_swap(string combination, char from, char to);
bool startsWithAllowedPrefix(const string& combination);
bool passesRotationTests(const string& combination, const set<string>& validCombinations);
void generateCombinations(int length, set<string>& validCombinations, int& counter, const unordered_set<string>& forbiddenSequences);
string y_rotation(const string& combination);
string y2_rotation(const string& combination);
string y3_rotation(const string& combination);
string z_rotation(const string& combination);
string z2_rotation(const string& combination);
string z3_rotation(const string& combination);
string x_rotation(const string& combination);
string x2_rotation(const string& combination);
string x3_rotation(const string& combination);
string m_mirror(const string& combination);
string s_mirror(const string& combination);
string e_mirror(const string& combination);
string sm_mirror(const string& combination);
string ms_mirror(const string& combination);
string R(const string& combination);
string R2(const string& combination);
string R3(const string& combination);
string r(const string& combination);
string r2(const string& combination);
string r3(const string& combination);
string L(const string& combination);
string L2(const string& combination);
string L3(const string& combination);
string l(const string& combination);
string l2(const string& combination);
string l3(const string& combination);
string F(const string& combination);
string F2(const string& combination);
string F3(const string& combination);
string f(const string& combination);
string f2(const string& combination);
string f3(const string& combination);
string B(const string& combination);
string B2(const string& combination);
string B3(const string& combination);
string b(const string& combination);
string b2(const string& combination);
string b3(const string& combination);
string U(const string& combination);
string U2(const string& combination);
string U3(const string& combination);
string u(const string& combination);
string u2(const string& combination);
string u3(const string& combination);
string D(const string& combination);
string D2(const string& combination);
string D3(const string& combination);
string d(const string& combination);
string d2(const string& combination);
string d3(const string& combination);
void cycle(char& a, char& b, char& c, char& d); 
void swap(char& a, char& b);
string remove_spaces(const string& str);
string apply_move(const string& state, const string& move);
string translate_scramble_to_moves(const string& scramble);

int solution_counter = 0;  // Global counter to track the number of solutions found
int solution_counter1 = 0;  
int solution_counter2 = 0;  
int solution_counter3 = 0;  
int solution_counter4 = 0;  


unordered_set<string> updated_states0;  // Global variable to store updated states
unordered_set<string> updated_states1;  
unordered_set<string> updated_states2;  
unordered_set<string> updated_states3;  
unordered_set<string> updated_states4;  
unordered_set<string> updated_states5;  
unordered_set<string> updated_states6;  
unordered_set<string> updated_states7;  
unordered_set<string> updated_states8;  
unordered_set<string> updated_states9;  




// Store logarithms of factorials to prevent overflow
vector<double> log_factorial_cache(25, 0.0);

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



// Debug function for detailed output
void test_multinomial_rank(const string &input_str, map<char, int> letter_counts) {
    cout << "Processing string: " << input_str << endl;
    unsigned long long rank = multinomial_rank(input_str, letter_counts);
    cout << "Final computed rank: " << rank << endl;
}

// Function to convert a number to a zero-padded binary string of fixed length
string to_binary_string(__uint128_t value, int bit_length) {
    return bitset<31>(static_cast<uint32_t>(value)).to_string();
}
void find_in_bin_file(const string &input_string, const string &filename) {
    // Compute multinomial rank
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};
    __uint128_t rank = multinomial_rank(input_string, letter_counts);
    cout << "Computed rank: " << uint128_to_string(rank) << endl;

    // Convert rank to 31-bit binary representation
    string rank_binary = bitset<31>(static_cast<uint32_t>(rank)).to_string();
    cout << "Binary representation: " << rank_binary << endl;

    // Calculate correct file position
    streampos file_position = rank * sizeof(uint32_t);

    ifstream input_file(filename, ios::binary);
    if (!input_file) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    uint32_t value;
    string previous_line, match_line, next_line;

    // Read the previous entry (if not the first)
    if (file_position >= static_cast<streamoff>(sizeof(uint32_t))) {
        input_file.seekg(file_position - static_cast<streamoff>(sizeof(uint32_t)), ios::beg);
        input_file.read(reinterpret_cast<char*>(&value), sizeof(uint32_t));
        previous_line = bitset<31>(value).to_string();
    } else {
        previous_line = "N/A (No previous entry)";
    }

    // Read the matching entry
    input_file.seekg(file_position, ios::beg);
    input_file.read(reinterpret_cast<char*>(&value), sizeof(uint32_t));
    match_line = bitset<31>(value).to_string();

    // Read the next entry
    if (input_file.read(reinterpret_cast<char*>(&value), sizeof(uint32_t))) {
        next_line = bitset<31>(value).to_string();
    } else {
        next_line = "N/A (No next entry)";
    }

    input_file.close();

    // Print results
    cout << "\nFound rank at computed file position: " << file_position << " bytes" << endl;
    if (!previous_line.empty()) cout << "Previous: " << previous_line << endl;
    cout << "Match:    " << match_line << endl;
    if (!next_line.empty()) cout << "Next:     " << next_line << endl;
}


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

        cout << "Binary: " << binary_representation 
             << " | Flag: " << flag_representation 
             << " | Decimal: " << value_part << endl;
    }

    input_file.close();
}

void binary_viewer_last_n_entries(const string &filename, int num_entries) {
    ifstream input_file(filename, ios::binary);
    if (!input_file) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    // Get the file size
    input_file.seekg(0, ios::end);
    streampos file_size = input_file.tellg();
    int entry_size = sizeof(uint32_t);

    // Calculate the total number of entries in the file
    int total_entries = file_size / entry_size;
    if (total_entries == 0) {
        cerr << "Error: File is empty or corrupted." << endl;
        return;
    }

    // Determine the starting position for reading the last `num_entries`
    int start_index = max(0, total_entries - num_entries);
    streampos start_position = start_index * entry_size;

    input_file.seekg(start_position, ios::beg);

    cout << "\nReading last " << num_entries << " entries from " << filename << "...\n";

    for (int i = start_index; i < total_entries; ++i) {
        uint32_t packed_value;
        input_file.read(reinterpret_cast<char*>(&packed_value), sizeof(uint32_t));
        if (input_file.eof()) break;

        // Extract 31-bit number and 4-bit flag
        uint32_t value_part = packed_value >> 4;   // Right shift to remove the flag
        uint32_t flag_part = packed_value & 0xF;   // Mask last 4 bits for flag

        // Convert to binary representation
        string binary_representation = bitset<31>(value_part).to_string();
        string flag_representation = bitset<4>(flag_part).to_string();

        cout << "Binary: " << binary_representation 
             << " | Flag: " << flag_representation 
             << " | Decimal: " << value_part << endl;
    }

    input_file.close();
}


// Function to update flag in binary file at a specific index
// Function to update flag in binary file at a specific index
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
        log_message("Error: Index out of range (attempted index: " + to_string(index) + ")");
        file.close();
        return;
    }

    // Extract the value part (first 31 bits)
    uint32_t value_part = entry >> bit_length_2;
    uint32_t current_flag = entry & 0xF;  // Extract the last 4 bits (flag)

    
    stringstream log_info;
    //log_info << "Hashindex: " << index
    //    << " | Binary: " << bitset<31>(value_part).to_string()
    //    << " | Current Flag: " << bitset<4>(current_flag).to_string()
    //    << " | New Flag: " << bitset<4>(new_flag).to_string();

    // Only update if the current flag is 1111 (binary) = 15 (decimal)
    if (current_flag == 0b1111) {
        uint32_t updated_entry = (value_part << bit_length_2) | (new_flag & 0xF);

        // Write back the updated entry
        file.seekp(file_position, ios::beg);
        file.write(reinterpret_cast<const char*>(&updated_entry), sizeof(uint32_t));

        //log_info << "  ✅ Flag updated!";

    } else {
        
        //log_info << "  ⚠️ Already upd.";
    }
    //log_message(log_info.str());
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

    cout << "Using " << total_bits << " bits per number ("
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
    string starting_cubestate = "aaaabbbbccccbbbbccccaaaa";
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};  // After removing one 'a'

    // Remove the first occurrence of 'a' from the starting state
    string cubestate_without_first_a = starting_cubestate.substr(1); 

    // Compute the index in the binary file
    __uint128_t index = multinomial_rank(cubestate_without_first_a, letter_counts);

    // Flag it as 0000
    update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0000);
    log_message("Flag at index " + uint128_to_string(index) + " set to 0000.");
        
    updated_states0.insert(cubestate_without_first_a);  // Store the transformed state

        ofstream file(filename, ios::out);
    if (!file) {
        log_message("Error opening file: " + filename);
        return;
    }

    log_message("\nSaving updated states for depth 0 in " + filename);
    for (const auto &state : updated_states0) {
        file << state << endl;
    }
    file.close();
}

// Function to apply moves and generate new states (returns move-state pairs)
vector<pair<string, string>> apply_all_moves(const string &cubestate, const vector<string> &moves) {
    vector<pair<string, string>> new_states;
    for (const string &move : moves) {
        string new_state = apply_move(cubestate, move);
        new_states.push_back({move, new_state}); // Store move and resulting state
    }

    return new_states;
}

// Function to update depth 1: Flag all new cube states
void depth_1_updater(const string &filename) { 
    string starting_cubestate = "aaaabbbbccccbbbbccccaaaa";

    vector<string> moves = {"R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
                            "U", "U2", "U'", "D", "D2", "D'", "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'",
                            "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"};

    // Apply all moves and get move-state pairs
    vector<pair<string, string>> move_results = apply_all_moves(starting_cubestate, moves);

    log_message("\nGenerated new cube states from depth 1 moves:");
    for (const auto &entry : move_results) {
        string move = entry.first;
        string fixed_state_with_23 = (fixLetterOrder(entry.second)).substr(1);   

        log_message("Move applied: " + move + " | New State: " + entry.second + " | Fixed and 23-ed: " + fixed_state_with_23);
        updated_states1.insert(fixed_state_with_23);  // Store the transformed state
    }

    // Flagging all new cube states in binary file
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};  // Adjusted counts

    for (const auto &entry : move_results) {
        __uint128_t index = multinomial_rank((fixLetterOrder(entry.second).substr(1)), letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0001);
    }

        ofstream file(filename, ios::out);
    if (!file) {
        log_message("Error opening file: " + filename);
        return;
    }

    log_message("\nSaving updated states for depth in " + filename);
    for (const auto &state : updated_states1) {
        file << state << endl;
    }
    file.close();
}

void depth_2_updater(const string &filename) {
    log_message("\n🔄 Generating depth 2 states from updated depth 1 states...");

    vector<string> moves = {"R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
                            "U", "U2", "U'", "D", "D2", "D'", "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'",
                            "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"};

    unordered_set<string> updated_states_with_a;
    unordered_set<string> depth_2_states;

    // Prepend 'a' to each state in updated_states1
    for (const auto &state : updated_states1) {
        updated_states_with_a.insert("a" + state);
    }

    log_message("\n✅ Processing " + to_string(updated_states_with_a.size()) + " depth 1 states...\n");

    int batch_counter = 1;

    // Apply all moves to each depth 1 state
    for (const auto &state : updated_states_with_a) {
        log_message("\n=== 🔹 Processing New Depth 1 State (Batch " + to_string(batch_counter) + ") ===");
        log_message("🔹 Original Depth 1 State: " + state);
        batch_counter++;

        for (const string &move : moves) {
            string new_state = apply_move(state, move);
            string fixed_new_state = fixLetterOrder(new_state).substr(1);
            depth_2_states.insert(fixed_new_state);

            log_message("Move Applied: " + move + 
                        " | Resulting Depth 2 State: " + new_state + 
                        " | Fixed and 23-ed: " + fixed_new_state);
        }
    }

    log_message("\n🔄 Updating " + to_string(depth_2_states.size()) + " depth 2 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : depth_2_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0010);
    }

    log_message("\n✅ Depth 2 state generation and update completed!");

    // Save the new depth 2 states for future processing
    ofstream file("updated_states_depth_2.txt", ios::out);
    if (!file) {
        log_message("❌ Error opening file: updated_states_depth_2.txt");
        return;
    }
    for (const auto &state : depth_2_states) {
        file << state << endl;
    }
    file.close();
    
    updated_states2 = depth_2_states;  // ✅ Ensure states are stored for next step

    log_message("\n📁 Depth 2 states saved to updated_states_depth_2.txt");


}




void depth_1_rotation_updater(const string &filename) {
    log_message("\nApplying all possible rotations to updated states from depth 1...");

    vector<string> rotations = {"x", "x2", "x'", "y", "y2", "y'", "z", "z2", "z'"};
    unordered_set<string> updated_states_with_a;

    unordered_set<string> rotated_states;

    // Prepend 'a' to each state in updated_states
    for (const auto &state : updated_states1) {
        updated_states_with_a.insert("a" + state);
    }

    for (const auto &state : updated_states_with_a) {
        for (const string &rotation : rotations) {
            string rotated_state = apply_move(state, rotation);
            string fixed_rotated_state = fixLetterOrder(rotated_state).substr(1);
            rotated_states.insert(fixed_rotated_state);

            log_message("Rotation: " + rotation + " | Resulting State: " + rotated_state + " | Fixed: " + fixed_rotated_state);
        }
    }

    log_message("\nUpdating rotated states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : rotated_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0001);
    }

    log_message("\nRotation-based depth 1 updates completed!");
}

void depth_2_rotation_updater(const string &filename) {
    log_message("\nApplying all possible rotations to updated states from depth 2...");

    vector<string> rotations = {"x", "x2", "x'", "y", "y2", "y'", "z", "z2", "z'"};
    unordered_set<string> updated_states_with_a;
    unordered_set<string> rotated_states;

    // Prepend 'a' to each state in updated_states1 (which now contains depth 2 states)
    for (const auto &state : updated_states1) {
        updated_states_with_a.insert("a" + state);
    }

    for (const auto &state : updated_states_with_a) {
        for (const string &rotation : rotations) {
            string rotated_state = apply_move(state, rotation);
            string fixed_rotated_state = fixLetterOrder(rotated_state).substr(1);
            rotated_states.insert(fixed_rotated_state);

            log_message("Rotation: " + rotation + " | Resulting State: " + rotated_state + " | Fixed: " + fixed_rotated_state);
        }
    }

    log_message("\nUpdating rotated depth 2 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : rotated_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0010);
    }
    
    log_message("\nRotation-based depth 2 updates completed!");
}

void depth_3_updater(const string &filename) {
    log_message("\n🔄 Generating depth 3 states from updated depth 2 states...");

    vector<string> moves = {"R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
                            "U", "U2", "U'", "D", "D2", "D'", "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'",
                            "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"};

    unordered_set<string> updated_states_with_a;
    unordered_set<string> depth_3_states;

    // Prepend 'a' to each state in updated_states2
    for (const auto &state : updated_states2) {
        updated_states_with_a.insert("a" + state);
    }

    log_message("\n✅ Processing " + to_string(updated_states_with_a.size()) + " depth 2 states...\n");

    int batch_counter = 1;

    // Apply all moves to each depth 2 state
    for (const auto &state : updated_states_with_a) {
        log_message("\n=== 🔹 Processing New Depth 2 State (Batch " + to_string(batch_counter) + ") ===");
        log_message("🔹 Original Depth 2 State: " + state);
        batch_counter++;

        for (const string &move : moves) {
            string new_state = apply_move(state, move);
            string fixed_new_state = fixLetterOrder(new_state).substr(1);
            depth_3_states.insert(fixed_new_state);

            //log_message("Move Applied: " + move + 
            //            " | Resulting Depth 3 State: " + new_state + 
            //            " | Fixed and 23-ed: " + fixed_new_state);
        }
    }

    log_message("\n🔄 Updating " + to_string(depth_3_states.size()) + " depth 3 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : depth_3_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0011);
    }

    log_message("\n✅ Depth 3 state generation and update completed!");

    // Save the new depth 3 states for future processing
    ofstream file("updated_states_depth_3.txt", ios::out);
    if (!file) {
        log_message("❌ Error opening file: updated_states_depth_3.txt");
        return;
    }
    for (const auto &state : depth_3_states) {
        file << state << endl;
    }
    file.close();
    updated_states3 = depth_3_states; 
    log_message("\n📁 Depth 3 states saved to updated_states_depth_3.txt");
}


void depth_4_updater(const string &filename) {
    log_message("\n🔄 Generating depth 4 states from updated depth 3 states...");

    vector<string> moves = {"R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
                            "U", "U2", "U'", "D", "D2", "D'", "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'",
                            "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"};

    unordered_set<string> updated_states_with_a;
    unordered_set<string> depth_4_states;

    // Prepend 'a' to each state in updated_states3
    for (const auto &state : updated_states3) {
        updated_states_with_a.insert("a" + state);
    }

    log_message("\n✅ Processing " + to_string(updated_states_with_a.size()) + " depth 3 states...\n");

    int batch_counter = 1;

    // Apply all moves to each depth 3 state
    for (const auto &state : updated_states_with_a) {
        //log_message("\n=== 🔹 Processing New Depth 3 State (Batch " + to_string(batch_counter) + ") ===");
        //log_message("🔹 Original Depth 3 State: " + state);
        batch_counter++;

        for (const string &move : moves) {
            string new_state = apply_move(state, move);
            string fixed_new_state = fixLetterOrder(new_state).substr(1);
            depth_4_states.insert(fixed_new_state);

            //log_message("Move Applied: " + move + 
            //            " | Resulting Depth 4 State: " + new_state + 
            //            " | Fixed and 23-ed: " + fixed_new_state);
        }
    }

    log_message("\n🔄 Updating " + to_string(depth_4_states.size()) + " depth 4 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : depth_4_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0100);
    }

    log_message("\n✅ Depth 4 state generation and update completed!");

    // Save the new depth 4 states for future processing
    ofstream file("updated_states_depth_4.txt", ios::out);
    if (!file) {
        log_message("❌ Error opening file: updated_states_depth_4.txt");
        return;
    }
    for (const auto &state : depth_4_states) {
        file << state << endl;
    }
    file.close();
    updated_states4 = depth_4_states; 
    log_message("\n📁 Depth 4 states saved to updated_states_depth_4.txt");
}

void depth_5_updater(const string &filename) {
    log_message("\n🔄 Generating depth 5 states from updated depth 4 states...");

    vector<string> moves = {"R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
                            "U", "U2", "U'", "D", "D2", "D'", "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'",
                            "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"};

    unordered_set<string> updated_states_with_a;
    unordered_set<string> depth_5_states;

    // Prepend 'a' to each state in updated_states4
    for (const auto &state : updated_states4) {
        updated_states_with_a.insert("a" + state);
    }

    log_message("\n✅ Processing " + to_string(updated_states_with_a.size()) + " depth 4 states...\n");

    int batch_counter = 1;

    // Apply all moves to each depth 4 state
    for (const auto &state : updated_states_with_a) {
        //log_message("\n=== 🔹 Processing New Depth 4 State (Batch " + to_string(batch_counter) + ") ===");
        //log_message("🔹 Original Depth 4 State: " + state);
        batch_counter++;

        for (const string &move : moves) {
            string new_state = apply_move(state, move);
            string fixed_new_state = fixLetterOrder(new_state).substr(1);
            depth_5_states.insert(fixed_new_state);

            //log_message("Move Applied: " + move + 
            //            " | Resulting Depth 5 State: " + new_state + 
            //            " | Fixed and 23-ed: " + fixed_new_state);
        }
    }

    log_message("\n🔄 Updating " + to_string(depth_5_states.size()) + " depth 5 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : depth_5_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0100);
    }

    log_message("\n✅ Depth 5 state generation and update completed!");

    // Save the new depth 5 states for future processing
    ofstream file("updated_states_depth_5.txt", ios::out);
    if (!file) {
        log_message("❌ Error opening file: updated_states_depth_5.txt");
        return;
    }
    for (const auto &state : depth_5_states) {
        file << state << endl;
    }
    file.close();
    updated_states5 = depth_5_states; 
    log_message("\n📁 Depth 5 states saved to updated_states_depth_5.txt");
}

void depth_6_updater(const string &filename) {
    log_message("\n🔄 Generating depth 6 states from updated depth 5 states...");

    vector<string> moves = {"R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
                            "U", "U2", "U'", "D", "D2", "D'", "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'",
                            "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"};

    unordered_set<string> updated_states_with_a;
    unordered_set<string> depth_6_states;

    // Prepend 'a' to each state in updated_states5
    for (const auto &state : updated_states5) {
        updated_states_with_a.insert("a" + state);
    }

    log_message("\n✅ Processing " + to_string(updated_states_with_a.size()) + " depth 5 states...\n");

    int batch_counter = 1;

    // Apply all moves to each depth 5 state
    for (const auto &state : updated_states_with_a) {
        //log_message("\n=== 🔹 Processing New Depth 5 State (Batch " + to_string(batch_counter) + ") ===");
        //log_message("🔹 Original Depth 5 State: " + state);
        batch_counter++;

        for (const string &move : moves) {
            string new_state = apply_move(state, move);
            string fixed_new_state = fixLetterOrder(new_state).substr(1);
            depth_6_states.insert(fixed_new_state);

            //log_message("Move Applied: " + move + 
            //            " | Resulting Depth 6 State: " + new_state + 
            //            " | Fixed and 23-ed: " + fixed_new_state);
        }
    }

    log_message("\n🔄 Updating " + to_string(depth_6_states.size()) + " depth 6 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : depth_6_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0110);
    }

    log_message("\n✅ Depth 6 state generation and update completed!");

    // Save the new depth 6 states for future processing
    ofstream file("updated_states_depth_6.txt", ios::out);
    if (!file) {
        log_message("❌ Error opening file: updated_states_depth_6.txt");
        return;
    }
    for (const auto &state : depth_6_states) {
        file << state << endl;
    }
    file.close();
    updated_states6 = depth_6_states; 
    log_message("\n📁 Depth 6 states saved to updated_states_depth_6.txt");
}

void depth_7_updater(const string &filename) {
    log_message("\n🔄 Generating depth 7 states from updated depth 6 states...");

    vector<string> moves = {"R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
                            "U", "U2", "U'", "D", "D2", "D'", "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'",
                            "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"};

    unordered_set<string> updated_states_with_a;
    unordered_set<string> depth_7_states;

    // Prepend 'a' to each state in updated_states6
    for (const auto &state : updated_states6) {
        updated_states_with_a.insert("a" + state);
    }

    log_message("\n✅ Processing " + to_string(updated_states_with_a.size()) + " depth 6 states...\n");

    int batch_counter = 1;

    // Apply all moves to each depth 6 state
    for (const auto &state : updated_states_with_a) {
        //log_message("\n=== 🔹 Processing New Depth 6 State (Batch " + to_string(batch_counter) + ") ===");
        //log_message("🔹 Original Depth 6 State: " + state);
        batch_counter++;

        for (const string &move : moves) {
            string new_state = apply_move(state, move);
            string fixed_new_state = fixLetterOrder(new_state).substr(1);
            depth_7_states.insert(fixed_new_state);

            //log_message("Move Applied: " + move + 
            //            " | Resulting Depth 7 State: " + new_state + 
            //            " | Fixed and 23-ed: " + fixed_new_state);
        }
    }

    log_message("\n🔄 Updating " + to_string(depth_7_states.size()) + " depth 7 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : depth_7_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0111);
    }

    log_message("\n✅ Depth 7 state generation and update completed!");

    // Save the new depth 7 states for future processing
    ofstream file("updated_states_depth_7.txt", ios::out);
    if (!file) {
        log_message("❌ Error opening file: updated_states_depth_7.txt");
        return;
    }
    for (const auto &state : depth_7_states) {
        file << state << endl;
    }
    file.close();
    updated_states7 = depth_7_states; 
    log_message("\n📁 Depth 7 states saved to updated_states_depth_7.txt");
}




// === Depth 3 Rotation Updater ===
void depth_3_rotation_updater(const string &filename) {
    log_message("\n🔄 Applying all possible rotations to updated depth 3 states...");

    vector<string> rotations = {"x", "x2", "x'", "y", "y2", "y'", "z", "z2", "z'"};
    unordered_set<string> updated_states_with_a;
    unordered_set<string> rotated_states;

    // Prepend 'a' to each state in updated_states2 (which now contains depth 3 states)
    for (const auto &state : updated_states2) {
        updated_states_with_a.insert("a" + state);
    }

    for (const auto &state : updated_states_with_a) {
        for (const string &rotation : rotations) {
            string rotated_state = apply_move(state, rotation);
            string fixed_rotated_state = fixLetterOrder(rotated_state).substr(1);
            rotated_states.insert(fixed_rotated_state);

            log_message("Rotation Applied: " + rotation + 
                        " | Resulting Rotated Depth 3 State: " + rotated_state + 
                        " | Fixed: " + fixed_rotated_state);
        }
    }

    log_message("\n🔄 Updating rotated depth 3 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : rotated_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0011);
    }

    log_message("\n✅ Rotation-based depth 3 updates completed!");
}

void depth_4_rotation_updater(const string &filename) {
    log_message("\n🔄 Applying all possible rotations to updated depth 4 states...");

    vector<string> rotations = {"x", "x2", "x'", "y", "y2", "y'", "z", "z2", "z'"};
    unordered_set<string> updated_states_with_a;
    unordered_set<string> rotated_states;

    // Prepend 'a' to each state in updated_states3 (which now contains depth 4 states)
    for (const auto &state : updated_states3) {
        updated_states_with_a.insert("a" + state);
    }

    for (const auto &state : updated_states_with_a) {
        for (const string &rotation : rotations) {
            string rotated_state = apply_move(state, rotation);
            string fixed_rotated_state = fixLetterOrder(rotated_state).substr(1);
            rotated_states.insert(fixed_rotated_state);

            log_message("Rotation Applied: " + rotation + 
                        " | Resulting Rotated Depth 4 State: " + rotated_state + 
                        " | Fixed: " + fixed_rotated_state);
        }
    }

    log_message("\n🔄 Updating rotated depth 4 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : rotated_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0100);
    }

    log_message("\n✅ Rotation-based depth 4 updates completed!");
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

// Function to remove spaces from the scramble string
string remove_spaces(const string& str) {
    string result;
    for (char ch : str) {
        if (ch != ' ') {
            result += ch;
        }
    }
    return result;
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
    //string clean_scramble = remove_spaces(scramble);
    stringstream ss(scramble);
    string move;
    string current_state = "aaaabbbbccccbbbbccccaaaa";  // HTR-solved cube state

    while (ss >> move) {
        current_state = apply_move(current_state, move);
    }
    return current_state;
}

// Function to fix letter order
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


// Check if the combination starts with an allowed prefix
bool startsWithAllowedPrefix(const string& combination) {
    const string allowedPrefixes[] = {"ab", "aab", "aaab", "aaaab", "aaaaab", "aaaaaab", "aaaaaaab", "aaaaaaaab"};
    for (const string& prefix : allowedPrefixes) {
        if (combination.rfind(prefix, 0) == 0) return true;
    }
    return false;
}



    // The following part has many redundant swaps and cylces, but it is easier to understand and verify and also faster to 
    // implement at this point in time. Will optimize later.


// Perform an "x-rotation" on the combination
string x_rotation(const string& combination) {
    string combination_new = combination;
    // Cycle 4 -> 7 -> 6 -> 5 -> 4
    cycle(combination_new[4], combination_new[7], combination_new[6], combination_new[5]);
    // Cycle 12 -> 13 -> 14 -> 15 -> 12
    cycle(combination_new[12], combination_new[13], combination_new[14], combination_new[15]);
    // Cycle 0 -> 18 -> 20 -> 8 -> 0
    cycle(combination_new[0], combination_new[18], combination_new[20], combination_new[8]);
    // Cycle 1 -> 19 -> 21 -> 9 -> 1
    cycle(combination_new[1], combination_new[19], combination_new[21], combination_new[9]);
    // Cycle 2 -> 16 -> 22 -> 10 -> 2
    cycle(combination_new[2], combination_new[16], combination_new[22], combination_new[10]);
    // Cycle 3 -> 17 -> 23 -> 11 -> 3
    cycle(combination_new[3], combination_new[17], combination_new[23], combination_new[11]);

    return combination_new;
}

string x2_rotation(const string& combination) {
    string combination_new = x_rotation(combination);  // First transformation
    return x_rotation(combination_new);               // Apply x_rotation again and return
}

string x3_rotation(const string& combination) { // Perform x3_rotation (x_rotation applied three times)
    return x_rotation(x2_rotation(combination));
}

// Perform a "y-rotation" on the combination
string y_rotation(const string& combination) {
    string combination_new = combination;

    // Cycle 0 -> 1 -> 2 -> 3 -> 0
    cycle(combination_new[0], combination_new[1], combination_new[2], combination_new[3]);
    // Cycle 4 -> 16 -> 12 -> 8 -> 4
    cycle(combination_new[4], combination_new[16], combination_new[12], combination_new[8]);
    // Cycle 5 -> 17 -> 13 -> 9 -> 5
    cycle(combination_new[5], combination_new[17], combination_new[13], combination_new[9]);
    // Cycle 6 -> 18 -> 14 -> 10 -> 6
    cycle(combination_new[6], combination_new[18], combination_new[14], combination_new[10]);
    // Cycle 7 -> 19 -> 15 -> 11 -> 7
    cycle(combination_new[7], combination_new[19], combination_new[15], combination_new[11]);
    // Cycle 20 -> 23 -> 22 -> 21 -> 20
    cycle(combination_new[20], combination_new[23], combination_new[22], combination_new[21]);
    return combination_new;

}


string y2_rotation(const string& combination) {
    string combination_new = combination;

    swap(combination_new[0], combination_new[2]);
    swap(combination_new[1], combination_new[3]);
    swap(combination_new[4], combination_new[12]);
    swap(combination_new[5], combination_new[13]);
    swap(combination_new[6], combination_new[14]);
    swap(combination_new[7], combination_new[15]);
    swap(combination_new[8], combination_new[16]);
    swap(combination_new[9], combination_new[17]);
    swap(combination_new[10], combination_new[18]);
    swap(combination_new[11], combination_new[19]);
    swap(combination_new[20], combination_new[22]);
    swap(combination_new[21], combination_new[23]);
    return combination_new;

}

string y3_rotation(const string& combination) {
    string combination_new = y2_rotation(combination); 
    return y_rotation(combination_new);            
}



// Perform a "z-rotation" on the combination
string z_rotation(const string& combination) {
    string combination_new = combination;

    // Cycle 0 -> 13 -> 22 -> 7 -> 0
    cycle(combination_new[0], combination_new[13], combination_new[22], combination_new[7]);
    // Cycle 1 -> 14 -> 23 -> 4 -> 1
    cycle(combination_new[1], combination_new[14], combination_new[23], combination_new[4]);
    // Cycle 2 -> 15 -> 20 -> 5 -> 2
    cycle(combination_new[2], combination_new[15], combination_new[20], combination_new[5]);
    // Cycle 3 -> 12 -> 21 -> 6 -> 3
    cycle(combination_new[3], combination_new[12], combination_new[21], combination_new[6]);
    // Cycle 8 -> 9 -> 10 -> 11 -> 8
    cycle(combination_new[8], combination_new[9], combination_new[10], combination_new[11]);
    // Cycle 16 -> 19 -> 18 -> 17 -> 16
    cycle(combination_new[16], combination_new[19], combination_new[18], combination_new[17]);
    return combination_new;
}

string z2_rotation(const string& combination) {
    string combination_new = z_rotation(combination);  // First transformation
    return z_rotation(combination_new);               // Apply z_rotation again and return
}

string z3_rotation(const string& combination) { // Perform z3_rotation (z_rotation applied three times)
    return z_rotation(z2_rotation(combination));
}

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
    string combination_new = combination;
    // Cycle 12 -> 13 -> 14 -> 15 -> 12
    cycle(combination_new[12], combination_new[13], combination_new[14], combination_new[15]);
    return combination_new;
}

string R2(const string& combination) {
    string combination_new = R(combination);  // First transformation
    return R(combination_new);               // Apply R again and return
}

string R3(const string& combination) { // Perform R3 (R applied three times)
    return R(R2(combination));
}

//note that uncapitalized r is not a slice move, but equivalent to Rw in some literature. This applies to all uncapiatlized moves

string r(const string& combination) {
    string combination_new = combination;
    // Cycle 12 -> 13 -> 14 -> 15 -> 12
    cycle(combination_new[12], combination_new[13], combination_new[14], combination_new[15]);
    // Cycle 1 -> 19 -> 21 -> 9 -> 1
    cycle(combination_new[1], combination_new[19], combination_new[21], combination_new[9]);
    // Cycle 2 -> 16 -> 22 -> 10 -> 2
    cycle(combination_new[2], combination_new[16], combination_new[22], combination_new[10]);
    return combination_new;
}

string r2(const string& combination) {
    string combination_new = r(combination);  // First transformation
    return r(combination_new);               // Apply R again and return
}

string r3(const string& combination) { // Perform R3 (R applied three times)
    return r(r2(combination));
}

string L(const string& combination) {
    string combination_new = combination;
    // Cycle 4 -> 5 -> 6 -> 7 -> 4
    cycle(combination_new[4], combination_new[5], combination_new[6], combination_new[7]);
    return combination_new;
}

string L2(const string& combination) {
    string combination_new = L(combination);  // First transformation
    return L(combination_new);               // Apply L again and return
}

string L3(const string& combination) { // Perform L3 (L applied three times)
    return L(L2(combination));
}

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
    // Cycle 8 -> 9 -> 10 -> 11 -> 8
    cycle(combination_new[8], combination_new[9], combination_new[10], combination_new[11]);
    return combination_new;
}

string F2(const string& combination) {
    string combination_new = F(combination);  // First transformation
    return F(combination_new);               // Apply F again and return
}

string F3(const string& combination) { // Perform F3 (F applied three times)
    return F(F2(combination));
}

string f(const string& combination) {
    string combination_new = combination;
    // Cycle 2 -> 15 -> 20 -> 5 -> 2
    cycle(combination_new[2], combination_new[15], combination_new[20], combination_new[5]);
    // Cycle 3 -> 12 -> 21 -> 6 -> 3
    cycle(combination_new[3], combination_new[12], combination_new[21], combination_new[6]);
    // Cycle 8 -> 9 -> 10 -> 11 -> 8
    cycle(combination_new[8], combination_new[9], combination_new[10], combination_new[11]);
    return combination_new;
}
        
string f2(const string& combination) {
    string combination_new = f(combination);  // First transformation
    return f(combination_new);               // Apply F again and return
}

string f3(const string& combination) { // Perform F3 (F applied three times)
    return f(f2(combination));
}

string B(const string& combination) {
    string combination_new = combination;
    // Cycle 16 -> 17 -> 18 -> 19 -> 16
    cycle(combination_new[16], combination_new[17], combination_new[18], combination_new[19]);
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
    // Cycle 0 -> 1 -> 2 -> 3 -> 0
    cycle(combination_new[0], combination_new[1], combination_new[2], combination_new[3]);
    return combination_new;
}

string U2(const string& combination) {
    string combination_new = U(combination);  // First transformation
    return U(combination_new);               // Apply F again and return
}

string U3(const string& combination) { // Perform F3 (F applied three times)
    return U(U2(combination));
}

string u(const string& combination) {
    string combination_new = combination;
    // Cycle 0 -> 1 -> 2 -> 3 -> 0
    cycle(combination_new[0], combination_new[1], combination_new[2], combination_new[3]);
    // Cycle 4 -> 16 -> 12 -> 8 -> 4
    cycle(combination_new[4], combination_new[16], combination_new[12], combination_new[8]);
    // Cycle 5 -> 17 -> 13 -> 9 -> 5
    cycle(combination_new[5], combination_new[17], combination_new[13], combination_new[9]);
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
    // Cycle 20 -> 21 -> 22 -> 23 -> 20
    cycle(combination_new[20], combination_new[21], combination_new[22], combination_new[23]);
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


// Function to generate move-group restrictions
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


// Main function
int main() {
    auto start_time = high_resolution_clock::now();
    precompute_factorials();
    vector<string> moves = {
        "R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'", 
        "U", "U2", "U'", "D", "D2", "D'", 
        "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'", 
        "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", 
        "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"
    };



    /*
    //string initial_state = "012345678901234567890123";  // Solved cube state
    string initial_state = "aaaabbbbccccbbbbccccaaaa";  // HTR-solved cube state
    cout << "\n🔢 Total solutions (d0) found: " << solution_counter << endl;
    cout << "\n🔢 Total solutions (d1) found: " << solution_counter1 << endl;
    cout << "\n🔢 Total solutions (d2) found: " << solution_counter2 << endl;
    cout << "\n🔢 Total solutions (d3) found: " << solution_counter3 << endl;
    //cout << "\n🔢 Total solutions (d4) found: " << solution_counter4 << endl;

    cout << endl << endl;
    test_multinomial_rank("aabbaaccbcbcccbaaccabbb", {{'a', 7}, {'b', 8}, {'c', 8}});
    test_multinomial_rank("abaacacbccaccbbacacbbbb", {{'a', 7}, {'b', 8}, {'c', 8}});
    test_multinomial_rank("aaccccccccbbbbbbababaaa", {{'a', 7}, {'b', 8}, {'c', 8}});
    test_multinomial_rank("aaccccccccbbbbbbbbaaaaa", {{'a', 7}, {'b', 8}, {'c', 8}});*/


    //string filename = "2100mio.bin";
    //string input_string = "accccccccbbbbbbbbaaaaaa"; // Example cube state
    //find_in_bin_file(input_string, filename);


    //test_multinomial_rank("bccccccccbbbbbbbaaaaaaa", {{'a', 7}, {'b', 8}, {'c', 8}});
    //test_multinomial_rank("abbbaabccccabbaccccbbaa", {{'a', 7}, {'b', 8}, {'c', 8}});

    
    string filename = "2100mio.bin";
    binary_generator(0, 2100000000, "2100mio.bin"); 
    binary_viewer(filename, 10);
    binary_viewer_last_n_entries(filename, 10);
    depth_0_updater(filename);
    depth_1_updater(filename);
    depth_1_rotation_updater(filename);
    depth_2_updater(filename);
    depth_2_rotation_updater(filename);

    depth_3_updater(filename);
    depth_3_rotation_updater(filename);

    depth_4_updater(filename);    
    depth_4_rotation_updater(filename);


    depth_5_updater(filename);
    depth_6_updater(filename);
    depth_7_updater(filename);


    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    log_message("Execution time: " + to_string(duration.count()) + " ms");

    log_file.close(); // Close log file
    return 0;
}