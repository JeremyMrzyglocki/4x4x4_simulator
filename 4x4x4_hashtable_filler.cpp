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
string get_canonical_rotation(const string &cubestate);
void recursive_expand(vector<string> &parent_states, const string &filename, uint32_t current_flag, map<char, int> &letter_counts, int depth);
bool recursive_expand(vector<pair<string, vector<string>>> &parent_states, 
                      const string &filename, uint32_t current_flag, 
                      map<char, int> &letter_counts, int depth, 
                      vector<string> &successful_path);
bool is_already_flagged_or_mirrored(const string &canonical_state, const string &filename, int depth, const map<char, int> &letter_counts);

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
unordered_set<string> updated_states10;  
unordered_set<string> updated_states11;  
unordered_set<string> updated_states12;  
unordered_set<string> updated_states13;  







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


// DEPTHS 0-13

// Function to update depth 0: Flag starting state with 0000
void depth_0_updater(const string &filename) {
    log_message("\n🔄 Updating depth 0 starting state...");

    string starting_cubestate = "aaaabbbbccccbbbbccccaaaa";
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    string cubestate_without_first_a = starting_cubestate.substr(1);
    __uint128_t index = multinomial_rank(cubestate_without_first_a, letter_counts);

    update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0000);
    log_message("Flag at index " + uint128_to_string(index) + " set to 0000.");

    updated_states0.insert(cubestate_without_first_a);

    ofstream file("updated_states_depth_0.txt", ios::out);
    if (!file) {
        log_message("❌ Error opening file: updated_states_depth_0.txt");
        return;
    }

    for (const auto &state : updated_states0) {
        file << state << endl;
    }
    file.close();
    log_message("📁 Depth 0 states saved to updated_states_depth_0.txt");
}


void depth_1_updater(const string &filename) {
    log_message("\n🔄 Generating depth 1 states from updated depth 0 states...");

    vector<string> moves = {"R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
                            "U", "U2", "U'", "D", "D2", "D'", "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'",
                            "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"};

    unordered_set<string> updated_states_with_a;
    unordered_set<string> depth_1_states;

    // Prepend 'a' to each state in updated_states2
    for (const auto &state : updated_states0) {
        updated_states_with_a.insert("a" + state);
    }

    log_message("\n✅ Processing " + to_string(updated_states_with_a.size()) + " depth 0 states...\n");

    int batch_counter = 1;
    int move_counter = 0;

        for (const auto &state : updated_states_with_a) {

            batch_counter++;

            for (const string &move : moves) {
                move_counter++;

                // ✅ Apply move
                string new_state = apply_move(state, move);

                // ✅ Compute canonical form
                string canonical_state = get_canonical_rotation(new_state);
                //log_message("🟢 Canonical form: " + canonical_state);

                // ✅ Extract without leading 'a'
                string fixed_state = canonical_state.substr(1);
                //log_message("🔵 Fixed canonical form: " + fixed_state);
                
                //if (is_already_flagged_or_mirrored(canonical_state, filename, 1, {{'a', 7}, {'b', 8}, {'c', 8}})) {
                //    continue;  // Skip adding/flagging this state
                //}

                if (depth_1_states.find(fixed_state) == depth_1_states.end()) {
                    //log_message("✅ New unique state added: " + fixed_state);
                } else {
                    //log_message("⚠️ Duplicate state ignored: " + fixed_state);
                }
                depth_1_states.insert(fixed_state);
            }
        }

    log_message("\n🔄 Updating " + to_string(depth_1_states.size()) + " depth 1 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : depth_1_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0001);
    }

    log_message("\n✅ Depth 1 state generation and update completed!");

    // Save the new depth 1 states for future processing
    ofstream file("updated_states_depth_1.txt", ios::out);
    if (!file) {
        log_message("❌ Error opening file: updated_states_depth_1.txt");
        return;
    }
    for (const auto &state : depth_1_states) {
        file << state << endl;
    }
    file.close();
    updated_states1 = depth_1_states; 
    log_message("\n📁 Depth 1 states saved to updated_states_depth_1.txt");
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
    int move_counter = 0;
 
    for (const auto &state : updated_states_with_a) {
        log_message("\n=== 🔹 Processing Depth 2 State (Batch " + to_string(batch_counter) + ") ===");
        log_message("🔹 Original Depth 2 State: " + state);
        batch_counter++;

        for (const string &move : moves) {
            move_counter++;

            // ✅ Apply move
            string new_state = apply_move(state, move);
            //log_message("🟡 Move applied: " + move + " | Resulting Depth 5 State: " + new_state);

            // ✅ Compute canonical form
            string canonical_state = get_canonical_rotation(new_state);
            //log_message("🟢 Canonical form: " + canonical_state);

            // ✅ Extract without leading 'a'
            string fixed_state = canonical_state.substr(1);
            //log_message("🔵 Fixed canonical form: " + fixed_state);

            //if (is_already_flagged_or_mirrored(canonical_state, filename, 2, {{'a', 7}, {'b', 8}, {'c', 8}})) {
            //    continue;  // Skip adding/flagging this state
            //}

            if (depth_2_states.find(fixed_state) == depth_2_states.end()) {
                //log_message("✅ New unique state added: " + fixed_state);
            } else {
                //log_message("⚠️ Duplicate state ignored: " + fixed_state);
            }
            depth_2_states.insert(fixed_state);
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
    int move_counter = 0;

        for (const auto &state : updated_states_with_a) {
            //log_message("\n=== 🔹 Processing Depth 2 State (Batch " + to_string(batch_counter) + ") ===");
            //log_message("🔹 Original Depth 2 State: " + state);
            batch_counter++;

            for (const string &move : moves) {
                move_counter++;

                // ✅ Apply move
                string new_state = apply_move(state, move);
                //log_message("🟡 Move applied: " + move + " | Resulting Depth 5 State: " + new_state);

                // ✅ Compute canonical form
                string canonical_state = get_canonical_rotation(new_state);
                //log_message("🟢 Canonical form: " + canonical_state);

                // ✅ Extract without leading 'a'
                string fixed_state = canonical_state.substr(1);
                //log_message("🔵 Fixed canonical form: " + fixed_state);
                
                //if (is_already_flagged_or_mirrored(canonical_state, filename, 3, {{'a', 7}, {'b', 8}, {'c', 8}})) {
                //    continue;  // Skip adding/flagging this state
                //}

                if (depth_3_states.find(fixed_state) == depth_3_states.end()) {
                    //log_message("✅ New unique state added: " + fixed_state);
                } else {
                    //log_message("⚠️ Duplicate state ignored: " + fixed_state);
                }
                depth_3_states.insert(fixed_state);
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
    int move_counter = 0;

     for (const auto &state : updated_states_with_a) {
        //log_message("\n=== 🔹 Processing Depth 3 State (Batch " + to_string(batch_counter) + ") ===");
        //log_message("🔹 Original Depth 3 State: " + state);
        batch_counter++;

        for (const string &move : moves) {
            move_counter++;

            // ✅ Apply move
            string new_state = apply_move(state, move);
            //log_message("🟡 Move applied: " + move + " | Resulting Depth 5 State: " + new_state);

            // ✅ Compute canonical form
            string canonical_state = get_canonical_rotation(new_state);
            //log_message("🟢 Canonical form: " + canonical_state);

            // ✅ Extract without leading 'a'
            string fixed_state = canonical_state.substr(1);
            //log_message("🔵 Fixed canonical form: " + fixed_state);

            //if (is_already_flagged_or_mirrored(canonical_state, filename, 4, {{'a', 7}, {'b', 8}, {'c', 8}})) {
            //    continue;  // Skip adding/flagging this state
            //}

            if (depth_4_states.find(fixed_state) == depth_4_states.end()) {
                //log_message("✅ New unique state added: " + fixed_state);
            } else {
                //log_message("⚠️ Duplicate state ignored: " + fixed_state);
            }
            depth_4_states.insert(fixed_state);
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
    int move_counter = 0;

     for (const auto &state : updated_states_with_a) {
        //log_message("\n=== 🔹 Processing Depth 4 State (Batch " + to_string(batch_counter) + ") ===");
        //log_message("🔹 Original Depth 4 State: " + state);
        batch_counter++;

        for (const string &move : moves) {
            move_counter++;

            // ✅ Apply move
            string new_state = apply_move(state, move);
            //log_message("🟡 Move applied: " + move + " | Resulting Depth 5 State: " + new_state);

            // ✅ Compute canonical form
            string canonical_state = get_canonical_rotation(new_state);
            //log_message("🟢 Canonical form: " + canonical_state);

            // ✅ Extract without leading 'a'
            string fixed_state = canonical_state.substr(1);
            //log_message("🔵 Fixed canonical form: " + fixed_state);

            //if (is_already_flagged_or_mirrored(canonical_state, filename, 5, {{'a', 7}, {'b', 8}, {'c', 8}})) {
            //    continue;  // Skip adding/flagging this state
            //}

            // ✅ Insert canonical state into depth_5_states
            if (depth_5_states.find(fixed_state) == depth_5_states.end()) {
                //log_message("✅ New unique state added: " + fixed_state);
            } else {
                //log_message("⚠️ Duplicate state ignored: " + fixed_state);
            }
            depth_5_states.insert(fixed_state);
        }
    }

    log_message("\n🔄 Updating " + to_string(depth_5_states.size()) + " depth 5 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : depth_5_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0101);
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
    int move_counter = 0;

        for (const auto &state : updated_states_with_a) {
            //log_message("\n=== 🔹 Processing Depth 5 State (Batch " + to_string(batch_counter) + ") ===");
            //log_message("🔹 Original Depth 5 State: " + state);
            batch_counter++;

            for (const string &move : moves) {
                move_counter++;

                // ✅ Apply move
                string new_state = apply_move(state, move);
                //log_message("🟡 Move applied: " + move + " | Resulting Depth 6 State: " + new_state);

                // ✅ Compute canonical form
                string canonical_state = get_canonical_rotation(new_state);
                //log_message("🟢 Canonical form: " + canonical_state);

                // ✅ Extract without leading 'a'
                string fixed_state = canonical_state.substr(1);
                //log_message("🔵 Fixed canonical form: " + fixed_state);
        
                //if (is_already_flagged_or_mirrored(canonical_state, filename, 6, {{'a', 7}, {'b', 8}, {'c', 8}})) {
                //    continue;  // Skip adding/flagging this state
                //}

                // ✅ Insert canonical state into depth_6_states
                if (depth_6_states.find(fixed_state) == depth_6_states.end()) {
                    //log_message("✅ New unique state added: " + fixed_state);
                } else {
                    //log_message("⚠️ Duplicate state ignored: " + fixed_state);
                }
                depth_6_states.insert(fixed_state);
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
    int move_counter = 0;

     for (const auto &state : updated_states_with_a) {
        //log_message("\n=== 🔹 Processing Depth 6 State (Batch " + to_string(batch_counter) + ") ===");
        //log_message("🔹 Original Depth 6 State: " + state);
        batch_counter++;

        for (const string &move : moves) {
            move_counter++;

            // ✅ Apply move
            string new_state = apply_move(state, move);
            //log_message("🟡 Move applied: " + move + " | Resulting Depth 5 State: " + new_state);

            // ✅ Compute canonical form
            string canonical_state = get_canonical_rotation(new_state);
            //log_message("🟢 Canonical form: " + canonical_state);

            // ✅ Extract without leading 'a'
            string fixed_state = canonical_state.substr(1);
            //log_message("🔵 Fixed canonical form: " + fixed_state);

            //if (is_already_flagged_or_mirrored(canonical_state, filename, 7, {{'a', 7}, {'b', 8}, {'c', 8}})) {
            //    continue;  // Skip adding/flagging this state
            //}

            if (depth_7_states.find(fixed_state) == depth_7_states.end()) {
                //log_message("✅ New unique state added: " + fixed_state);
            } else {
                //log_message("⚠️ Duplicate state ignored: " + fixed_state);
            }
            depth_7_states.insert(fixed_state);
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

void depth_8_updater(const string &filename) {
    log_message("\n🔄 Generating depth 8 states from updated depth 7 states...");

    vector<string> moves = {"R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
                            "U", "U2", "U'", "D", "D2", "D'", "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'",
                            "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"};

    unordered_set<string> updated_states_with_a;
    unordered_set<string> depth_8_states;

    // Prepend 'a' to each state in updated_states7
    for (const auto &state : updated_states7) {
        updated_states_with_a.insert("a" + state);
    }

    log_message("\n✅ Processing " + to_string(updated_states_with_a.size()) + " depth 8 states...\n");

    int batch_counter = 1;
    int move_counter = 0;

     for (const auto &state : updated_states_with_a) {
        //log_message("\n=== 🔹 Processing Depth 6 State (Batch " + to_string(batch_counter) + ") ===");
        //log_message("🔹 Original Depth 7 State: " + state);
        batch_counter++;

        for (const string &move : moves) {
            move_counter++;

            // ✅ Apply move
            string new_state = apply_move(state, move);
            //log_message("🟡 Move applied: " + move + " | Resulting Depth 8 State: " + new_state);

            // ✅ Compute canonical form
            string canonical_state = get_canonical_rotation(new_state);
            //log_message("🟢 Canonical form: " + canonical_state);

            // ✅ Extract without leading 'a'
            string fixed_state = canonical_state.substr(1);
            //log_message("🔵 Fixed canonical form: " + fixed_state);

            //if (is_already_flagged_or_mirrored(canonical_state, filename, 8, {{'a', 7}, {'b', 8}, {'c', 8}})) {
            //    continue;  // Skip adding/flagging this state
            //}

            if (depth_8_states.find(fixed_state) == depth_8_states.end()) {
                //log_message("✅ New unique state added: " + fixed_state);
            } else {
                //log_message("⚠️ Duplicate state ignored: " + fixed_state);
            }
            depth_8_states.insert(fixed_state);
        }
    }

    log_message("\n🔄 Updating " + to_string(depth_8_states.size()) + " depth 8 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : depth_8_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b1000);
    }

    log_message("\n✅ Depth 8 state generation and update completed!");

    // Save the new depth 8 states for future processing
    ofstream file("updated_states_depth_8.txt", ios::out);
    if (!file) {
        log_message("❌ Error opening file: updated_states_depth_8.txt");
        return;
    }
    for (const auto &state : depth_8_states) {
        file << state << endl;
    }
    file.close();
    updated_states8 = depth_8_states; 
    log_message("\n📁 Depth 8 states saved to updated_states_depth_8.txt");
}


void depth_9_updater(const string &filename) {
    log_message("\n🔄 Generating depth 9 states from updated depth 8 states...");

    vector<string> moves = {"R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
                            "U", "U2", "U'", "D", "D2", "D'", "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'",
                            "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"};

    unordered_set<string> updated_states_with_a;
    unordered_set<string> depth_9_states;

    // Prepend 'a' to each state in updated_states8
    for (const auto &state : updated_states8) {
        updated_states_with_a.insert("a" + state);
    }

    log_message("\n✅ Processing " + to_string(updated_states_with_a.size()) + " depth 9 states...\n");

    int batch_counter = 1;
    int move_counter = 0;

     for (const auto &state : updated_states_with_a) {
        //log_message("\n=== 🔹 Processing Depth 9 State (Batch " + to_string(batch_counter) + ") ===");
        //log_message("🔹 Original Depth 8 State: " + state);
        batch_counter++;

        for (const string &move : moves) {
            move_counter++;

            // ✅ Apply move
            string new_state = apply_move(state, move);
            //log_message("🟡 Move applied: " + move + " | Resulting Depth 8 State: " + new_state);

            // ✅ Compute canonical form
            string canonical_state = get_canonical_rotation(new_state);
            //log_message("🟢 Canonical form: " + canonical_state);

            // ✅ Extract without leading 'a'
            string fixed_state = canonical_state.substr(1);
            //log_message("🔵 Fixed canonical form: " + fixed_state);

            //if (is_already_flagged_or_mirrored(canonical_state, filename, 9, {{'a', 7}, {'b', 8}, {'c', 8}})) {
            //    continue;  // Skip adding/flagging this state
            //}

            if (depth_9_states.find(fixed_state) == depth_9_states.end()) {
                //log_message("✅ New unique state added: " + fixed_state);
            } else {
                //log_message("⚠️ Duplicate state ignored: " + fixed_state);
            }
            depth_9_states.insert(fixed_state);
        }
    }

    log_message("\n🔄 Updating " + to_string(depth_9_states.size()) + " depth 9 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : depth_9_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b1001);
    }

    log_message("\n✅ Depth 9 state generation and update completed!");

    // Save the new depth 9 states for future processing
    ofstream file("updated_states_depth_9.txt", ios::out);
    if (!file) {
        log_message("❌ Error opening file: updated_states_depth_9.txt");
        return;
    }
    for (const auto &state : depth_9_states) {
        file << state << endl;
    }
    file.close();
    updated_states9 = depth_9_states; 
    log_message("\n📁 Depth 9 states saved to updated_states_depth_9.txt");
}


void depth_10_updater(const string &filename) {
    log_message("\n🔄 Generating depth 10 states from updated depth 9 states...");

    vector<string> moves = {"R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
                            "U", "U2", "U'", "D", "D2", "D'", "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'",
                            "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"};

    unordered_set<string> updated_states_with_a;
    unordered_set<string> depth_10_states;

    // Prepend 'a' to each state in updated_states10
    for (const auto &state : updated_states9) {
        updated_states_with_a.insert("a" + state);
    }

    log_message("\n✅ Processing " + to_string(updated_states_with_a.size()) + " depth 10 states...\n");

    int batch_counter = 1;
    int move_counter = 0;

     for (const auto &state : updated_states_with_a) {

        batch_counter++;

        for (const string &move : moves) {
            move_counter++;

            // ✅ Apply move
            string new_state = apply_move(state, move);

            // ✅ Compute canonical form
            string canonical_state = get_canonical_rotation(new_state);
            //log_message("🟢 Canonical form: " + canonical_state);

            // ✅ Extract without leading 'a'
            string fixed_state = canonical_state.substr(1);
            //log_message("🔵 Fixed canonical form: " + fixed_state);

            //if (is_already_flagged_or_mirrored(canonical_state, filename, 10, {{'a', 7}, {'b', 8}, {'c', 8}})) {
            //    continue;  // Skip adding/flagging this state
            //}

            if (depth_10_states.find(fixed_state) == depth_10_states.end()) {
                //log_message("✅ New unique state added: " + fixed_state);
            } else {
                //log_message("⚠️ Duplicate state ignored: " + fixed_state);
            }
            depth_10_states.insert(fixed_state);
        }
    }

    log_message("\n🔄 Updating " + to_string(depth_10_states.size()) + " depth 10 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : depth_10_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b1010);
    }

    log_message("\n✅ Depth 10 state generation and update completed!");

    // Save the new depth 10 states for future processing
    ofstream file("updated_states_depth_10.txt", ios::out);
    if (!file) {
        log_message("❌ Error opening file: updated_states_depth_10.txt");
        return;
    }
    for (const auto &state : depth_10_states) {
        file << state << endl;
    }
    file.close();
    updated_states9 = depth_10_states; 
    log_message("\n📁 Depth 10 states saved to updated_states_depth_10.txt");
}


void depth_11_updater(const string &filename) {
    log_message("\n🔄 Generating depth 11 states from updated depth 10 states...");

    vector<string> moves = {"R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
                            "U", "U2", "U'", "D", "D2", "D'", "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'",
                            "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"};

    unordered_set<string> updated_states_with_a;
    unordered_set<string> depth_11_states;

    // Prepend 'a' to each state in updated_states10
    for (const auto &state : updated_states10) {
        updated_states_with_a.insert("a" + state);
    }

    log_message("\n✅ Processing " + to_string(updated_states_with_a.size()) + " depth 11 states...\n");

    int batch_counter = 1;
    int move_counter = 0;

     for (const auto &state : updated_states_with_a) {

        batch_counter++;

        for (const string &move : moves) {
            move_counter++;

            // ✅ Apply move
            string new_state = apply_move(state, move);

            // ✅ Compute canonical form
            string canonical_state = get_canonical_rotation(new_state);
            //log_message("🟢 Canonical form: " + canonical_state);

            // ✅ Extract without leading 'a'
            string fixed_state = canonical_state.substr(1);
            //log_message("🔵 Fixed canonical form: " + fixed_state);

            //if (is_already_flagged_or_mirrored(canonical_state, filename, 11, {{'a', 7}, {'b', 8}, {'c', 8}})) {
            //    continue;  // Skip adding/flagging this state
            //}

            if (depth_11_states.find(fixed_state) == depth_11_states.end()) {
                //log_message("✅ New unique state added: " + fixed_state);
            } else {
                //log_message("⚠️ Duplicate state ignored: " + fixed_state);
            }
            depth_11_states.insert(fixed_state);
        }
    }

    log_message("\n🔄 Updating " + to_string(depth_11_states.size()) + " depth 11 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : depth_11_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b1011);
    }

    log_message("\n✅ Depth 11 state generation and update completed!");

    // Save the new depth 11 states for future processing
    ofstream file("updated_states_depth_11.txt", ios::out);
    if (!file) {
        log_message("❌ Error opening file: updated_states_depth_11.txt");
        return;
    }
    for (const auto &state : depth_11_states) {
        file << state << endl;
    }
    file.close();
    updated_states9 = depth_11_states; 
    log_message("\n📁 Depth 11 states saved to updated_states_depth_11.txt");
}

void depth_12_updater(const string &filename) {
    log_message("\n🔄 Generating depth 12 states from updated depth 11 states...");

    vector<string> moves = {"R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
                            "U", "U2", "U'", "D", "D2", "D'", "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'",
                            "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"};

    unordered_set<string> updated_states_with_a;
    unordered_set<string> depth_12_states;

    // Prepend 'a' to each state in updated_states11
    for (const auto &state : updated_states11) {
        updated_states_with_a.insert("a" + state);
    }

    log_message("\n✅ Processing " + to_string(updated_states_with_a.size()) + " depth 12 states...\n");

    int batch_counter = 1;
    int move_counter = 0;

     for (const auto &state : updated_states_with_a) {

        batch_counter++;

        for (const string &move : moves) {
            move_counter++;

            // ✅ Apply move
            string new_state = apply_move(state, move);

            // ✅ Compute canonical form
            string canonical_state = get_canonical_rotation(new_state);
            //log_message("🟢 Canonical form: " + canonical_state);

            // ✅ Extract without leading 'a'
            string fixed_state = canonical_state.substr(1);
            //log_message("🔵 Fixed canonical form: " + fixed_state);

            //if (is_already_flagged_or_mirrored(canonical_state, filename, 12, {{'a', 7}, {'b', 8}, {'c', 8}})) {
            //    continue;  // Skip adding/flagging this state
            //}

            if (depth_12_states.find(fixed_state) == depth_12_states.end()) {
                //log_message("✅ New unique state added: " + fixed_state);
            } else {
                //log_message("⚠️ Duplicate state ignored: " + fixed_state);
            }
            depth_12_states.insert(fixed_state);
        }
    }

    log_message("\n🔄 Updating " + to_string(depth_12_states.size()) + " depth 12 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : depth_12_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b1100);
    }

    log_message("\n✅ Depth 12 state generation and update completed!");

    // Save the new depth 12 states for future processing
    ofstream file("updated_states_depth_12.txt", ios::out);
    if (!file) {
        log_message("❌ Error opening file: updated_states_depth_12.txt");
        return;
    }
    for (const auto &state : depth_12_states) {
        file << state << endl;
    }
    file.close();
    updated_states9 = depth_12_states; 
    log_message("\n📁 Depth 12 states saved to updated_states_depth_12.txt");
}

void depth_13_updater(const string &filename) {
    log_message("\n🔄 Generating depth 13 states from updated depth 12 states...");

    vector<string> moves = {"R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
                            "U", "U2", "U'", "D", "D2", "D'", "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'",
                            "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"};

    unordered_set<string> updated_states_with_a;
    unordered_set<string> depth_13_states;

    // Prepend 'a' to each state in updated_states11
    for (const auto &state : updated_states12) {
        updated_states_with_a.insert("a" + state);
    }

    log_message("\n✅ Processing " + to_string(updated_states_with_a.size()) + " depth 13 states...\n");

    int batch_counter = 1;
    int move_counter = 0;

     for (const auto &state : updated_states_with_a) {

        batch_counter++;

        for (const string &move : moves) {
            move_counter++;

            // ✅ Apply move
            string new_state = apply_move(state, move);

            // ✅ Compute canonical form
            string canonical_state = get_canonical_rotation(new_state);
            //log_message("🟢 Canonical form: " + canonical_state);

            // ✅ Extract without leading 'a'
            string fixed_state = canonical_state.substr(1);
            //log_message("🔵 Fixed canonical form: " + fixed_state);

            //if (is_already_flagged_or_mirrored(canonical_state, filename, 13, {{'a', 7}, {'b', 8}, {'c', 8}})) {
            //    continue;  // Skip adding/flagging this state
            //}

            if (depth_13_states.find(fixed_state) == depth_13_states.end()) {
                //log_message("✅ New unique state added: " + fixed_state);
            } else {
                //log_message("⚠️ Duplicate state ignored: " + fixed_state);
            }
            depth_13_states.insert(fixed_state);
        }
    }

    log_message("\n🔄 Updating " + to_string(depth_13_states.size()) + " depth 13 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : depth_13_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b1101);
    }

    log_message("\n✅ Depth 13 state generation and update completed!");

    // Save the new depth 13 states for future processing
    ofstream file("updated_states_depth_13.txt", ios::out);
    if (!file) {
        log_message("❌ Error opening file: updated_states_depth_13.txt");
        return;
    }
    for (const auto &state : depth_13_states) {
        file << state << endl;
    }
    file.close();
    updated_states9 = depth_13_states; 
    log_message("\n📁 Depth 13 states saved to updated_states_depth_13.txt");
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

            //log_message("Rotation Applied: " + rotation + 
            //            " | Resulting Rotated Depth 3 State: " + rotated_state + 
            //            " | Fixed: " + fixed_rotated_state);
        }
    }

    //log_message("\n🔄 Updating rotated depth 3 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : rotated_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0011);
    }

    //log_message("\n✅ Rotation-based depth 3 updates completed!");
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

            //log_message("Rotation Applied: " + rotation + 
            //            " | Resulting Rotated Depth 4 State: " + rotated_state + 
            //            " | Fixed: " + fixed_rotated_state);
        }
    }

    //log_message("\n🔄 Updating rotated depth 4 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : rotated_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0100);
    }

    log_message("\n✅ Rotation-based depth 4 updates completed!");
}

void depth_5_rotation_updater(const string &filename) {
    log_message("\n🔄 Applying all possible rotations to updated depth 5 states...");

    vector<string> rotations = {"x", "x2", "x'", "y", "y2", "y'", "z", "z2", "z'"};
    unordered_set<string> updated_states_with_a;
    unordered_set<string> rotated_states;

    for (const auto &state : updated_states5) {
        updated_states_with_a.insert("a" + state);
    }

    for (const auto &state : updated_states_with_a) {
        for (const string &rotation : rotations) {
            string rotated_state = apply_move(state, rotation);
            string fixed_rotated_state = fixLetterOrder(rotated_state).substr(1);
            rotated_states.insert(fixed_rotated_state);

            //log_message("Rotation Applied: " + rotation + 
            //            " | Resulting Rotated Depth 5 State: " + rotated_state + 
            //            " | Fixed: " + fixed_rotated_state);
        }
    }

    //log_message("\n🔄 Updating rotated depth 5 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : rotated_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0101);
    }

    log_message("\n✅ Rotation-based depth 5 updates completed!");
}


void depth_6_rotation_updater(const string &filename) {
    log_message("\n🔄 Applying all possible rotations to updated depth 6 states...");

    vector<string> rotations = {"x", "x2", "x'", "y", "y2", "y'", "z", "z2", "z'"};
    unordered_set<string> updated_states_with_a;
    unordered_set<string> rotated_states;

    for (const auto &state : updated_states6) {
        updated_states_with_a.insert("a" + state);
    }

    for (const auto &state : updated_states_with_a) {
        for (const string &rotation : rotations) {
            string rotated_state = apply_move(state, rotation);
            string fixed_rotated_state = fixLetterOrder(rotated_state).substr(1);
            rotated_states.insert(fixed_rotated_state);

            //log_message("Rotation Applied: " + rotation + 
            //            " | Resulting Rotated Depth 6 State: " + rotated_state + 
            //            " | Fixed: " + fixed_rotated_state);
        }
    }

    //log_message("\n🔄 Updating rotated depth 6 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : rotated_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0110);
    }

    log_message("\n✅ Rotation-based depth 6 updates completed!");
}

void depth_7_rotation_updater(const string &filename) {
    log_message("\n🔄 Applying all possible rotations to updated depth 7 states...");

    vector<string> rotations = {"x", "x2", "x'", "y", "y2", "y'", "z", "z2", "z'"};
    unordered_set<string> updated_states_with_a;
    unordered_set<string> rotated_states;

    for (const auto &state : updated_states7) {
        updated_states_with_a.insert("a" + state);
    }

    for (const auto &state : updated_states_with_a) {
        for (const string &rotation : rotations) {
            string rotated_state = apply_move(state, rotation);
            string fixed_rotated_state = fixLetterOrder(rotated_state).substr(1);
            rotated_states.insert(fixed_rotated_state);

            //log_message("Rotation Applied: " + rotation + 
            //            " | Resulting Rotated Depth 7 State: " + rotated_state + 
            //            " | Fixed: " + fixed_rotated_state);
        }
    }

    log_message("\n🔄 Updating rotated depth 7 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : rotated_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b0111);
    }

    log_message("\n✅ Rotation-based depth 7 updates completed!");
}

void depth_8_rotation_updater(const string &filename) {
    log_message("\n🔄 Applying all possible rotations to updated depth 8 states...");

    vector<string> rotations = {"x", "x2", "x'", "y", "y2", "y'", "z", "z2", "z'"};
    unordered_set<string> updated_states_with_a;
    unordered_set<string> rotated_states;

    for (const auto &state : updated_states6) {
        updated_states_with_a.insert("a" + state);
    }

    for (const auto &state : updated_states_with_a) {
        for (const string &rotation : rotations) {
            string rotated_state = apply_move(state, rotation);
            string fixed_rotated_state = fixLetterOrder(rotated_state).substr(1);
            rotated_states.insert(fixed_rotated_state);

            //log_message("Rotation Applied: " + rotation + 
            //            " | Resulting Rotated Depth 8 State: " + rotated_state + 
            //            " | Fixed: " + fixed_rotated_state);
        }
    }

    log_message("\n🔄 Updating rotated depth 8 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : rotated_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b1000);
    }

    log_message("\n✅ Rotation-based depth 8 updates completed!");
}

void depth_9_rotation_updater(const string &filename) {
    log_message("\n🔄 Applying all possible rotations to updated depth 9 states...");

    vector<string> rotations = {"x", "x2", "x'", "y", "y2", "y'", "z", "z2", "z'"};
    unordered_set<string> updated_states_with_a;
    unordered_set<string> rotated_states;

    for (const auto &state : updated_states9) {
        updated_states_with_a.insert("a" + state);
    }

    for (const auto &state : updated_states_with_a) {
        for (const string &rotation : rotations) {
            string rotated_state = apply_move(state, rotation);
            string fixed_rotated_state = fixLetterOrder(rotated_state).substr(1);
            rotated_states.insert(fixed_rotated_state);

            //log_message("Rotation Applied: " + rotation + 
            //            " | Resulting Rotated Depth 9 State: " + rotated_state + 
            //            " | Fixed: " + fixed_rotated_state);
        }
    }

    log_message("\n🔄 Updating rotated depth 9 states in binary file...");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};

    for (const auto &state : rotated_states) {
        __uint128_t index = multinomial_rank(state, letter_counts);
        update_flag_in_file(filename, static_cast<uint32_t>(index), 0b1001);
    }

    log_message("\n✅ Rotation-based depth 9 updates completed!");
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



// Recursive function to expand further layers until depth 0 is reached
bool recursive_expand(vector<tuple<string, vector<string>, vector<string>>> &parent_states, 
                      const string &filename, uint32_t current_flag, 
                      map<char, int> &letter_counts, int depth, 
                      vector<vector<string>> &successful_paths) {
    
    // Base case: If we've reached flag 0, a solution has been found
    if (current_flag == 0) {
        log_message("\n🎯 Reached depth 0. Solution found!");
        return true;
    }

    // All possible moves for the puzzle — single, double, and prime turns + wide moves
    vector<string> moves = {"R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
                            "U", "U2", "U'", "D", "D2", "D'", "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'",
                            "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"};

    log_message("\n🌳 Expanding to depth " + to_string(depth) + " (Flag " + to_string(current_flag) + ")...");

    uint32_t new_min_flag = UINT32_MAX;  // Track smallest flag found at this depth
    vector<tuple<string, vector<string>, vector<string>>> good_paths_next;  // Store promising states to go deeper on

    // Iterate over all current parent states at this depth level
    for (const auto &[parent_state, history, rotation_history] : parent_states) {
        log_message("\n🌲 Parent State: " + parent_state);

        // Try all possible moves from this parent state
        for (const string &move : moves) {
            string new_state = "a" + parent_state; // Add 'a' to preserve length or maintain encoding compatibility
            new_state = apply_move(new_state, move); // Apply move to current state

            // Normalize the state to its canonical form and track applied rotation
            auto [canonical_state, applied_rotation] = get_canonical_rotation_with_rotation(new_state);
            string fixed_state = canonical_state.substr(1);  // Remove the added 'a' to return to original format

            // Compute a unique index using multinomial ranking (based on the letter frequencies)
            __uint128_t index = multinomial_rank(fixed_state, letter_counts);

            // Load the flag from the binary file corresponding to this state index
            uint32_t flag = 0;
            ifstream input_file(filename, ios::binary);
            if (!input_file) {
                log_message("❌ Error opening file: " + filename);
                return false; // Exit on file read error
            }

            streampos file_position = index * sizeof(uint32_t); // Calculate byte offset
            input_file.seekg(file_position, ios::beg);
            input_file.read(reinterpret_cast<char*>(&flag), sizeof(uint32_t));
            input_file.close();

            // Mask only the lowest 4 bits of the flag (this is where actual depth is encoded)
            uint32_t extracted_flag = flag & 0xF;

            // Update minimum flag if smaller one is found; reset candidates
            if (extracted_flag < new_min_flag) {
                new_min_flag = extracted_flag;
                good_paths_next.clear();
            }

            // If this state matches the current best flag, track it
            if (extracted_flag == new_min_flag) {
                vector<string> new_history = history;
                new_history.push_back(move);  // Record the move taken

                vector<string> new_rotation_history = rotation_history;
                new_rotation_history.push_back(applied_rotation);  // Track the rotation applied

                good_paths_next.push_back({fixed_state, new_history, new_rotation_history});
            }

            // If we've hit a goal state (flag == 0), build and store the solution path
            if (extracted_flag == 0) {
                vector<string> solution_path;

                // Copy current move and rotation history
                vector<string> new_history = history;
                new_history.push_back(move);

                vector<string> new_rotation_history = rotation_history;
                new_rotation_history.push_back(applied_rotation);

                // Build the full path as move → rotation pairs
                for (size_t i = 0; i < new_history.size(); ++i) {
                    solution_path.push_back(new_history[i] + " → " + new_rotation_history[i]);
                }

                successful_paths.push_back(solution_path);

                // Also build a compact final solution string for logging
                string final_solution;
                for (size_t i = 0; i < new_history.size(); ++i) {
                    final_solution += new_rotation_history[i] + " " + new_history[i] + " ";
                }
                final_solution.pop_back(); // Remove trailing space
                final_solution += " (depth=" + to_string(new_history.size()) + ")";
            }
        }
    }

    // Log the best flag found at this level
    log_message("\n✅ Minimum flag at depth " + to_string(depth) + ": " + to_string(new_min_flag));

    // Recurse deeper using the good paths found
    return recursive_expand(good_paths_next, filename, new_min_flag, letter_counts, depth + 1, successful_paths);
}

// Main function to solve a scramble using precomputed binary flags
void solve_scramble(const string &scramble, const string &filename) {
    log_message("\n🔍 Solving scramble: " + scramble);

    // Step 1: Apply the scramble to the solved state
    string solved_state = "aaaabbbbccccbbbbccccaaaa";  // Reference solved configuration
    string scrambled_state = translate_scramble_to_moves(scramble);  // Apply scramble to get new state
    log_message("\n🔄 Scrambled State: " + scrambled_state);

    // Step 2: Generate all cube rotations and correct letter order for each
    vector<string> rotations = {"x", "x2", "x'", "y", "y2", "y'", "z", "z2", "z'"};
    vector<tuple<string, string, string>> all_rotations_for_this_scramble;
    
    for (const string &rotation : rotations) {
        string rotated_state = apply_move(scrambled_state, rotation);  // Rotate the scrambled state
        string fixed_rotated_state = fixLetterOrder(rotated_state);    // Fix letter ordering for consistency
        all_rotations_for_this_scramble.push_back({rotation, rotated_state, fixed_rotated_state});
    }

    // Step 3: For each rotated version, compute its index and fetch corresponding flag
    log_message("\n📜 All possible rotations and their states:");
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};  // Letter frequency for multinomial ranking
    
    uint32_t min_flag = UINT32_MAX;  // Track minimum flag among all rotations
    vector<tuple<string, vector<string>, vector<string>>> good_paths;  // States with best flag

    for (const auto &[rotation, state, fixed_state] : all_rotations_for_this_scramble) {
        string canonical_state = fixed_state.substr(1); // Remove leading 'a'
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

        // Track all equally optimal starting states
        if (extracted_flag < min_flag) {
            min_flag = extracted_flag;
            good_paths.clear();
        }
        if (extracted_flag == min_flag) {
            good_paths.push_back({canonical_state, {rotation}, {rotation}});
        }

        // Output diagnostic info for this rotation
        log_message("🔹 Rotation: " + rotation + 
                    " | State: " + state + 
                    " | Index: " + uint128_to_string(index) + 
                    " | Flag: " + to_string(extracted_flag));
    }

    // Step 4: Begin recursive expansion from best initial states
    vector<vector<string>> successful_paths;
    recursive_expand(good_paths, filename, min_flag, letter_counts, 1, successful_paths);

    // Step 5: Print all successful solution sequences
    log_message("\n🏆 SUCCESSFUL SOLUTIONS:");
    for (const auto &path : successful_paths) {
        string formatted_solution = "";
        bool is_first_entry = true;

        string initial_rotation = path[0].substr(0, 2);  // Extract initial rotation from path
        formatted_solution = initial_rotation;

        // Reconstruct the full path, skipping the first rotation (already applied)
        for (const auto &step : path) {
            size_t arrow_pos = step.find(" → ");
            if (arrow_pos != string::npos) {
                string move = step.substr(0, arrow_pos);  // Extract move
                string rotation = step.substr(arrow_pos + 4);  // Extract rotation

                if (is_first_entry) {
                    is_first_entry = false;
                } else {
                    formatted_solution += rotation + " " + move + " ";  // Output as "Rotation Move"
                }
            }
        }

        if (!formatted_solution.empty()) {
            formatted_solution.pop_back(); // Trim trailing space
        }

        log_message("Final Solution: " + formatted_solution);
    }
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

    
    string filename = "2100mio_mir.bin";
    
    binary_generator(0, 2100000000, "2100mio_mir.bin"); 
    binary_viewer(filename, 100);
    binary_viewer_last_n_entries(filename, 100);


    depth_0_updater(filename);
    depth_1_updater(filename);
    depth_2_updater(filename);
    //depth_2_rotation_updater(filename);

    depth_3_updater(filename);
    //depth_3_rotation_updater(filename);

    depth_4_updater(filename);    
    //depth_4_rotation_updater(filename);

    depth_5_updater(filename);   
    //depth_5_rotation_updater(filename);

    depth_6_updater(filename);
    //depth_6_rotation_updater(filename);

    depth_7_updater(filename);
    //depth_7_rotation_updater(filename);

    depth_8_updater(filename);   
    //depth_8_rotation_updater(filename);
 
    depth_9_updater(filename);
    //depth_9_rotation_updater(filename);

    depth_10_updater(filename);
    depth_11_updater(filename);
    depth_12_updater(filename);


    //solve_scramble("B' Rw2 R' L2 F2 R' U2 Rw B L2 Fw' D2 F Uw' R' B2 D2 L B2 L U2 L' U' Fw' D R Fw2 F Uw2 F' Rw Fw2 B' L2 R' F' L2 Fw2 D' F'", filename);
    //solve_scramble("L' R Fw B F' D' R' Uw Fw' U' Uw L Rw D2 Rw' B2 Uw2 Fw' Rw2 L2 B Fw' F' Uw U' Rw2 L F2 L D' Fw D' L B2 D2 Rw' R Fw2 U' Uw' ", filename);
    //solve_scramble("B2 F' Fw R' U F Fw' Rw U' B' Uw2 U' Fw2 F' B L2 B L2 B2 D U' Fw B' Uw' F2 Rw2 F2 Uw' L D' R L2 U' L2 F U' R Rw' L2 U'", filename);
    //solve_scramble("B' Fw Rw' R' B2 Rw' D R2 Uw' L2 Fw R' Fw2 U' L2 U2 Uw2 Rw L F2 D2 Fw U' Uw' F Rw2 Fw2 Uw R2 U Uw2 B2 Rw' Uw' R2 Rw2 Uw Fw2 B' U'", filename); // random scramble
    //solve_scramble("L2 D2 B' D2 L2 F2 B' R2 F' L2 U' D2 R2 L D F' L' D' F2 R2 Rw2 Fw2 R' U Rw2 D Fw2 L U2 B2 R2 D L' F Fw' U2 Fw2 R Uw' L2 F R' Fw Uw B L", filename); // 10 moves with current solver

    //solve_scramble("L2 Fw2 Uw L U2 F Fw2 Uw2 L2 F L F2 B D Fw2 U Uw F' B Rw2 F2 U' F' B2 L F U' R2 B' F' Fw2 D Uw2 R2 L Fw2 U B L Uw2 ", filename);
    //solve_scramble("U2 L' U2 D2 F2 R F2 R' F2 U2 L' U' F' L D' R D' F2 B2 R Fw2 R Rw2 U' Fw2 D2 R' D' L' Fw2 U2 D' B' Uw2 L Fw Uw R' Rw' F2 U Uw2 Fw2 ", filename);
    //solve_scramble("R B2 U2 R D2 R' D2 L D2 R2 U F2 R F' B L B D R B2 Fw2 Rw2 U2 L' Fw2 U Fw2 R B2 L' Rw2 Fw L2 B R' Fw' Rw' F U' R2 Fw Rw2 U2 L2 ", filename);
    //solve_scramble("D' R2 B2 U B2 U2 R2 D' F2 U' B' L2 B2 L U' D2 R2 U' R' D Fw2 L Rw2 Uw2 R F2 U' R2 U2 Fw2 D2 Uw2 L' Fw D F' L' U Rw' Fw' Uw' F Rw B' R2 ", filename);
    /*solve_scramble("U F2 R2 U' B2 U2 R2 D B2 U2 L2 F L' B' U' R' L2 B U D R Fw2 Uw2 R2 L F2 U' Rw2 F2 Fw2 D' L' U Fw' Uw2 L B2 U Rw' U2 B' Fw' Rw' U Uw R' ", filename);
    solve_scramble("B2 U2 L2 D R2 D L2 F2 U' L2 U' R2 B' R2 D' L F U R U F Rw2 U Fw2 D2 Rw2 F' U' R2 F' Fw2 D' F2 D Rw B' D2 L' Rw Uw' R' Rw' U B' Uw' B2 ", filename);
    solve_scramble("D2 R' U F2 D F2 B2 R2 U B2 D B U L F R2 U D F' R2 Rw2 U Fw2 U' D2 Rw2 B' L2 B2 D2 B Rw2 D2 R Rw B' Rw2 U2 R Fw2 U' Fw' U Fw2 Uw' F2 ", filename);
    solve_scramble("D2 L2 F2 L2 F2 D R2 B2 U R2 D2 B U2 F D' F2 U' L D' F R2 Rw2 Fw2 L U R2 F2 Uw2 B2 L' D' Rw2 Uw2 R' Fw' L D2 F' Rw D2 Fw L2 D' Uw2 ", filename);
    solve_scramble("B D2 B2 U2 L' D2 R' B2 R' F2 B2 R U' R2 F' D2 F' R U2 R Uw2 R Fw2 R D2 Uw2 Rw2 F2 Uw2 B L Fw2 D B Fw2 R Uw' L2 Uw' Fw Rw' F' Uw Rw2 ", filename);
    solve_scramble("F2 U2 F2 U' R2 U' R2 F2 D' B2 U F2 L U2 L' U' R' F L D' Uw2 F' D' L2 B Uw2 B' Fw2 U L2 D Rw' D' B2 Rw2 D2 Rw' Fw' R Fw D' B Uw B' ", filename);
    solve_scramble("U' R B2 D2 L2 D2 B U2 F' L2 F D2 F' U B L' D2 F B2 D Fw2 D L2 Uw2 Rw2 D2 Fw2 D' L' Fw2 R2 U' Fw R F2 B2 L2 Fw Uw' B2 R Fw' Rw F ", filename);
    solve_scramble("L' D' R' B' L2 F' U F D' F2 D2 R D2 F2 R2 U2 R D2 R' Rw2 Uw2 Fw2 D2 L' U Uw2 R D2 B2 D' Fw R2 U' F' Fw' D' Rw Fw U2 F B Fw2 L ", filename);
    solve_scramble("L' U2 D2 B2 U2 R F2 L' B2 D L F L B U' L2 B R B Rw2 B2 U' Uw2 F2 U2 Rw2 U' L U' R' B Rw2 Fw' L2 F2 Uw F' B2 Fw D2 L2 Uw Rw ", filename);
    solve_scramble("B R2 D2 F' R2 B' U2 B' U2 L2 F' D' B' D R2 F2 L' D' R' F' Fw2 L U' Fw2 D' R' U2 B2 Fw2 L' D Uw2 B2 Fw' U2 R2 U Fw2 Rw' D2 Fw D B' Fw Rw U' ", filename);
solve_scramble("D2 B2 L2 B2 U B2 L2 U D2 R2 U' D' F D L2 F2 L2 F R D' R Fw2 R2 Uw2 F2 U' Fw2 U L2 B' Uw2 L2 Uw2 Rw' Uw2 Rw2 B2 D2 Rw Fw' L Fw' D' R' Fw Uw' ", filename);
solve_scramble("R2 L' U2 R2 U2 F L2 B D2 L2 F2 D2 B' U' B2 L' F' U L' D Rw2 Uw2 F Fw2 L' Uw2 F2 R L' Uw2 F Uw2 R Uw' Fw2 D' B D Uw Rw' D2 Fw' D' Rw' B2 ", filename);
solve_scramble("B L F R2 F' R D2 F U R2 D' L2 D F2 U L2 D F2 L2 F Rw2 B' Fw2 Uw2 F2 R D2 F2 R Uw2 L D2 R' D Rw2 Uw Fw2 R L2 Rw B2 Fw2 Uw' Fw' D F' ", filename);
solve_scramble("D2 B U2 R2 L2 F L2 F2 U2 L2 F' U F2 R' U' R2 U' D2 L' U2 F Fw2 U' F2 Uw2 B' Fw2 L2 Rw2 Uw2 L2 B' U' B' L U' Rw' U2 R' B Uw' Rw' Uw2 L D F2 Rw2 ", filename);
solve_scramble("U' B R B2 L F2 L D2 R' L' B2 U D2 F L F' U B L' Fw2 L' U2 F2 Fw2 D2 R2 U Rw2 Fw2 D' Fw' R Fw2 D2 F2 Rw D2 B Rw' Uw F2 Uw' L2 ", filename);
solve_scramble("L' U' F2 B2 L2 F2 B2 L2 D2 R' F2 R F2 B U R F B L D' Rw2 B U2 Rw2 U D' F' D' B2 Rw2 Fw2 D Rw' F Rw F B2 R Uw' Fw2 Rw' F' Uw2 L B' ", filename);
solve_scramble("U2 F R2 D' B L F' D' R' B2 U F2 D L2 F2 L2 D B2 R2 U B2 Fw2 D' R L2 Uw2 Rw2 Fw2 R' D R2 B2 D' Fw' D' F L' U' Rw F2 Uw' F2 L Rw Uw' F' ", filename);
solve_scramble("L2 B2 D B2 U' R2 D2 L2 D L D F' L' F2 R' U B R Rw2 U2 B' U' Uw2 B Fw2 D2 F D F' B Fw2 Rw D2 R' Rw' B' Uw Fw D2 Rw2 B' U2 Fw' ", filename);
solve_scramble("B2 D2 B2 R2 B2 U' L2 D2 F2 U' B L B2 U2 F R' L U2 R2 Rw2 Uw2 F L B R2 L B' R' Uw2 B' D2 R2 Uw F U' Rw2 Uw2 Rw' Fw' Rw2 U' Uw Rw Uw2 ", filename);
solve_scramble("B U R F' U2 D' R' U L' U' B2 D2 F2 U L2 U2 R2 B2 D Uw2 R Rw2 U L' U' D' B2 Fw2 R' U2 F' Fw2 Uw2 R' Fw' R Rw Uw' Fw' D' B2 Uw L' Rw ", filename);
solve_scramble("F' U2 R2 D2 F2 D2 L' F2 B2 R' B2 U F' D2 R D' R' B2 R Fw2 R U' D2 L2 Uw2 Rw2 B2 Fw2 R U2 Fw D2 Fw' R2 D2 L' Uw Rw' F' Fw' D' Fw R' Rw2 ", filename);
solve_scramble("F' L2 B2 L D2 L2 U2 R' U2 B2 U2 D2 B R' L2 B U' R U2 D F2 Fw2 R' Uw2 Rw2 U Rw2 U R' Fw2 R2 D' L B Fw L D' B D' Uw' F2 B Uw2 Rw' Uw F2 D ", filename);
solve_scramble("B' U2 D2 R' B2 R F2 R U2 B2 R2 L' F2 B R2 L' F L2 U2 D' B' Fw2 Rw2 D Uw2 B2 L D' Rw2 Fw2 L2 D2 B' L Fw L U' Uw' F' B' L' Fw Uw Rw F2 ", filename);
solve_scramble("L2 U2 F2 R' L' F2 D2 R F2 L B2 R' D L D B' L D2 F2 D' B2 Uw2 B2 L D Fw2 U2 L' Rw2 Uw2 Fw2 U F2 Fw' Uw2 F2 B Fw2 Uw F2 Uw Rw' D2 B L' U ", filename);
solve_scramble("D2 L2 B2 U' F2 L2 D R2 U' R2 U F2 R' F' U2 L D2 B U D2 Rw2 D B U F Uw2 F' R2 U Rw2 U R2 Rw' B' Uw2 R B' Uw' F R2 Rw' F2 Rw2 Fw' ", filename);
solve_scramble("L' U R2 D F2 D2 B2 D' R2 L2 D B2 D B' D2 L' U R' L' D2 Uw2 Fw2 D R2 L' Fw2 R' Fw2 U' F2 R' Uw2 L' F' Fw' R2 D R' Uw' L2 Rw F' Uw F' Fw L2 ", filename);
solve_scramble("F' D2 L D' B2 D F R U' D2 B' R2 D2 F2 B' R2 L2 D2 B' R2 B2 Uw2 L' Rw2 Fw2 R' D R2 Fw2 L U2 D' B Fw2 L2 D Fw' R2 F U' Rw' Fw D' Fw Rw2 Uw ", filename);
solve_scramble("L U B2 D' F2 D R2 D2 R2 B2 R2 D' R L2 D F2 R2 D F' L2 Fw2 L U' Fw2 D2 L Rw2 Uw2 L' D' R Uw2 Fw L2 U R F2 Uw' B' Rw' F Uw' B2 L' Fw2 ", filename);
solve_scramble("U2 L U2 D' F2 B' R U D2 B2 L F2 L B2 D2 B2 R' F2 D2 R2 B Rw2 U' Fw2 D L D2 Fw2 L' Rw2 U2 R2 F2 R' Fw L B U D2 Fw L Uw' Rw D F2 Uw Rw ", filename);
solve_scramble("U L D2 R' B2 D2 B2 D2 L2 F2 L' F2 R B' L' F' B' R2 D' F' L' Fw2 R' L U Fw2 L U2 L Fw2 R D2 R Fw U2 Fw' U F' Fw Rw' D2 Uw L' Fw Uw2 Rw ", filename);
solve_scramble("B2 R B' U2 L2 U2 F2 U2 F' B' D2 B' D R' F2 U2 F' B' U Uw2 Fw2 L2 F Uw2 R2 Rw2 F R' L2 F' L Uw F2 D' Rw2 Fw2 Uw F' Fw' Uw L2 Fw' L2 U' ", filename);
solve_scramble("F' D' B U F2 D B2 U R2 U B2 U2 L2 D' B D2 B2 R' F L' B' Fw2 Rw2 B' U' Rw2 D' Fw2 D2 F2 Rw2 B R2 Rw B2 Fw2 L U' R2 Fw D' Uw2 R Fw ", filename);
solve_scramble("B2 L2 U' B' R' B' U' D2 F U2 D2 F B2 D2 B R' L2 F' Uw2 B D' Uw2 F' Rw2 Fw2 D2 F R2 L2 B' Rw U' F2 D Rw U Uw R2 Fw R' D2 R' F ", filename);
solve_scramble("L2 U' L' D' F L' B D2 L' F2 B2 D' B2 D2 R2 U2 L2 D' F2 D Uw2 R B2 Uw2 Rw2 B U2 F2 L' D2 F R2 Uw F D2 B2 U2 Rw' Fw' Rw2 Uw2 B2 Fw U2 ", filename);
solve_scramble("F' R' B L2 F L2 F' R2 U2 B2 L2 B' U' D2 F D L U B R B2 Uw2 Rw2 D R' F2 Rw2 Fw2 L' U2 B2 D' F Fw L Rw2 Fw' L Uw' L B R' Uw' R Uw' ", filename);
solve_scramble("R2 B2 R2 B2 U2 L U2 R' F2 R2 F B' L D' L2 D' L' D B2 U Fw2 Uw2 F' L Fw2 U2 L' Uw2 Fw2 R U2 R2 U' Uw F' Uw' Rw2 B Rw' U Fw' R2 Uw' Fw2 ", filename);
solve_scramble("R2 D' F2 U' L2 U F2 D2 L2 U' L2 F2 B L F' U' L' D' B2 R B Uw2 L Uw2 B Uw2 Fw2 R2 B' U2 F' L' F' D F B2 Fw2 Uw B' Fw R' Uw R2 F2 L Rw ", filename);
solve_scramble("F U L B2 U F' R' B2 L2 F L2 U2 L2 D2 F' L2 B' D B2 Uw2 F R' Fw2 L' Rw2 U2 F' L Uw2 B Fw2 D2 Uw' F2 B2 L' Uw' F' U2 Rw D' Uw' Rw2 D ", filename);
solve_scramble("B R' B2 R2 D2 F' U2 D2 F2 R2 F2 R F' B2 U L D R' U2 Rw2 Uw2 F2 U' Rw2 D' Uw2 B2 D' F' Rw2 U' B Rw F2 R2 Uw2 F B' R2 Uw L' Rw' Uw' F ", filename);
solve_scramble("D' L D2 R2 F2 U2 D2 R2 F U2 R2 B' R' L' U' R F2 U D Uw2 F' Fw2 U R2 D' Uw2 R2 B D2 B' Fw2 L Rw' B' Rw2 D' Fw R2 Uw' F' D2 Rw2 Fw' ", filename);
solve_scramble("B2 R U2 R D2 R2 U2 D2 L' U2 L B D' B2 U F' U2 F L D' L2 Rw2 B2 U' Uw2 L B2 L F2 Rw2 Fw2 D' F2 L2 Fw' L' Fw2 D R2 Fw' U' Uw' F2 Uw2 Rw' B' Uw' ", filename);
solve_scramble("F' L' B2 U' R2 U R2 D R2 U L2 F2 U2 F2 L U2 B' R F' R' B Rw2 F' Fw2 L2 Uw2 B Rw2 D' Rw2 F2 R2 L2 Rw' F' R2 U2 Rw' B Fw D' L2 U' Uw' R2 ", filename);
solve_scramble("L' B D2 L D2 R2 D2 F2 L U2 F2 B2 R B2 U L2 D F' D2 R' D2 Rw2 F Fw2 U F2 L2 Fw2 Rw2 B D Fw2 U' F' Rw U' L' U' Fw' Uw2 Fw' Uw Fw Rw ", filename);
solve_scramble("B2 D R2 F2 U' L2 F2 U' R2 D2 B D2 B' L' F' B2 U B L U Fw2 R B2 Fw2 D' F2 Fw2 Rw2 D2 Uw2 L B2 U' Fw' L D2 F2 L2 Rw Fw' L Rw2 U2 Fw2 L Rw ", filename);
solve_scramble("D L' F L F' U' R' B' U F2 D B2 L2 U L2 F2 R2 U D2 R Rw2 F' R2 Uw2 L2 U B L2 Rw2 U' F Uw2 L2 Rw F Fw2 Rw F' Rw Fw' D L2 Uw Fw U L2 ", filename);
solve_scramble("B L2 U2 L U2 D2 R D2 F2 D2 L2 U F U' D2 R2 F' D R' Uw2 F2 R F2 U Fw2 D B2 R Rw2 D Fw2 U' Fw U L2 B2 Rw Uw' B' Fw L' D2 F B2 ", filename);
solve_scramble("D R L2 F2 D L2 B2 D' F2 U D2 B2 U F2 L' U F' B2 U' L2 F Uw2 R F2 Uw2 B' R D2 Uw2 B2 L B Uw2 F2 Uw F L' Rw2 Fw2 L' Fw Rw2 Uw F Uw F2 U' ", filename);
solve_scramble("R2 B D2 L2 F' B' R2 B D2 R2 L2 F U L F2 L' D2 F' R' D B Fw2 D Uw2 L2 B' L2 Rw2 B2 R2 L2 U' R Rw2 U' Rw' U F Rw2 U2 Rw Fw U2 Fw2 L Uw2 ", filename);
solve_scramble("D' R2 B2 R B2 R' U2 L B2 L' B2 R' B' D' R' B U' R2 U' B D' Fw2 R' Fw2 U2 D' Fw2 U Fw2 L' Uw2 L2 B2 Fw' Rw2 D' Fw' U' R B D' Rw' Fw' Rw B' L U' ", filename);
solve_scramble("L2 F' L2 F2 B2 L' F2 B2 L' B2 L B2 D2 R2 B U2 R' D L' U' F' Rw2 U Fw2 L' Uw2 Rw2 F2 D' R2 U' R D2 Fw' Rw2 D Fw2 R' F2 Rw U' B' Uw' Rw' B' Uw' ", filename);
solve_scramble("B' D2 F2 B U2 L2 F' U2 D2 B' L2 D' L' F' R2 B2 D' B' U' F R Rw2 B Uw2 Fw2 Rw2 F' R Fw2 R2 Fw2 L F Uw R' Rw2 Uw' F2 U F2 D' Fw' L' Rw' U Uw2 B ", filename);
solve_scramble("U R2 L2 U2 D' F2 D F2 D' F2 B R' F2 L B' U' D2 B Uw2 B U2 Uw2 Rw2 F' L2 U' D' Uw2 Rw2 F D Rw' U' F' U2 F2 D2 Fw' D Rw Fw' L' Fw2 ", filename);
solve_scramble("L2 D2 F2 L2 U2 B L2 F' R2 B D2 R2 U F' B R F B2 R F' L' Rw2 Uw2 R B' Fw2 U2 F Fw2 R2 Rw2 Uw2 R' D Fw2 U B Uw' L2 Rw F Uw' R' Rw U2 Uw ", filename);
solve_scramble("L' U2 R' D2 F2 R' D2 R' L2 F B2 U2 F U B' L' F B D L Uw2 Rw2 D' Fw2 L' U2 D R Fw2 U' Fw2 D' F Fw' R' U2 F2 Rw D Fw U2 Uw Rw2 D' ", filename);
solve_scramble("D2 F U2 B' R2 D2 L2 B' U2 L2 B D' R' U2 F' D' B R2 U L2 Uw2 L' Uw2 R D2 Rw2 D' F2 D2 Fw2 R U' B U2 Fw' R2 U2 Fw2 D2 Uw R2 Rw' Uw2 Fw R' ", filename);
solve_scramble("R' F D2 L2 U' L2 U' B2 U2 D R2 F2 B U R' D' L2 F U' R Rw2 Fw2 R U Fw2 L' U2 Fw2 R U R2 U' R' Fw Rw2 B D' F2 R' Rw' D Fw D R' Fw Rw ", filename);
solve_scramble("U R F2 L F2 R2 U2 L' F2 U2 R D2 F' R F2 U' D2 R F B2 Uw2 B Uw2 F' R2 Fw2 U' Rw2 U F2 Fw2 U L D' B2 Rw' F U Fw Rw' D' Uw2 F' Rw' U ", filename);
solve_scramble("L D2 B2 U2 L2 D2 R2 B2 L' D2 B2 R' D R F D L2 U R B' L Fw2 R U Fw2 Uw2 F2 U F2 Fw2 D L' Fw L2 F L2 F R Uw L' Rw' Fw2 U2 Uw L' ", filename);
solve_scramble("R U B2 U2 D2 B L2 F' D2 F' U2 F2 D2 F' R F' L' D B2 R B' Rw2 F Fw2 L2 Uw2 L2 B Fw2 U' Rw2 U B' Rw' Uw2 L2 U2 B2 D Fw U Rw2 Fw2 U' Uw' B ", filename);
solve_scramble("R2 D' L2 U2 L2 D F2 R2 F2 B2 U' L F L B' D' F R' F2 L' Fw2 Uw2 L B2 U' Fw2 Rw2 D R L F2 D2 B' Uw2 F2 Fw' L2 Rw' D2 F Uw Fw' Uw' Rw F2 ", filename);
solve_scramble("L' D' L B2 U B2 R F2 D' B R2 U2 F D2 B2 L2 B R2 B D2 F2 Fw2 L' Uw2 F2 Uw2 Rw2 B' R Fw2 R B2 D2 B Uw Rw2 U2 R' B2 U2 Fw Uw L B2 Fw2 R2 L ", filename);
solve_scramble("F' R' B' R2 D B U F U' L U2 D2 L U2 L' D2 B2 R' B2 R' Fw2 R' U Uw2 Rw2 F2 U' L2 U' B2 Fw2 L F2 Fw' D2 F' Fw Rw' F' U2 Fw D2 B' Fw Rw2 ", filename);
solve_scramble("D2 B R B2 L' U B D B2 L2 B' D2 R2 L2 F' R2 B R2 U2 B2 R Uw2 B Fw2 L' B L' Rw2 Fw2 R' F Uw2 L2 Fw2 Uw R L2 U2 L2 Rw Fw L' Uw Rw D' Fw2 U ", filename);
solve_scramble("R F2 R B2 D2 R' U2 L' B2 R U2 L D' L' U' R U' L2 B' L U' Fw2 R U Rw2 Uw2 R2 D2 B2 Uw2 F2 L' F' Uw2 Fw D B2 Rw2 Fw' Uw Fw L Fw Rw' Fw ", filename);
solve_scramble("B' R2 F2 R U B' L B U2 R' D2 L B2 R' L' F2 D2 B2 U Fw2 R' Rw2 Uw2 F2 B Fw2 U2 Fw2 R' Uw2 F' Uw F2 Uw Fw2 D' Rw' D2 Fw L2 F2 Rw D2 ", filename);
solve_scramble("U2 R' B2 R' U2 D2 R F2 L D2 L' B2 U L' B D2 R U' F U2 B Uw2 R' U' Uw2 L' U Rw2 U R' Fw2 R B' Fw' R' U2 L' Rw F' Uw' R' U' Rw' F Uw ", filename);
solve_scramble("U' F2 D F2 B2 L2 U2 D L2 B2 R L2 U2 L B L2 F D L2 B L2 Rw2 D R2 Fw2 D2 L' Fw2 U F2 L D R' L' Fw' D' R L2 Fw2 Uw' L' U Rw Fw2 U Rw ", filename);
solve_scramble("L F2 U2 L2 B2 R' B2 R' U2 B2 L' B2 D F B2 D' R2 F L2 F' R' Rw2 Uw2 F' U2 Fw2 Uw2 L F2 U2 L' F Rw2 F' D' Uw L2 F' R2 F' Rw' U' Rw2 D' Uw' Rw B2 ", filename);
solve_scramble("F2 U' L B' R D2 B2 U' F' R U2 R' U2 L D2 B2 L2 F2 L' B2 Fw2 R' Rw2 Uw2 Fw2 D2 B' R B' Rw2 D2 F D' Uw' R2 B Uw2 Fw L' Uw F D2 R2 D' ", filename);
solve_scramble("L2 U2 L2 F U2 B2 D2 L2 F D2 F' B' L' F R U B D R2 U' B Uw2 Fw2 U' D2 R2 B' L2 U' Rw2 B U Uw2 B2 Rw' D' Fw2 R F2 Rw' Uw F R L2 Fw2 R' Rw ", filename);
solve_scramble("L' U2 F2 D2 F L2 F' B2 R2 D2 F R2 D' R F' R' B D2 L F' Uw2 R2 Rw2 D R2 L' F2 D' Rw2 D' Rw2 Uw2 L2 B' R' Rw2 D Fw' Uw' F2 D R' Rw' B' Uw' ", filename);
solve_scramble("B' R2 L F2 R2 F2 U2 R B2 R2 F2 B' U' L' F2 D R D2 R L Fw2 D Rw2 U' R2 F2 Uw2 F2 L Fw2 L U2 D2 Fw' L2 U2 Fw U' R2 Uw' R D2 Rw2 Fw Rw ", filename);*/


    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    log_message("Execution time: " + to_string(duration.count()) + " ms");

    log_file.close(); // Close log file
    return 0;
}