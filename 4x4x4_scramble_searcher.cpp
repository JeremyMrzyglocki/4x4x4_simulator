#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include <vector>
#include <unordered_set>
#include <sstream>
#include <map>
#include <chrono> 

using namespace std;
using namespace std::chrono; 

volatile bool solution_found = false;

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
    string combination_new = B(combination);  // First transformation
    return B(combination_new);               // Apply F again and return
}

string B3(const string& combination) { // Perform F3 (F applied three times)
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



// the following function has a scramble as input and then searches for the cubestate in the file "all_five_move_scrambles_unique_states.txt"
/*void search_this_scramble(const string& scramble, const string& search_filename) {
    cout << "🔍 Loading known states from file: " << search_filename << "..." << endl;

    // Load the file into a set for quick lookup
    unordered_set<string> known_states;
    ifstream file(search_filename);

    if (!file) {
        cerr << "❌ Error: Could not open " << search_filename << " for reading!" << endl;
        return;
    }

    string line;
    int line_count = 0;

    while (getline(file, line)) {
        // Find the first occurrence of 'a' which marks the start of the cube state
        size_t state_start = line.find('a');

        if (state_start != string::npos) {
            string state = line.substr(state_start);  // Extract cube state from first 'a' onward

            // Remove potential extra spaces (if any)
            state.erase(remove(state.begin(), state.end(), ' '), state.end());

            // Store in the known states set
            known_states.insert(state);
            line_count++;

            //cout << "Known state [" << line_count << "]: " << state << endl;  // DEBUG PRINT
        }
    }
    file.close();

    cout << "✅ Loaded " << line_count << " unique cube states from file." << endl;

    if (known_states.empty()) {
        cerr << "❌ Error: No states were loaded. Check file format!" << endl;
        return;
    }

    // Generate the cube state of the scramble
    string cube_state = fixLetterOrder(translate_scramble_to_moves(scramble));
    //cout << "🔄 Cube state after scramble: " << cube_state << endl;

    // Generate all symmetrical states for comparison
    unordered_set<string> equivalent_states = {
        fixLetterOrder(cube_state),
        fixLetterOrder(y_rotation(cube_state)),
        fixLetterOrder(y2_rotation(cube_state)),
        fixLetterOrder(y3_rotation(cube_state)),
        fixLetterOrder(x_rotation(cube_state)),
        fixLetterOrder(x2_rotation(cube_state)),
        fixLetterOrder(x3_rotation(cube_state)),
        fixLetterOrder(z_rotation(cube_state)),
        fixLetterOrder(z2_rotation(cube_state)),
        fixLetterOrder(z3_rotation(cube_state)),
        fixLetterOrder(x3_rotation(z_rotation(cube_state))),
        fixLetterOrder(x3_rotation(z2_rotation(cube_state))),
        fixLetterOrder(x3_rotation(z3_rotation(cube_state))),
        fixLetterOrder(y2_rotation(z_rotation(cube_state))),
        fixLetterOrder(y2_rotation(z2_rotation(cube_state))),
        fixLetterOrder(y2_rotation(z3_rotation(cube_state))),
        fixLetterOrder(y_rotation(z_rotation(cube_state))),
        fixLetterOrder(y_rotation(z2_rotation(cube_state))),
        fixLetterOrder(y_rotation(z3_rotation(cube_state))),
        fixLetterOrder(y3_rotation(z_rotation(cube_state))),
        fixLetterOrder(y3_rotation(z2_rotation(cube_state))),
        fixLetterOrder(y3_rotation(z3_rotation(cube_state))),
        fixLetterOrder(x_rotation(z_rotation(cube_state))),
        fixLetterOrder(x_rotation(z2_rotation(cube_state))),
        fixLetterOrder(x_rotation(z3_rotation(cube_state))),
        fixLetterOrder(m_mirror(cube_state)),
        fixLetterOrder(s_mirror(cube_state)),
        fixLetterOrder(e_mirror(cube_state)),
        fixLetterOrder(m_mirror(s_mirror(cube_state))),
        fixLetterOrder(s_mirror(m_mirror(cube_state))),
        fixLetterOrder(e_mirror(m_mirror(cube_state))),
        fixLetterOrder(m_mirror(e_mirror(cube_state))),
        fixLetterOrder(s_mirror(e_mirror(cube_state))),
        fixLetterOrder(e_mirror(s_mirror(cube_state)))
    };

    cout << "🌀 Generated " << equivalent_states.size() << " symmetrical variations of the cube state." << endl;

    // Show all generated symmetrical states for debugging
    for (const string& state_variant : equivalent_states) {
        //cout << "🔍 Checking symmetrical state: " << state_variant << endl;
    }

    // Check if any symmetrical variant exists in known_states
    for (const string& state_variant : equivalent_states) {
        if (known_states.find(state_variant) != known_states.end()) {
            cout << "✅ Found match! Scramble: " << scramble << " -> State: " << state_variant << endl;
            return;
        }
    }

    cout << "❌ No match found for scramble: " << scramble << endl;
}*/

void find_solution_with_0_extra_moves(const string& scramble, const string& search_filename) {
        if (solution_found) return;  // Stop if a solution was already found

    cout << "\n🔍 Searching for solution to scramble: " << scramble << endl;

    unordered_map<string, string> state_to_scramble;  // Maps cube state -> original scramble
    ifstream file(search_filename);

    if (!file) {
        cerr << "❌ Error: Could not open " << search_filename << " for reading!" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
    //cout << "start" << endl;
        size_t state_start = line.find('a');
        if (state_start != string::npos) {
            string scramble_seq = line.substr(0, state_start - 1); // Extract scramble
            string state = line.substr(state_start); // Extract state
            state.erase(remove(state.begin(), state.end(), ' '), state.end()); // Remove spaces
            state_to_scramble[state] = scramble_seq; // Map state -> scramble
        }
    }
    //cout << "end" << endl;
    file.close();

    cout << "✅ Loaded " << state_to_scramble.size() << " unique cube states from file.\n" << endl;

    // Base state (after applying input scramble)
    string base_state = translate_scramble_to_moves(scramble);
    //cout << "🔄 Cube state after scramble: " << base_state << endl;

    unordered_map<string, string> sym_states = {
        {fixLetterOrder(base_state), "Original"},
        {fixLetterOrder(y_rotation(base_state)), "y Rotation"},
        {fixLetterOrder(y2_rotation(base_state)), "y2 Rotation"},
        {fixLetterOrder(y3_rotation(base_state)), "y' Rotation"},
        {fixLetterOrder(x_rotation(base_state)), "x Rotation"},
        {fixLetterOrder(x2_rotation(base_state)), "x2 Rotation"},
        {fixLetterOrder(x3_rotation(base_state)), "x' Rotation"},
        {fixLetterOrder(z_rotation(base_state)), "z Rotation"},
        {fixLetterOrder(z2_rotation(base_state)), "z2 Rotation"},
        {fixLetterOrder(z3_rotation(base_state)), "z' Rotation"},
        {fixLetterOrder(m_mirror(base_state)), "M Mirror"},
        {fixLetterOrder(s_mirror(base_state)), "S Mirror"},
        {fixLetterOrder(e_mirror(base_state)), "E Mirror"},
        {fixLetterOrder(m_mirror(s_mirror(base_state))), "M + S Mirror"},
        {fixLetterOrder(s_mirror(m_mirror(base_state))), "S + M Mirror"},
        {fixLetterOrder(e_mirror(m_mirror(base_state))), "E + M Mirror"},
        {fixLetterOrder(m_mirror(e_mirror(base_state))), "M + E Mirror"},
        {fixLetterOrder(s_mirror(e_mirror(base_state))), "S + E Mirror"},
        {fixLetterOrder(e_mirror(s_mirror(base_state))), "E + S Mirror"}
    };

    // Debug output: Show transformed states
    for (const auto& [transformed_state, transformation_desc] : sym_states) {
        //cout << "🔄 Transformation: " << transformation_desc << " -> " << transformed_state << endl;
    }

    // Search in the dataset
    for (const auto& [transformed_state, transformation_desc] : sym_states) {
        auto it = state_to_scramble.find(transformed_state);
        if (it != state_to_scramble.end()) {
            string original_scramble = it->second; // Retrieve original scramble
            solution_found = true;  // ✅ Mark as found

            solution_counter++;  // 🔥 INCREMENT SOLUTION COUNTER HERE

            cout << "\n✅ Found solution!" << endl;
            cout << "🔹 Given scramble: " << scramble << endl;
            cout << "🔹 Applied transformation: " << transformation_desc << endl;
            cout << "🔹 Matching dataset scramble: " << original_scramble << endl;
            cout << "🔹 Final solution sequence: " << transformation_desc 
                << " invert(" << original_scramble << ")" << endl;
            cout << "🧩 Final cube state (for debugging): " << transformed_state << endl;

            return;
        }
    }

    cout << "\n❌ No solution found for scramble: " << scramble << endl;
}


void find_solution_with_1_extra_move(const string& scramble, const string& search_filename, const vector<string>& moves) {
        if (solution_found) return;  // Stop if a solution was already found

    cout << "\n🔍 Processing scramble: " << scramble << endl;

    unordered_map<string, string> state_to_scramble;  // Maps cube state -> original scramble
    ifstream file(search_filename);

    if (!file) {
        cerr << "❌ Error: Could not open " << search_filename << " for reading!" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        size_t state_start = line.find('a');
        if (state_start != string::npos) {
            string scramble_seq = line.substr(0, state_start - 1); // Extract scramble
            string state = line.substr(state_start); // Extract state
            state.erase(remove(state.begin(), state.end(), ' '), state.end()); // Remove spaces
            state_to_scramble[state] = scramble_seq; // Map state -> scramble
        }
    }
    file.close();

    cout << "✅ Loaded " << state_to_scramble.size() << " unique cube states from file.\n" << endl;

    // Base state (after applying input scramble)
    string base_state = translate_scramble_to_moves(scramble);
    //cout << "🔄 Cube state after scramble: " << base_state << endl;

    unordered_map<string, string> temporary_branches;

    // Generate moves of depth 1
    for (const string& move1 : moves) {
        string scramble_1 = scramble + " " + move1;
        string state_1 = translate_scramble_to_moves(scramble_1);
        //cout << "➡️ After move '" << move1 << "': " << state_1 << endl;
        temporary_branches[state_1] = move1; // Store only the extra move
    }

    cout << "\n🌀 Generated " << temporary_branches.size() << " temporary branches.\n" << endl;

    // Apply normalization and symmetry transformations
    for (const auto& [state, move_applied] : temporary_branches) {
        //cout << "\n🔍 Checking state: " << state << " (from extra move: " << move_applied << ")" << endl;

        unordered_map<string, string> sym_states = {
            {fixLetterOrder(state), "Original"},
            {fixLetterOrder(y_rotation(state)), "y Rotation"},
            {fixLetterOrder(y2_rotation(state)), "y2 Rotation"},
            {fixLetterOrder(y3_rotation(state)), "y' Rotation"},
            {fixLetterOrder(x_rotation(state)), "x Rotation"},
            {fixLetterOrder(x2_rotation(state)), "x2 Rotation"},
            {fixLetterOrder(x3_rotation(state)), "x' Rotation"},
            {fixLetterOrder(z_rotation(state)), "z Rotation"},
            {fixLetterOrder(z2_rotation(state)), "z2 Rotation"},
            {fixLetterOrder(z3_rotation(state)), "z' Rotation"},
            {fixLetterOrder(m_mirror(state)), "M Mirror"},
            {fixLetterOrder(s_mirror(state)), "S Mirror"},
            {fixLetterOrder(e_mirror(state)), "E Mirror"},
            {fixLetterOrder(m_mirror(s_mirror(state))), "M + S Mirror"},
            {fixLetterOrder(s_mirror(m_mirror(state))), "S + M Mirror"},
            {fixLetterOrder(e_mirror(m_mirror(state))), "E + M Mirror"},
            {fixLetterOrder(m_mirror(e_mirror(state))), "M + E Mirror"},
            {fixLetterOrder(s_mirror(e_mirror(state))), "S + E Mirror"},
            {fixLetterOrder(e_mirror(s_mirror(state))), "E + S Mirror"}
        };

        // Debug output: Show transformed states
        for (const auto& [transformed_state, transformation_desc] : sym_states) {
            //cout << "🔄 Transformation: " << transformation_desc << " -> " << transformed_state << endl;
        }

        // Search in the dataset
        for (const auto& [transformed_state, transformation_desc] : sym_states) {
            auto it = state_to_scramble.find(transformed_state);
            if (it != state_to_scramble.end()) {
                string original_scramble = it->second; // Retrieve original scramble
                solution_found = true;  // ✅ Mark as found

                solution_counter1++;  // 🔥 INCREMENT SOLUTION COUNTER HERE

                cout << "\n✅ Found solution!" << endl;
                cout << "🔹 Given scramble: " << scramble << endl;
                cout << "🔹 Extra move applied: " << move_applied << endl;
                cout << "🔹 Applied transformation: " << transformation_desc << endl;
                cout << "🔹 Matching dataset scramble: " << original_scramble << endl;
                cout << "🔹 Final solution sequence: " << move_applied 
                    << " " << transformation_desc << " invert(" << original_scramble << ")" << endl;
                cout << "🧩 Final cube state (for debugging): " << transformed_state << endl;

                return;
            }   
            
        }
    }

    cout << "\n❌ No solution found for scramble: " << scramble << endl;
}


void find_solution_with_2_extra_moves(const string& scramble, const string& search_filename, const vector<string>& moves) {
        if (solution_found) return;  // Stop if a solution was already found

    cout << "\n🔍 Processing scramble: " << scramble << endl;

    unordered_map<string, string> state_to_scramble;  // Maps cube state -> original scramble
    ifstream file(search_filename);

    if (!file) {
        cerr << "❌ Error: Could not open " << search_filename << " for reading!" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        size_t state_start = line.find('a');
        if (state_start != string::npos) {
            string scramble_seq = line.substr(0, state_start - 1); // Extract scramble
            string state = line.substr(state_start); // Extract state
            state.erase(remove(state.begin(), state.end(), ' '), state.end()); // Remove spaces
            state_to_scramble[state] = scramble_seq; // Map state -> scramble
        }
    }
    file.close();

    cout << "✅ Loaded " << state_to_scramble.size() << " unique cube states from file.\n" << endl;

    // Base state (after applying input scramble)
    string base_state = translate_scramble_to_moves(scramble);
    //cout << "🔄 Cube state after scramble: " << base_state << endl;

    unordered_map<string, string> temporary_branches;

    // Generate moves of depth 1 and 2
    for (const string& move1 : moves) {
        string scramble_1 = scramble + " " + move1;
        string state_1 = translate_scramble_to_moves(scramble_1);
        //cout << "➡️ After move '" << move1 << "': " << state_1 << endl;
        temporary_branches[state_1] = move1; // Store only the extra move

        for (const string& move2 : moves) {
            string scramble_2 = scramble_1 + " " + move2;
            string state_2 = translate_scramble_to_moves(scramble_2);
            //cout << "➡️ After moves '" << move1 << " " << move2 << "': " << state_2 << endl;
            temporary_branches[state_2] = move1 + " " + move2; // Store only the extra two moves
        }
    }

    cout << "\n🌀 Generated " << temporary_branches.size() << " temporary branches.\n" << endl;

    // Apply normalization and symmetry transformations
    for (const auto& [state, moves_applied] : temporary_branches) {
        //cout << "\n🔍 Checking state: " << state << " (from extra moves: " << moves_applied << ")" << endl;

        unordered_map<string, string> sym_states = {
            {fixLetterOrder(state), "Original"},
            {fixLetterOrder(y_rotation(state)), "y Rotation"},
            {fixLetterOrder(y2_rotation(state)), "y2 Rotation"},
            {fixLetterOrder(y3_rotation(state)), "y' Rotation"},
            {fixLetterOrder(x_rotation(state)), "x Rotation"},
            {fixLetterOrder(x2_rotation(state)), "x2 Rotation"},
            {fixLetterOrder(x3_rotation(state)), "x' Rotation"},
            {fixLetterOrder(z_rotation(state)), "z Rotation"},
            {fixLetterOrder(z2_rotation(state)), "z2 Rotation"},
            {fixLetterOrder(z3_rotation(state)), "z' Rotation"},
            {fixLetterOrder(m_mirror(state)), "M Mirror"},
            {fixLetterOrder(s_mirror(state)), "S Mirror"},
            {fixLetterOrder(e_mirror(state)), "E Mirror"},
            {fixLetterOrder(m_mirror(s_mirror(state))), "M + S Mirror"},
            {fixLetterOrder(s_mirror(m_mirror(state))), "S + M Mirror"},
            {fixLetterOrder(e_mirror(m_mirror(state))), "E + M Mirror"},
            {fixLetterOrder(m_mirror(e_mirror(state))), "M + E Mirror"},
            {fixLetterOrder(s_mirror(e_mirror(state))), "S + E Mirror"},
            {fixLetterOrder(e_mirror(s_mirror(state))), "E + S Mirror"}
        };

        // Debug output: Show transformed states
        for (const auto& [transformed_state, transformation_desc] : sym_states) {
            //cout << "🔄 Transformation: " << transformation_desc << " -> " << transformed_state << endl;
        }

        // Search in the dataset
        for (const auto& [transformed_state, transformation_desc] : sym_states) {
            auto it = state_to_scramble.find(transformed_state);
            if (it != state_to_scramble.end()) {
            string original_scramble = it->second; // Retrieve original scramble
            solution_found = true;  // ✅ Mark as found

            solution_counter2++;  // 🔥 INCREMENT SOLUTION COUNTER HERE

            cout << "\n✅ Found solution!" << endl;
            cout << "🔹 Given scramble: " << scramble << endl;
            cout << "🔹 Extra move applied: " << moves_applied << endl;
            cout << "🔹 Applied transformation: " << transformation_desc << endl;
            cout << "🔹 Matching dataset scramble: " << original_scramble << endl;
            cout << "🔹 Final solution sequence: " << moves_applied 
                << " " << transformation_desc << " invert(" << original_scramble << ")" << endl;
            cout << "🧩 Final cube state (for debugging): " << transformed_state << endl;

            return;
        }
        }
    }

    cout << "\n❌ No solution found for scramble: " << scramble << endl;
}


void find_solution_with_3_extra_moves(const string& scramble, const string& search_filename, const vector<string>& moves) {
        if (solution_found) return;  // Stop if a solution was already found

    cout << "\n🔍 Processing scramble: " << scramble << endl;

    unordered_map<string, string> state_to_scramble;  // Maps cube state -> original scramble
    ifstream file(search_filename);

    if (!file) {
        cerr << "❌ Error: Could not open " << search_filename << " for reading!" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        size_t state_start = line.find('a');
        if (state_start != string::npos) {
            string scramble_seq = line.substr(0, state_start - 1); // Extract scramble
            string state = line.substr(state_start); // Extract state
            state.erase(remove(state.begin(), state.end(), ' '), state.end()); // Remove spaces
            state_to_scramble[state] = scramble_seq; // Map state -> scramble
        }
    }
    file.close();

    cout << "✅ Loaded " << state_to_scramble.size() << " unique cube states from file.\n" << endl;

    // Base state (after applying input scramble)
    string base_state = translate_scramble_to_moves(scramble);
    //cout << "🔄 Cube state after scramble: " << base_state << endl;

    unordered_map<string, string> temporary_branches;

    // Generate moves of depth 1, 2, and 3
    for (const string& move1 : moves) {
        string scramble_1 = scramble + " " + move1;
        string state_1 = translate_scramble_to_moves(scramble_1);
        //cout << "➡️ After move '" << move1 << "': " << state_1 << endl;
        temporary_branches[state_1] = move1; // Store only the extra move

        for (const string& move2 : moves) {
            string scramble_2 = scramble_1 + " " + move2;
            string state_2 = translate_scramble_to_moves(scramble_2);
            //cout << "➡️ After moves '" << move1 << " " << move2 << "': " << state_2 << endl;
            temporary_branches[state_2] = move1 + " " + move2; // Store only the extra two moves

            for (const string& move3 : moves) {
                string scramble_3 = scramble_2 + " " + move3;
                string state_3 = translate_scramble_to_moves(scramble_3);
                //cout << "➡️ After moves '" << move1 << " " << move2 << " " << move3 << "': " << state_3 << endl;
                temporary_branches[state_3] = move1 + " " + move2 + " " + move3; // Store only the extra three moves
            }
        }
    }

    cout << "\n🌀 Generated " << temporary_branches.size() << " temporary branches.\n" << endl;

    // Apply normalization and symmetry transformations
    for (const auto& [state, moves_applied] : temporary_branches) {
        //cout << "\n🔍 Checking state: " << state << " (from extra moves: " << moves_applied << ")" << endl;

        unordered_map<string, string> sym_states = {
            {fixLetterOrder(state), "Original"},
            {fixLetterOrder(y_rotation(state)), "y Rotation"},
            {fixLetterOrder(y2_rotation(state)), "y2 Rotation"},
            {fixLetterOrder(y3_rotation(state)), "y' Rotation"},
            {fixLetterOrder(x_rotation(state)), "x Rotation"},
            {fixLetterOrder(x2_rotation(state)), "x2 Rotation"},
            {fixLetterOrder(x3_rotation(state)), "x' Rotation"},
            {fixLetterOrder(z_rotation(state)), "z Rotation"},
            {fixLetterOrder(z2_rotation(state)), "z2 Rotation"},
            {fixLetterOrder(z3_rotation(state)), "z' Rotation"},
            {fixLetterOrder(m_mirror(state)), "M Mirror"},
            {fixLetterOrder(s_mirror(state)), "S Mirror"},
            {fixLetterOrder(e_mirror(state)), "E Mirror"},
            {fixLetterOrder(m_mirror(s_mirror(state))), "M + S Mirror"},
            {fixLetterOrder(s_mirror(m_mirror(state))), "S + M Mirror"},
            {fixLetterOrder(e_mirror(m_mirror(state))), "E + M Mirror"},
            {fixLetterOrder(m_mirror(e_mirror(state))), "M + E Mirror"},
            {fixLetterOrder(s_mirror(e_mirror(state))), "S + E Mirror"},
            {fixLetterOrder(e_mirror(s_mirror(state))), "E + S Mirror"}
        };

        // Debug output: Show transformed states
        for (const auto& [transformed_state, transformation_desc] : sym_states) {
            //cout << "🔄 Transformation: " << transformation_desc << " -> " << transformed_state << endl;
        }

        // Search in the dataset
        for (const auto& [transformed_state, transformation_desc] : sym_states) {
            auto it = state_to_scramble.find(transformed_state);
            if (it != state_to_scramble.end()) {
                string original_scramble = it->second; // Retrieve original scramble
                solution_found = true;  // ✅ Mark as found

                solution_counter3++;  // 🔥 INCREMENT SOLUTION COUNTER HERE

                cout << "\n✅ Found solution!" << endl;
                cout << "🔹 Given scramble: " << scramble << endl;
                cout << "🔹 Extra move applied: " << moves_applied << endl;
                cout << "🔹 Applied transformation: " << transformation_desc << endl;
                cout << "🔹 Matching dataset scramble: " << original_scramble << endl;
                cout << "🔹 Final solution sequence: " << moves_applied 
                    << " " << transformation_desc << " invert(" << original_scramble << ")" << endl;
                cout << "🧩 Final cube state (for debugging): " << transformed_state << endl;

                return;
            }
        }
    }

    cout << "\n❌ No solution found for scramble: " << scramble << endl;
}

void find_solution_with_4_extra_moves(const string& scramble, const string& search_filename, const vector<string>& moves) {
    if (solution_found) return;  // Stop if a solution was already found

    cout << "\n🔍 Processing scramble: " << scramble << endl;

    unordered_map<string, string> state_to_scramble;  // Maps cube state -> original scramble
    ifstream file(search_filename);

    if (!file) {
        cerr << "❌ Error: Could not open " << search_filename << " for reading!" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        size_t state_start = line.find('a');
        if (state_start != string::npos) {
            string scramble_seq = line.substr(0, state_start - 1); // Extract scramble
            string state = line.substr(state_start); // Extract state
            state.erase(remove(state.begin(), state.end(), ' '), state.end()); // Remove spaces
            state_to_scramble[state] = scramble_seq; // Map state -> scramble
        }
    }
    file.close();

    cout << "✅ Loaded " << state_to_scramble.size() << " unique cube states from file.\n" << endl;

    // Base state (after applying input scramble)
    string base_state = translate_scramble_to_moves(scramble);

    unordered_map<string, string> temporary_branches;

    // Generate moves of depth 1, 2, 3, and 4
    for (const string& move1 : moves) {
        string scramble_1 = scramble + " " + move1;
        string state_1 = translate_scramble_to_moves(scramble_1);
        temporary_branches[state_1] = move1; // Store only the extra move

        for (const string& move2 : moves) {
            string scramble_2 = scramble_1 + " " + move2;
            string state_2 = translate_scramble_to_moves(scramble_2);
            temporary_branches[state_2] = move1 + " " + move2; // Store only the extra two moves

            for (const string& move3 : moves) {
                string scramble_3 = scramble_2 + " " + move3;
                string state_3 = translate_scramble_to_moves(scramble_3);
                temporary_branches[state_3] = move1 + " " + move2 + " " + move3; // Store only the extra three moves

                for (const string& move4 : moves) {
                    string scramble_4 = scramble_3 + " " + move4;
                    string state_4 = translate_scramble_to_moves(scramble_4);
                    temporary_branches[state_4] = move1 + " " + move2 + " " + move3 + " " + move4; // Store only the extra four moves
                }
            }
        }
    }

    cout << "\n🌀 Generated " << temporary_branches.size() << " temporary branches.\n" << endl;

    // Apply normalization and symmetry transformations
    for (const auto& [state, moves_applied] : temporary_branches) {
        unordered_map<string, string> sym_states = {
            {fixLetterOrder(state), "Original"},
            {fixLetterOrder(y_rotation(state)), "y Rotation"},
            {fixLetterOrder(y2_rotation(state)), "y2 Rotation"},
            {fixLetterOrder(y3_rotation(state)), "y' Rotation"},
            {fixLetterOrder(x_rotation(state)), "x Rotation"},
            {fixLetterOrder(x2_rotation(state)), "x2 Rotation"},
            {fixLetterOrder(x3_rotation(state)), "x' Rotation"},
            {fixLetterOrder(z_rotation(state)), "z Rotation"},
            {fixLetterOrder(z2_rotation(state)), "z2 Rotation"},
            {fixLetterOrder(z3_rotation(state)), "z' Rotation"},
            {fixLetterOrder(m_mirror(state)), "M Mirror"},
            {fixLetterOrder(s_mirror(state)), "S Mirror"},
            {fixLetterOrder(e_mirror(state)), "E Mirror"},
            {fixLetterOrder(m_mirror(s_mirror(state))), "M + S Mirror"},
            {fixLetterOrder(s_mirror(m_mirror(state))), "S + M Mirror"},
            {fixLetterOrder(e_mirror(m_mirror(state))), "E + M Mirror"},
            {fixLetterOrder(m_mirror(e_mirror(state))), "M + E Mirror"},
            {fixLetterOrder(s_mirror(e_mirror(state))), "S + E Mirror"},
            {fixLetterOrder(e_mirror(s_mirror(state))), "E + S Mirror"}
        };

        // Search in the dataset
        for (const auto& [transformed_state, transformation_desc] : sym_states) {
            auto it = state_to_scramble.find(transformed_state);
            if (it != state_to_scramble.end()) {
                string original_scramble = it->second; // Retrieve original scramble
                solution_found = true;  // ✅ Mark as found

                solution_counter4++;  // 🔥 INCREMENT SOLUTION COUNTER HERE

                cout << "\n✅ Found solution!" << endl;
                cout << "🔹 Given scramble: " << scramble << endl;
                cout << "🔹 Extra moves applied: " << moves_applied << endl;
                cout << "🔹 Applied transformation: " << transformation_desc << endl;
                cout << "🔹 Matching dataset scramble: " << original_scramble << endl;
                cout << "🔹 Final solution sequence: " << moves_applied 
                    << " " << transformation_desc << " invert(" << original_scramble << ")" << endl;
                cout << "🧩 Final cube state (for debugging): " << transformed_state << endl;

                return;
            }
        }
    }

    cout << "\n❌ No solution found for scramble: " << scramble << endl;
}




void find_solution_with_extra_moves(const string& scramble, const string& search_filename, const vector<string>& moves){
    // "normal" pruning variant:
    /*find_solution_with_0_extra_moves(scramble, "all_five_move_scrambles_unique_states.txt");
    find_solution_with_1_extra_move(scramble, "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_2_extra_moves(scramble, "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_3_extra_moves(scramble, "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_4_extra_moves(scramble, "all_five_move_scrambles_unique_states.txt", moves);*/

    // "smartbranch" pruning variant:

    find_solution_with_0_extra_moves(scramble, "derivation_5_to_6.txt");
    find_solution_with_0_extra_moves(scramble, "derivation_6_to_7.txt");
    find_solution_with_0_extra_moves(scramble, "derivation_7_to_8.txt");

    find_solution_with_1_extra_move(scramble, "derivation_7_to_8.txt", moves);
    find_solution_with_2_extra_moves(scramble, "derivation_7_to_8.txt", moves);
    find_solution_with_3_extra_moves(scramble, "derivation_7_to_8.txt", moves);
    cout << "==========================================";
    solution_found = false;
    //find_solution_with_4_extra_moves(scramble, "derivation_6_to_7.txt", moves);

}





// Main function
int main() {
    auto start_time = high_resolution_clock::now();

    vector<string> moves = {
        "R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'", 
        "U", "U2", "U'", "D", "D2", "D'", 
        "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'", 
        "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", 
        "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"
    };
    //string initial_state = "012345678901234567890123";  // Solved cube state
    string initial_state = "aaaabbbbccccbbbbccccaaaa";  // HTR-solved cube state




    // FIRST WORKING SOLVER:


find_solution_with_extra_moves("R' F U R' B' R B D2 L B R2 L2 F' D2 B2 R2 D2 B U2 F' R2 Uw2 R2 B' D2 Fw2 Rw2 B' R D2 L2 B2 R Uw' F2 L' Uw B' R B Fw Uw B' Fw Rw' D2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' L U' R L2 F2 L2 U2 B L U2 R2 F2 R' F2 L B2 U2 L' D2 B2 Rw2 Fw2 R' B2 U2 F R B Uw2 Fw2 D2 Uw' B D' Uw2 L2 U Rw' B2 Uw Rw2 Fw L2 U2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D' B2 R' B2 D2 F B2 U' R U2 D2 F2 D' B2 R2 F2 U2 F2 D F2 Fw2 Rw2 F2 L' F2 U Uw2 L2 U' R' Fw2 D Rw2 Fw' Rw2 U2 R B2 Rw' Fw2 Rw U2 Uw B2 Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 R' D2 B2 L' U2 D2 R' L2 D2 R' D' L' F' U F2 R' D F' U' Rw2 U R' Fw2 U2 R2 U' R L' Fw2 U' D' Fw L D Fw2 Uw F2 Fw Uw L' Rw' U L ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F' R B2 U2 F U F' D' L' U2 L2 F2 U2 B2 L D2 L' D2 R2 B2 Uw2 L' Fw2 L2 Uw2 L' B' L2 D2 R B Uw L2 U2 R' Fw2 Uw' F2 Fw' U R' L2 Rw' Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L F2 B2 D B R' F D' R L2 F D2 B2 R2 D2 R2 F' L2 B2 D2 R' Uw2 F2 R Fw2 Rw2 B' U2 F' Rw2 Uw2 B L' Fw2 U' Uw' Fw2 R' D' Rw' Fw R L Fw U2 B ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F' L U2 R D2 F2 R' F2 L D2 L' B2 L F L U' L2 D2 L2 B' R Fw2 R' Uw2 R L' D' Rw2 F2 D' R Fw2 Rw2 U Fw' L2 Fw2 Rw2 B' L F2 Rw' Uw' B U2 F2 Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 L' U2 L B2 D2 F2 D2 F2 L F' R' U R2 B' L2 F2 U F L' D Uw2 Fw2 U' L' U' B2 L' D' Fw2 U R' L Fw' L U R2 Rw2 D Fw2 Rw' U F' D2 B' Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R D' R L F' D2 L U F L2 U L2 U D2 B2 D F2 U' R2 L2 B2 Uw2 B Uw2 Fw2 D R2 B U D R2 U2 L F' Rw B' D R2 Uw' B Fw2 R Rw B Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F' L F2 D2 B' D2 R2 F' B2 L2 U2 F R2 L D' R B2 U D2 R' F2 Fw2 L F2 Uw2 R B' Uw2 F' U2 B' Fw2 R Uw F' R' D' R B2 Fw U R Uw B R' F ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L' B2 R D2 R2 B2 R' F2 L2 U2 D R B' R L D' B U B L' Rw2 Uw2 Fw2 L' B2 Rw2 Fw2 D2 L' F' D2 Fw2 R Uw' F L F2 B Fw2 R2 Rw' Uw' Rw U B2 D' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B2 D R2 D2 F' D2 B U2 B D2 B D2 R2 D' R B2 L F' U L2 D2 Fw2 R' Rw2 Uw2 B U2 Rw2 Fw2 L Uw2 L F2 Uw' L' F' D' B2 Fw U Rw2 D F2 Uw Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' D' F' D2 R2 F' U2 F2 R2 U2 F2 B' L2 D' R' L2 F' L D2 B L Fw2 L2 Uw2 F' R' L' F' Rw2 D2 L F R U Uw' L B U2 Uw2 L F Rw' F' Rw2 Fw L2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F R2 B L2 U2 F D2 F U2 F L2 F2 R B' L' D' R D2 B R2 D' Rw2 U' L2 Rw2 D L2 Fw2 U2 R' F2 U2 Uw2 Fw D' R2 Uw2 B2 Rw B2 Rw D2 B2 Uw L2 F ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' D F2 R2 F2 U B2 R2 D' F2 R2 U2 B D' R F D2 B R2 U' Uw2 Fw2 U B' Rw2 D B Rw2 U B2 U2 F' R Rw D' R' B R' Uw L2 Rw' Fw2 Rw2 F2 R2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D' R2 F2 L' U2 L' F2 R' U2 D2 B2 U2 L' B D L F R' L' U' R Rw2 B Uw2 L Fw2 R' U2 R2 B' L2 Uw F' B2 U' D Uw2 Rw F Fw' Uw Rw D' B' Fw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D B2 U L B2 U' L U F2 R2 U2 L2 F' L2 F2 R2 F' D2 B2 U2 Rw2 F D2 F Rw2 B D' Rw2 B' Rw2 D' Fw2 R U Uw2 Fw2 Rw D2 Uw Fw R' D2 L2 B' Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F R2 U2 R' U' B D2 R2 D' F D2 F R2 D2 F D2 F' U2 L2 B2 Fw2 D R B2 U B2 Fw2 L2 Fw2 L' Uw2 B' Fw2 U Fw U R Fw' Uw R2 L' B2 Fw' Uw2 Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D' L2 U2 R2 D' F2 U' R2 F2 U B2 D' F' L' D F U F' U F' B Fw2 D R' Uw2 F2 R2 Rw2 Uw2 F2 U2 L' U2 Fw Rw2 B' U2 L Rw2 F Rw Fw' Uw F' Rw U2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B2 D2 L' F2 D F2 U' F' R D L2 F2 B2 U' B2 D F2 B2 L2 D' Fw2 L Fw2 Rw2 U R' F2 D2 L2 Uw2 Rw2 U' B Fw Uw2 R' U' F B' Fw Rw F' Uw' F Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L D F' L F2 D R L' U R2 L2 D2 F2 U L2 D' L2 U2 Fw2 D R Rw2 U Uw2 Fw2 R' D' L U L Fw U2 Fw R F' L Uw' R' Uw2 Fw U D2 R ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F' L2 B2 D2 F2 L U L2 B2 U2 R2 F2 L D2 B2 R' F2 D2 F' D' L2 Rw2 B2 Fw2 R Uw2 B U2 F L' F' B2 Uw L Uw2 B U R2 U Rw' Fw' U2 Uw2 L' Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 B2 L2 D2 L2 D B2 D' F2 B2 R L2 U L D' B R2 D R F R2 Uw2 Fw2 D' F' B2 D Fw2 L2 F B U' Rw' D2 R2 U Rw2 B Uw R' B2 Rw U2 F2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U F2 B2 U L' B D' F D' R B2 R U2 R2 F2 R U2 R D2 F2 B2 Fw2 Rw2 Uw2 L' U Rw2 U2 R Fw2 R' Rw2 Fw U2 L2 B R2 Fw' R Uw R2 Fw' R2 Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' B' R' U2 L2 U2 R' B2 D2 R B2 L2 D' L D L2 U' D L2 Fw2 Uw2 B' Fw2 Uw2 L' D2 Rw2 Uw2 L B' Uw Rw2 D R' Uw2 Fw' Uw' B' Rw' U2 Uw Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 U2 L' F2 D2 L B2 D2 B2 L F R2 L' F2 D B2 U' F U D Fw2 D' L Rw2 U' Rw2 Uw2 Fw2 L2 F2 Rw2 U' D2 B U Fw L D' L' Uw L2 Uw2 Fw Uw Fw Rw' D' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 R2 B L2 F' D2 R2 F R2 F U2 B' L' F U R' B' U2 B D' Uw2 R F2 D2 Rw2 B2 D' Rw2 D' R' L Uw2 R B' Fw' D' Uw2 F' U Rw' Fw R2 Rw' Fw D' L' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 R' U2 F' R2 U2 F2 D2 F U2 B U2 D B2 L' U2 B2 L2 F2 Uw2 Fw2 R' F Uw2 R F2 Uw2 R' Rw2 F2 D2 R Uw L2 D L2 U' Fw' D' Uw2 Rw F Fw Rw2 B' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L U R2 F2 U D2 F2 R2 L2 U' D2 L2 F' R2 L B L F2 D R Rw2 D' F Rw2 Uw2 Fw2 U2 R2 F L2 U2 D2 F2 Rw' F D' Uw2 Fw2 U2 B' Rw Uw' Rw' U ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R D2 L2 F U2 F B2 R2 B U2 F R2 L' U R' D2 L2 U' R2 F' D' Fw2 U' Uw2 Rw2 F R2 Fw2 D2 Rw2 U2 F2 R2 F Rw' U' Fw2 L2 Rw' Uw' B Rw Uw2 Rw D2 Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U R' U2 B2 R' U2 R F2 U2 L D2 R2 F2 B U R B2 D R2 F' Uw2 B Rw2 D Fw2 Uw2 Rw2 D2 F2 U D' B' U Rw' F' U2 R2 F' Uw Rw2 D Fw' R' U ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R F2 L2 B' U2 R' D' F R2 D2 F' R2 U2 R2 B2 L2 B' L2 D' F' Rw2 B' U L2 Rw2 F2 U2 D' B' U' Rw2 D' B' Rw D2 B' Rw2 B2 U' Uw Rw' Uw2 F' Fw' U2 R2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 B' U2 B2 R2 F2 R2 U2 D B2 U' F2 R' L' D' R' U2 F U2 F' Uw2 Rw2 D R2 L Uw2 R' U2 F2 D Rw2 Fw2 R' Fw' U Uw2 Fw Uw2 F2 U Rw Uw2 Rw' D' Rw' Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 L2 U F2 U' B2 D F2 B2 L2 F2 B' R L' F R2 B R D' F D2 Fw2 D2 R D2 Uw2 F Fw2 L' Uw2 L U2 B2 Rw2 Uw F Rw2 U' D' Rw D2 Uw' L2 Fw' L' Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 B U' F2 L2 B R2 F L' B2 D2 B2 U' L2 U' L2 F2 D B2 U2 B2 Rw2 Fw2 U L Uw2 R' L U Uw2 R Rw2 U' F Fw2 U Rw2 Fw Rw Fw' D2 L' Rw' F' Fw2 R2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 D R' B' U D2 F2 R F2 B2 R2 U2 D' R2 F2 U B2 R2 B Uw2 R B' Uw2 Rw2 F Fw2 D2 R' F2 R2 B2 Uw' Rw2 F' R Uw' F' U F' Fw' Rw F' Fw U2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F' R2 L' U2 R B2 R' F2 L2 F2 R' B' U L D R F U R Fw2 Rw2 D2 R' B2 D L2 Fw2 D L2 B2 Uw2 L' Fw' D2 Fw' Rw2 B2 Fw2 Uw' F2 U Rw' Fw Uw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D L2 D2 F2 B2 R2 L2 D' R2 U R2 F U2 L' F R' U L F' R D2 Fw2 D' L2 D Uw2 B Fw2 L2 Rw2 F' B2 Rw F' L2 F U B Uw' R' F' Rw B L Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F L F2 D2 L F2 D2 F2 B2 R' L2 D2 L D F2 U F2 B' L' F L2 Uw2 L' U' L' Rw2 D' R2 Fw2 R' F2 R' L Fw R Rw2 D Fw L' B2 L Rw B Fw Rw2 B2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' U' L2 U2 L' B R' D B2 U F2 D B2 R2 U' R2 D2 L2 F' L' Uw2 L Rw2 F L2 F2 Fw2 U2 L Rw2 U2 Uw B Rw2 U D L' B Rw F' Uw Rw' D' F2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F U' R2 D' B2 D2 L2 B2 L2 U' B2 R2 B' D' R' U F' L2 D L' U Uw2 R Fw2 L' F2 L' Fw2 U Rw2 D' Uw2 F D F' Fw' L Rw' D' F R' Uw2 Fw R2 Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L D2 R2 U L2 B2 D' L2 F2 D B2 U L' D' F2 L' B D' F B' U Uw2 F2 Rw2 U' B2 Fw2 U2 B' D' B2 U Uw2 L' D2 B' Rw B U' Uw' L B Rw Uw' B' Rw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B' U2 R D2 B2 R' U2 F2 D2 R2 B2 L2 B' R' L' B L D L' U' Rw2 B2 Fw2 R F' R' Uw2 L2 F R2 Rw2 Fw2 R' Uw R' F L2 B2 D Fw' U Rw D' Fw2 L2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' B' R2 F2 R2 L2 F U2 D2 B' R2 F' L U' F B' R' L F2 U B' Uw2 F2 Rw2 U2 Uw2 F L B2 Rw2 F R L Uw' F2 B' Fw2 U' Rw' Uw F' R' B2 Fw' D' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' L' F2 L2 B2 U L2 B2 D2 L2 F2 D B R' F' U' D L B2 L Uw2 Fw2 R U Rw2 U L2 Rw2 Fw2 R U2 Fw U2 L2 Fw2 Rw2 F' D R' Uw' B2 Rw U2 Uw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' B2 R L U2 L' F2 R U2 L' D2 R U B D F U2 F2 R B Fw2 U Fw2 U2 L' Fw2 D2 L' Uw2 Rw2 D F B2 D2 Fw' Uw2 F' Uw' L' Fw2 R' D' F Rw' D2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B2 D' F' R2 U2 D R B2 U' L2 F2 B L2 F' U2 F' U2 L2 D2 B Rw2 Fw2 L' F L D2 R Fw2 R B2 Rw2 B Fw2 Uw' Rw2 U' R L' B' R' Fw' R' L' Uw Rw' Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R F' U2 D2 L2 F L2 U2 F2 B D2 F2 L2 D R2 F R' B2 U' R D Uw2 F Uw2 L Rw2 B2 R L2 Fw2 U2 R F Uw' L D2 F' Rw2 U Uw' B' Rw' Uw R' U F Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 F L2 D2 R2 F2 D2 L2 B L2 F' U2 D' R' B U' B' U' B R' L Uw2 F U Rw2 D R2 U' B' Rw2 U2 Rw2 U' L' Rw Uw2 Rw F R' Uw' Rw' U R B' Rw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R L2 U D' F L2 D F L2 F2 D2 R2 F2 R' B2 R' U2 D2 B Fw2 R' Uw2 B' L2 B2 R2 F2 Uw2 B2 D2 Rw2 Uw' B' U' F B' R2 Rw' Uw' Rw2 B2 Fw2 D' Uw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F B U D' F2 L F U F' U R2 U R2 L2 D' F2 D2 F2 D B2 L2 Uw2 Rw2 B U' Fw2 U2 R2 Fw2 Rw2 U2 F' U D Rw' Fw2 U' Rw' F' Fw' Uw R2 Fw L2 Fw L' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 B' U F D F L B' L2 F B2 R2 F2 D2 R2 U2 F' U' D2 L' B2 Uw2 L B' U2 Uw2 F2 R' Uw2 L2 B' Rw2 D2 L Uw R' L2 Rw2 B Uw Fw D' R2 Uw2 Fw U' R' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B2 R2 B2 U2 L' F2 L U2 R' F2 L' U L' B U2 R F2 L' D' F' Fw2 Uw2 B' Fw2 Uw2 F' R2 U' F U' B' R2 Rw B2 R2 F2 D B' Uw B2 Uw Rw2 F' R2 Uw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 U F' B2 U D L' B' R2 L2 B' U2 F' R2 L2 F2 D2 F2 U' R Rw2 D B' Rw2 U D2 Rw2 U B2 Fw2 D' F Rw' U2 Uw2 R L B Uw Rw2 Uw' Rw' U Fw' Rw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 B' R2 U' B2 U2 F2 R2 D F2 B2 R2 U2 R' D L D2 F D2 F L2 Rw2 F L Uw2 R B2 Rw2 Uw2 L F R2 Fw2 D2 Uw L' F' Uw' Fw2 D' Rw Fw Uw2 R2 B2 Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B2 D2 L' U2 L2 B2 D2 F2 D2 L U2 F' B2 D2 F' D' F B U L U2 Uw2 B R' Uw2 R Rw2 D2 Uw2 L' F L' Uw2 Fw2 D' Uw B' D2 Uw2 Rw' D2 L' Uw' Rw' B Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F' R' F2 U F2 R B' L' U R L2 B2 U2 F R2 L2 F2 D2 F' R2 F2 Fw2 R' Uw2 Rw2 D2 B Uw2 F2 R' F' B R Rw2 Uw B' D' Uw2 Fw' L2 Uw L F2 Uw F2 Fw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D B2 L U2 R2 B2 U2 R B2 U2 B2 L' U' D2 R F2 D' L2 F R' L Rw2 B2 D' F2 Uw2 Fw2 U' R L Rw2 U' D' B' Rw2 U' L' B Uw F Fw' Rw' Fw2 D' Rw B2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R F2 U2 L' U2 D2 L' U2 D2 F2 L2 F D' B R2 U2 R' B' D2 B2 L' Uw2 R2 F' Uw2 F L Rw2 F2 B' R Uw2 B Uw' B2 U2 B Fw2 Uw2 Rw' Uw' L' D Uw2 Fw' D ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 B2 L2 B2 D2 L2 U F2 R2 F2 R U2 B' D F' U2 B L F' B2 Rw2 Uw2 B R2 U2 D' Rw2 F' Fw2 D B L2 F2 R' U' Rw2 Fw2 Rw F2 Uw R Rw Fw U' F ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 L' U2 R2 F2 R' L' F2 U B2 D F R2 D' B R' B R Uw2 R B2 Uw2 F Fw2 L' Rw2 F B2 U2 Uw F2 B2 Uw R2 Uw2 Rw' U Fw Rw Uw' B' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F' R2 B L2 U2 F' D2 B2 U2 L2 F2 B' U' B2 U2 R2 B' R' F' R' L' Uw2 L' F2 Fw2 L Fw2 Uw2 R U2 B' U2 Uw2 F Uw' Rw2 F Uw L U2 F' U2 Fw U Rw Fw' U2 Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 F2 R2 B2 L B2 L D2 F2 R' F2 R' D' B' D L D' F2 R2 U Uw2 F' B2 L2 Uw2 R' B' L2 Uw2 Fw2 L F U2 Uw' R2 F' L' Uw L' U2 Rw' B' Rw2 U2 Fw L ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B U D' B R D2 F B2 D2 L D2 L F2 L2 D2 B2 U2 R' D2 F U' Fw2 U' L Fw2 U Fw2 R' U D' Rw2 F2 L' U Fw' L' B' U2 Rw D Rw2 Fw U Rw Uw' F2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 D2 B R2 F' R2 U2 B2 U2 D2 B U B2 U F' D R' D' F2 B Rw2 B2 U' L2 Uw2 L Fw2 U Rw2 D2 Uw2 R' Fw' L' Uw2 R D Rw Uw Rw2 F' D R2 B2 U' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B2 R2 B2 U2 L2 U' L2 D L2 F2 U R' B' R F' R' U' L' U B2 U Fw2 L U Uw2 R' Fw2 R' L2 U Uw2 F2 Rw2 Fw U2 D F Fw2 Uw B L' U2 F Uw' Fw' U ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B R' L2 B R2 U2 F' R2 F' D2 L2 U2 F2 R' U' F2 R2 L' F D R' Fw2 L2 U' L2 Fw2 U' Rw2 B' U' Fw2 Uw2 Rw' U2 F L' D2 L Rw' Uw' Rw2 Fw' Uw F R2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B D B' R D' L2 U' B L' U2 B2 L2 U D2 L2 U2 L2 U F2 B2 Rw2 F D' L2 Fw2 U' D B' R2 Fw2 U2 B2 Rw B' Fw2 U2 B R2 F2 Uw F2 D Rw D' Fw2 Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 F2 D2 R2 F L2 D2 F D2 R2 F' D R F2 U D' R' L B' D2 Fw2 Rw2 D' R' U D L' Rw2 D' L2 Uw2 R D Fw U' F2 D2 Uw B' R' Uw' L Rw D' Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B U' B2 L2 B' D2 F R U2 L2 D' L2 U B2 D F2 U R2 D2 F Fw2 L Rw2 F' L U2 B Uw2 L F' Rw2 F2 R' Uw' L U Rw2 U Rw' Uw' L' B' Rw2 Uw' Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L' D' R2 B2 R2 F2 U2 R B2 U2 B2 R2 U2 B L U2 D L U2 D Uw2 B L Fw2 L' B Fw2 L Rw2 D2 L2 F' Rw2 Uw' L U2 R' Fw2 U D Rw' U2 Uw Fw Uw B ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 F2 R2 D2 R U2 F2 R D2 L2 U B' R' L' B' R U2 F2 B Fw2 Uw2 R D2 R Fw2 L' U D2 L D B U' Uw2 Fw L' F D Fw2 Rw B D F2 Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 L2 D2 F R2 F' L2 F2 B' R' F B2 D' L' U F' B' D' R2 Rw2 Uw2 F D2 F2 Uw2 R D2 R' B L' Uw' B2 R' D2 L2 F2 Uw' Fw' Uw' Fw' Rw' B2 D2 Fw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 D B2 R2 B2 D' R2 U L2 D' F2 D F L2 F U B R2 B L' U Uw2 R L' U L' Uw2 L' Fw2 R U Fw2 Uw2 Rw2 Fw' L' F B R' Uw Fw' Rw F' R' Uw' L2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 B R2 L2 F2 R2 F' D2 R2 L2 B2 U' R B L2 U' D B2 L' D' L2 Rw2 Uw2 R F' L' F B' Rw2 U2 Uw2 F Uw2 L Uw' F2 R F' B Rw' Fw' Uw2 F2 L' Uw B' U' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R U L2 D2 F2 L' F2 R U2 L' D2 L2 F2 L' F' L U2 F D F B' Uw2 Fw2 Rw2 Uw2 F' U R2 Rw2 D2 Rw2 F2 U' Rw F U R Rw' U Fw L Uw Fw D2 B2 Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L' D2 R2 D2 F2 R2 B D2 B' L2 D2 B2 U D2 B' R' B L2 D2 L' D Uw2 B Rw2 B' Fw2 L2 U2 L2 U Rw2 D' L2 Rw U F Fw2 Rw U D2 Fw' Rw' D' R' Fw2 D' L' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 L2 B2 L2 U' F2 R2 U' F2 D' B2 R B' U2 F R' U2 L' B L' Fw2 L2 Uw2 R' F Fw2 D2 L' F' D2 Rw2 Uw F' Uw F2 U2 L Rw Uw F Fw' L Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 F' R L2 U2 F2 D2 F' B2 U2 B2 R2 U2 R' L2 U D2 R' F2 U2 D' Uw2 R' B' Rw2 Fw2 R2 B' Fw2 Uw2 B2 R' U' Uw' R2 F L' F2 Fw L Fw2 R U' Uw' R' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 L2 F' L2 F R2 F' U2 F2 L2 U D2 L' D2 F2 D2 F' R2 F2 L Rw2 Uw2 F' B2 L' D2 Uw2 F' U2 L Rw2 Uw B2 R B U' D2 Fw L2 Fw Rw U' Uw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D' R' F U' L' F U2 F D' R2 U2 L2 B R2 F L2 F U2 B2 L2 Rw2 Uw2 L' F' Uw2 F2 R' Uw2 L2 Rw2 D2 R Uw' R2 Uw R' Uw' R2 Fw2 Rw Fw U2 Fw D' F2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D L U' L F' U2 D R D' B2 D F2 R2 D' R2 D' R2 F2 D2 L2 Fw2 Uw2 F D2 R2 B R' Uw2 Rw2 Fw2 L2 B2 Uw' Fw2 Uw F2 U2 F2 Rw' D2 Uw' Rw' Uw' L Rw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D R' U' L2 F B' L B2 D' L2 U' B2 U L2 B2 R2 U2 B2 U R B2 Uw2 R' Fw2 D Uw2 L2 Rw2 D' R F2 Fw2 Uw2 Rw2 Fw' D' B2 Fw Uw' Rw F2 U2 D' R Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 F2 U2 F' R2 F L2 F2 L2 D2 R D L' U D2 L' D2 F' B2 D Rw2 U2 F' Fw2 R' Rw2 B' Fw2 Rw2 D2 R Uw R' U' R B2 D Fw' U2 D2 Uw' Rw Fw R B2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' B2 D2 R' B2 L2 F2 D2 R2 L B2 D2 B' D F U D2 L2 D' Uw2 Rw2 B' Fw2 D2 B2 U L2 Rw2 B Uw2 L2 F L' Rw' Fw2 D' R2 Rw' Fw Rw' Uw' F' L2 Fw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 R2 F' D2 B2 L2 B' D2 L2 B L2 U R' D2 B L U D2 L2 F2 Rw2 D Uw2 Rw2 U2 L' U2 D' F2 Fw2 U L Fw L2 D2 F L D2 Rw' Fw' L F2 Uw' B2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' F2 D B2 L2 D2 F2 D B2 D' R2 D2 B' U2 L B' U2 L' D R2 Uw2 Rw2 B' D2 R' Uw2 F L' F B2 R Uw L' F U2 L' Uw' Rw' Fw L Uw Rw D L' U' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D F2 D2 R F2 L' F2 D2 R' F2 L2 D2 F' U D2 L' B' L2 U L' Uw2 L' Fw2 U' D' L Fw2 U L U2 L B2 Fw' D2 R2 F' D B' Rw' Uw2 Fw L2 U Fw2 Uw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D' R2 U2 R U2 R2 F2 L' D2 B2 L B U2 L' B R2 F2 U Uw2 L' Uw2 B L2 Uw2 F R2 D2 B Uw2 R L Uw R Fw2 U D2 Uw' Fw' Uw L Fw2 L' D2 Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' B' U' F2 U B2 U2 L2 U' F2 R2 D L2 U2 L D2 F2 U2 R' U2 L' Fw2 U Rw2 F D F2 B D Uw2 F2 Fw2 D' Fw2 Rw U D F' Rw' D' Fw' Uw' B2 D' Rw' Fw2 Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R L2 F L' F B D R' U' F2 R' F2 R' U2 F2 U2 R2 F2 L2 Uw2 B' L D2 R Rw2 Uw2 Fw2 U2 F' Rw2 Uw F L F B' Uw L' Uw Rw U2 Uw L2 Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F' B2 D2 L B2 U2 R' U2 R U2 B2 L D F2 D2 L D' B' D' F Uw2 Rw2 B2 R D2 Rw2 Uw2 F2 L2 B R Uw' B' L B' U' Uw' Fw L Uw' F D Fw2 Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 B2 L2 B2 L' B2 R' D2 L' D2 R' D2 B' L2 U L2 F' D R' D2 B2 Rw2 F' Uw2 Rw2 B' L B' Uw2 F2 B' L' Fw2 R Uw B' R2 Rw' F' L' D' Uw L Rw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 B2 R' D2 R2 B2 L' U2 B2 L2 D B' L' D2 R' U' D2 R D L Uw2 Fw2 L B Rw2 B R' F2 U2 F L2 Uw' R' F' R L2 Fw2 D Fw' Rw' F D2 Uw' F2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 B' L2 U2 D2 F R2 U2 B' R D B' L2 U2 B' L B L Fw2 Uw2 L D F2 L' U2 Fw2 R' U' Fw2 U Fw Uw2 F' R' D' Rw2 Fw Uw' Rw U' B Rw F ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L B R' F2 B2 L D2 R L F2 B2 U2 F U B2 R' U L2 U R' Fw2 Uw2 L' B2 U2 R2 D2 F Uw2 F' Rw2 D Uw' F' B' U Fw2 R Fw U' Fw' U Uw Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F D2 R B2 U2 R U2 L' B2 L B L D2 B' D F U' B' U2 Uw2 L F2 U' Fw2 R U2 Rw2 B2 L2 U' D2 Fw D' L Uw2 R2 U' Fw' Rw2 Uw' L U2 B' Rw2 F' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 F L2 D2 B L2 D2 F2 B D2 F' R2 L' D R' L B' U L2 B' Fw2 L D R' Fw2 L' D2 Rw2 D' B2 L Fw' Uw2 L D F2 Rw2 Uw Rw F2 Fw' Rw Uw' F' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F L2 F2 U2 L2 U2 B R2 F' D2 B' D' B' U2 B' D2 L D' R D Fw2 Uw2 F' R2 L F' R2 Uw2 R Fw2 Rw2 D2 F2 Uw L2 Uw F2 Fw' Uw' R2 D' Uw' L2 F ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F' D' F' D2 B' L' D' F' U2 D F2 D2 R' D2 B2 R' F2 L' F2 Fw2 U F2 Uw2 L' Rw2 D' R' Rw2 Uw2 R L D2 Fw' U' B' R' Uw Fw' Uw F2 Fw Uw2 R' U' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L' U2 B2 L U2 R B2 R2 L' D2 L' B L' D' B U2 B' U' B L2 Fw2 D' R' Rw2 U L Fw2 Uw2 Rw2 Uw2 L2 F2 U2 Fw' R2 B' D' R2 Uw' R' L2 Rw2 Fw' Uw' Rw' B2 ", "derivation_7_to_8.txt", moves);
/*find_solution_with_extra_moves("R2 L2 D' F L' U' D2 L' B' D2 L2 B' U2 B' U2 F' D2 F R2 B' Rw2 B' Fw2 U Fw2 L2 U2 B2 L2 D' B' Rw' F Fw2 U' Rw2 F L2 Rw' Fw' L2 U ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F R' L2 D' B2 R2 B2 U2 F2 D' L2 U2 D' F' R L' D' F2 B Rw2 U' D2 Fw2 R L D' F2 Rw2 D Fw2 D2 F' Fw' D' L' D Fw' Uw' Rw' U F2 D' Rw' B ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 B' R' F L2 D L2 D2 R B2 U2 B' L2 F2 R2 D2 R2 F L2 F' Fw2 D' Uw2 L Rw2 B2 U' D' Fw2 Uw2 L U Fw L F2 L2 F2 Uw' B2 L' F' Rw2 Uw Fw R ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R B2 D2 R' U2 R2 F2 U2 F2 R U2 R F R' F' B2 U R D' B2 D' Rw2 D' Uw2 R B2 D' L Rw2 B2 D' Rw2 D R2 Fw' L U2 Uw2 L U2 B2 Uw Fw Uw' F B2 U2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D F' B' R F2 B' D' F2 R' B2 D2 R2 L' U2 R F2 L F2 D R F Fw2 R' Rw2 Uw2 R U2 D2 Rw2 U' Fw2 R2 Fw' R' B' U F' Uw' F L2 Rw Fw2 Rw' F Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D' F' U2 L2 D2 L2 B2 R2 U' F2 L2 D' L2 B' L F' U2 B2 R' D' Fw2 R' U' Fw2 L' U2 F2 Fw2 U' D' Fw2 U' Fw' D Rw2 B' D' Fw' Rw' F2 D' Uw Fw' R B ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' D2 R2 B R2 F U2 B L2 F' R2 F U' L2 B' R U' R L B2 U Fw2 Rw2 F U D2 B Fw2 D F2 L2 D2 Rw' U Rw2 D R2 Uw' B2 Fw R' Uw' R Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 D' B2 U F L B D' L2 B2 D' R2 U L2 F2 D F2 R2 U2 R' F' Rw2 F' D' R2 Rw2 F' U' Fw2 D' Rw2 B' D' Rw D B' U' F2 R' Uw Fw' L2 Rw' F Uw' B ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 F2 B' D B2 U' L2 F2 R2 B2 D2 F2 D L F2 B U F' L D' R2 Rw2 F' D Fw2 U Uw2 F' U2 F2 U2 F2 Rw' D2 L' F2 U2 R Fw U2 Rw Uw Rw' Fw Uw' L ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L' U' R' L F' R2 L' D' R' F2 L U2 F2 L' D2 L2 U2 D2 F2 Uw2 L' F Fw2 Uw2 R' B Rw2 F R' F' Uw' B' R U F' R' L2 Rw2 Fw' U2 Fw Rw' Fw' U' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' F2 D2 L2 U' B2 R2 F2 R2 U F R' F' U' L U' B D2 R2 Fw2 R' F2 Fw2 D' Fw2 U L' Uw2 Fw2 R2 B2 Uw2 Fw' L' D2 R2 Rw Fw R2 U2 F Uw' L' F' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 B' R L' B2 L' U2 L B2 R' B' L' D2 R2 D F2 B2 D2 Uw2 R B Uw2 L2 Fw2 Rw2 Fw2 R2 D2 B U2 Uw F2 U2 B L Fw' L2 Uw Rw2 Uw2 Fw U L' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B' D2 R2 B2 U2 B R2 B U2 F' B' D' L2 D2 R' B D2 L D2 B2 U Fw2 Rw2 F U' Fw2 U2 L2 Uw2 B2 D Rw2 B' D' Rw' F D Uw2 F' Uw2 L2 Fw' L2 Fw2 D' Uw' R' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L B2 U2 F2 R F2 L D2 R U2 F' R' L2 F' U R2 L U' R' Uw2 R Uw2 F Uw2 R F' R Uw2 R L B Uw F2 B' Fw2 Rw Uw' F' Fw2 Rw Uw2 B2 D ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' U R2 L2 F' L2 B' U2 F U2 L2 F2 R' L2 B L B D R2 B' Fw2 L Rw2 F2 D' R D' Rw2 U2 Uw2 F2 U Fw L2 Uw2 F2 U Rw' B2 Fw2 U' Fw Rw D' Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L B' U F R' L2 U2 L U2 F D2 F U2 F' D2 F2 U2 D2 F' L' Rw2 U' B' Uw2 B2 U' Fw2 U B U R2 Rw' D B' R Rw F Fw L2 Uw' B2 R2 L' Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U R' U2 F2 B R2 F D2 L2 D2 L2 B R2 F U B' D2 F U D' F Rw2 F U2 F2 Uw2 R F2 L2 Uw2 B U2 L2 Uw Rw2 Uw R2 F' R' Fw' U Rw' D B U' D2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U D2 B2 L2 U2 D' F2 D' R2 D' B2 R' L D' B2 R' B' U' L' D2 Fw2 U R Rw2 B2 Uw2 Rw2 U' Rw2 D' L U B' Fw D' B2 L' U' Rw' Fw L2 B' Rw D F2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U B' U' F U' L B L2 U2 R2 L B2 L' U2 L F2 D2 B' U' Fw2 R' D' Rw2 B2 Rw2 U R2 D' Rw2 D R' L' Fw R B' D L Fw2 Uw F2 Fw U2 L2 Uw Rw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D' L' D2 B2 D2 F2 R D2 B2 L2 B2 R2 B2 U B U2 R' F' L D Rw2 F' D Uw2 F2 Rw2 F B' R2 U' F2 B' Rw' F' B2 R U' Uw' F Rw' F' Uw2 Fw2 R ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B R D2 L' F L2 B' R' U2 D R2 U B2 D2 F2 L2 U L2 D2 F Rw2 F L2 Rw2 F' U2 Fw2 R' D2 Uw2 F2 Fw2 U D' Uw' B2 L U' Uw' Fw Uw' Fw2 D R L2 Fw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L F' R2 F' L2 U2 F B2 D2 R2 B2 D' L B' U' B' R F R L' Uw2 B' R D2 Uw2 F2 R' D2 B Uw2 L Uw R U' Rw2 B Uw2 Fw' L' U B Uw2 Fw' Uw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F' U2 F' R2 B' R2 D2 F' U2 L2 D F2 R B U R' U2 D F2 Fw2 L Rw2 D2 B U2 Uw2 L' Rw2 F R' Uw2 Rw2 Uw F2 Rw2 B L' Uw2 Fw' Uw B L' U2 Rw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B2 D B' R U F2 R2 L B' U2 F2 D2 B2 L' B2 U2 R L F2 D2 R Fw2 D L2 Fw2 R' D' L U' Fw2 Uw2 F2 U' Fw' L2 D' Rw2 Uw' F2 Uw Rw' B' Uw2 R' U' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 L F D2 F2 R2 L2 F' R2 D2 B D2 F R' D B' L B' U' B Fw2 U' L2 U Rw2 D' Fw2 L' U R' F2 Fw' U' L D2 L B2 Rw' Uw B2 U2 Fw' Uw2 B Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U D2 B' R2 L2 B' U2 F' U2 B2 R2 B D L U D2 F D R B' U' Uw2 F' L2 F' Uw2 B' R Uw2 Rw2 F' Fw2 R U2 Uw' R' U Uw R2 F2 Rw Fw L2 D' Fw Rw' U ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 B2 R' F2 D2 R' L2 F2 L2 U2 R' F D L2 B2 L' D2 R B2 R2 Rw2 U' D2 Uw2 L B2 Fw2 U Rw2 U2 Fw2 Uw2 B2 Fw U' D R' B Uw' B2 Rw' F' R2 Fw U ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F L2 U2 D2 F' U2 F' D2 L2 F2 R2 F' R' F R' F2 L' U2 F' U' F' Fw2 Uw2 R' D Fw2 D Rw2 U2 F2 L2 B2 Uw2 R Fw' L' U F2 Uw' R2 L' Uw L' Fw D' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 D' R2 L2 F2 D' F2 B2 U' R F B U R' B' U' D2 F' R Fw2 Uw2 F D' Rw2 F Uw2 F Uw2 L2 U' Rw B2 Rw B' L2 U' Uw Rw2 B Rw Uw2 Fw2 R2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D' F' D' F B' U F B2 L D2 L' B2 R B2 D2 L U2 L B U Fw2 R F2 B' R' D2 Uw2 L' Fw2 L B' R Uw F B2 R2 L' Fw2 Rw F' D B' Rw' D2 F ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B' L2 F U2 D2 F' R2 F2 B' R2 B' R U' R B' L D' B' R2 F' U' Fw2 D F L2 Uw2 L2 U F' Fw2 R2 F U' Rw D F2 D2 L' B2 Fw' R' Fw' Rw B Fw' Uw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 F' R2 U2 F' U2 B2 U2 L2 B R2 D' L2 B2 D2 R' F' R2 U B2 Rw2 U' Fw2 R Fw2 U R2 B2 Fw2 Uw2 L2 F2 D' Fw' U Fw' L Fw' Rw2 Uw' R Rw Uw' Rw2 Uw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L F2 L' D2 R F2 R' L' D2 L' B2 D' F' B D' L2 D' R B2 R D2 Rw2 D' R Uw2 Fw2 U2 Rw2 U' L U2 Rw2 B Fw' L' Rw2 Fw' L2 Uw Fw' R Uw2 F2 Rw Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 R' F D2 F2 R2 U2 L2 F' U2 B' U2 B' R U R' U B' U R2 Uw2 Rw2 D R B2 Fw2 U R' D R2 F B2 Fw' D2 R' Uw2 Rw' Fw R F' B2 Uw2 Rw2 D ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B' D2 R F2 L2 D2 B2 D2 B2 L U2 L2 U2 B U' L F2 D' F L F Uw2 R2 F' D2 B Fw2 L' Uw2 Rw2 U2 B Uw2 F' Uw' L U' B2 R' D2 Rw Uw F Uw' Fw R Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 F D2 F U2 R2 L2 B R2 F B2 L U' B2 R' F U R' L' D' Rw2 Fw2 U' Uw2 B2 L2 F2 U Rw2 B Uw2 R D Fw2 Rw' F' R' B Uw' Fw' Uw2 R' U' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F R U L' U2 F' R2 B2 D R2 B2 R' F2 R U2 D2 L' U2 F2 L' Rw2 D Uw2 L F2 D2 L2 Fw2 U' Uw2 Rw2 U' R' B' Fw D2 Uw2 Rw' Uw' L2 Rw2 Uw L' F Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 L2 U' L2 F2 U F2 R2 U' F B2 U R' U F B R' B D Uw2 L' F2 D2 Rw2 D Fw2 R Fw2 D R' L2 Fw' U2 L' Fw D Rw U2 Fw' D Uw2 L2 Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 L2 U2 R2 D' L2 U L2 F2 D F2 D' F D' L' B2 U2 F U B' R2 Rw2 F' Rw2 U' R2 B' Rw2 F2 L2 B' Rw2 D2 F' Rw Fw2 U' L2 B' Rw2 B' Uw B2 Fw' Uw2 Fw R' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 L U R' U2 F2 L' F2 U2 R D2 F2 B2 U' R2 B' L2 F' R F Rw2 Uw2 F R' B2 L2 U2 D2 Rw2 B Uw' L2 F Uw' R2 Fw U Rw D Uw2 Fw' D L2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 D2 F U2 R2 B L2 F' R2 B' U2 R B2 U' B2 R2 D L U' R Rw2 Uw2 F' Fw2 L2 B Uw2 F' U Rw2 B2 Rw B' L D Rw U' D' Uw' Rw' B' U' Uw Fw2 L2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 D2 B2 L2 U F2 R2 D2 F2 D F2 B' R' D' F' R' U' L' D' F R Rw2 Fw2 U' D2 Rw2 U L' U Fw2 L U' Fw' U' Fw D' F R B Rw' Uw Rw2 D2 B' R2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 B R U2 R' B2 U2 L' U2 L F2 L U2 D F2 L B' R U2 F2 L2 Fw2 D R Rw2 Fw2 R' D' R' U' B2 L' Rw2 U' Fw U2 D2 L Uw2 Rw D' L2 Uw' R Fw' Uw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L' D' R2 B R2 F' U2 D2 B' D2 L2 F2 D L F2 U2 D' F2 L' B2 Fw2 Rw2 D' F Uw2 F' B' Rw2 F' D B' Fw2 U Rw' B' R' D Uw2 F L' Uw F B2 Rw' Uw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L F D R2 D' R2 U' F2 D2 B2 D' R2 F2 L' D B2 D2 F' R2 U' L Fw2 R' U D' Uw2 Fw2 Rw2 D2 R' Fw2 D' F2 Fw R D2 B2 R2 B2 Uw2 Rw Fw D2 Uw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B2 D' L' B L' U D' R2 B L2 D' F2 B2 D' B2 U' D' F2 D' R2 Rw2 D' Fw2 D2 B D' L2 D' Rw2 B' L' Rw2 D' Rw F L Uw2 Fw' Uw' F2 Uw2 R2 B ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 U2 R' B2 R D2 R2 F2 D2 L B D' F2 D2 L D B2 R F' Rw2 Fw2 U B' R2 Rw2 D' F B D' Fw2 Rw U D Rw U2 Rw Fw Rw' B U Uw' L Rw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F U L' F2 D2 F2 B U2 F L2 D2 L2 U2 F2 R F2 L' B' L' F' U Fw2 R F' Uw2 F' Uw2 L Fw2 R D2 R' L' U Uw' L2 F' Uw' Fw' Rw F2 Fw' Rw' D L ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 F D' F' R2 F2 L' U R2 D2 R2 F D2 F' U2 L2 F U2 R2 Fw2 L F2 Rw2 Uw2 B D2 R2 L' F2 Rw2 F' U Uw2 Rw2 B' Uw F' Uw' Fw' U2 Rw F D2 Uw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 F' L F2 R D2 L2 F2 B2 L F2 R' U2 L2 F B L D B' R2 U Fw2 R' U2 Rw2 D2 F' R' B L Uw2 L' D2 Fw2 Uw Fw2 D R' D' F' Rw D' Uw' Rw F' Uw' R ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 R' U R2 B2 D2 B2 L2 D2 R2 D' R2 U L2 B' R2 D' L F2 B' R2 Fw2 U' Rw2 Uw2 L F2 U2 Fw2 L Fw2 Rw2 Fw' Rw2 U Fw2 R Uw Fw2 U' Rw' F U Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' F2 R' D' R2 U' F D' R2 L2 D2 R2 D F2 U' R2 U' B2 U L Uw2 B' D Rw2 F2 U R2 D Fw2 U' F Fw2 R2 Rw B' U2 F2 Uw2 L' F2 Fw' R L D' Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B R' U' D F' R D2 L2 B R' L F2 U2 F2 B2 L' U2 R' F2 U2 R Rw2 U R Fw2 R' Fw2 D2 R L Fw2 U Fw2 D' F Fw R Fw2 R2 F' L' Rw Uw R' D2 Fw2 Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 F' U' B2 L2 D2 R B2 L U2 R2 F2 R F2 B U F L' F2 B2 U' Fw2 L Uw2 L D2 F' R F2 B Uw2 F' B' Uw R' U B' D' Rw2 Fw' L2 Fw L' Rw' F R' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D' F2 L2 D2 B2 R2 D' L2 U2 R2 U' B L' F B U L2 B' U' D R' Uw2 Fw2 U F2 L' Fw2 L' Rw2 B2 L B2 D' L Fw D' Uw2 Rw' D' R Uw R2 Uw R Rw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 F2 B2 U2 R' L2 F2 U2 L' B2 D2 R B' D2 L' B' L' D' B U D Rw2 Fw2 L' U2 Rw2 Fw2 U Uw2 R' U F2 Uw2 B2 Fw' R2 U' R U' Uw' R Rw' D2 F L' Uw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' L' F2 R' B D' F' L F' U D B2 R2 B2 D L2 D F2 R2 D2 R2 Fw2 D2 R D F2 L Fw2 U' L Rw2 D' R Fw U' Fw U' Fw Rw' U2 F2 Rw2 Fw Uw B2 U' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B R' B2 L F2 U2 L2 B2 U2 F2 U' F' R' B' D' L2 F' L B Fw2 Rw2 D' L' D Fw2 L2 U Uw2 F2 B2 L2 D' Fw L' Fw' D' F Rw' U2 Fw' Uw' F B' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F L2 D' L2 D B2 R2 B2 R2 L2 D2 L2 B' R' F B' D2 L B2 R2 Rw2 F R' B2 D2 Rw2 B D2 Fw2 D2 R' Rw2 Uw' Rw2 U' Uw L B2 L' Fw' U Rw' Uw' Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B L U' F2 L2 B U2 B' U2 D2 R2 F2 D2 R2 L F D' L2 U B' D Uw2 B' Fw2 U2 L' U2 F' Uw2 R' D2 R2 Rw2 Uw B2 Fw2 D R2 Rw B' Fw' L Rw' Fw2 Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L F2 U' F2 U' F2 U2 R2 B2 R2 F2 U' R' F' B2 D' R2 U B L' Rw2 U' B2 Uw2 Rw2 U R2 D2 R F2 U2 Fw' L2 F' D' F2 Fw' Rw F' Fw Rw' Fw Uw' Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B' U2 F' B' R2 B U2 D2 F' U2 F' L' D R2 D2 R' B2 L2 F' B' Fw2 U Rw2 U2 R Fw2 Uw2 R' L2 U2 Fw2 U' Fw' R' L F U B Rw' Uw R L' B' Rw Fw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R U2 R L' B' U2 R F' U D F2 B2 U B2 R2 B2 L2 D' R Fw2 D F' U Fw2 D2 F2 Uw2 F' Rw2 U' B' Rw' U' R2 F' Rw2 B' L' Fw U' D' Uw' Fw' Uw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 B' D R2 D' R2 U' R2 F2 U' D2 B2 D' F' U2 L' D L' D2 Fw2 L' D' Uw2 L' B2 Uw2 Rw2 D F2 Fw2 D R' Fw L' U' Fw R' F' Uw Rw D' F2 L Uw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F D2 B2 U2 B2 R2 D R2 F2 D' B2 R2 U' F' R' D L' B2 L' U Rw2 Fw2 U R' L' D L' Uw2 L2 Rw2 Fw2 R' Fw D2 B U' Fw D Uw' Rw' Fw2 L2 Rw' B U' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 F B U2 D2 B' D2 F' R2 B R' B L F' U L U' R' D Uw2 Fw2 R2 F' R L' F' U2 R Uw2 F R2 U F Uw' F' L B' Fw R2 U Fw' Uw' F2 U' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 B D F B U R F U' B2 R2 D2 R U2 D2 L D2 B2 R' U2 Rw2 Fw2 L' D F2 L' Fw2 D' Fw2 R2 D F2 B Fw Uw2 Fw' U' L' Rw2 Uw R' Fw' L' Uw2 Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 R2 L2 F' U2 F D2 B' D2 R2 F2 U2 R F2 B' U2 F U' D F D' Rw2 U2 F' Rw2 D2 B Uw2 Rw2 U Fw2 U2 R2 F2 Rw B2 L2 U' Fw2 Rw Uw' L Uw2 Fw' Uw' L2 Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 D2 F2 L2 F2 R' F2 U2 B2 D2 B U2 D' F R L B2 U' R' Fw2 L' U2 Uw2 R B' L' F2 Uw2 B' U2 Uw F' B2 Fw2 Uw B' Fw' R L2 Fw' L Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F L2 D2 F2 L U D F' L' F2 R2 L2 U' L2 B2 U' B2 U L2 F2 D' Rw2 F L Uw2 F' Uw2 F' B2 R Rw2 Uw2 L Uw' L' U Uw Fw2 Rw Uw R2 D' F Uw Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B2 R U2 R F2 U2 L B2 U2 L U2 F2 D' R' B U' R' D R' Uw2 R2 Rw2 Fw2 L2 D B2 Rw2 B D' Uw2 L2 Rw D' L2 F2 B2 Fw' Uw' L F Rw' U' R2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F' L B2 L2 F2 B2 D2 L' F2 U2 F2 L U B' U' B L' D' R' U' Uw2 R' Fw2 Rw2 D2 R2 B L2 Fw2 Uw2 F L U' D2 Fw2 Uw Fw2 R Uw Rw2 Fw' R' Fw2 U F ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D' R' F2 U D L F' D F L D2 B2 L' F2 D2 L' U2 L D2 R Fw2 D' F2 Fw2 Uw2 Rw2 D R' L D' R' U2 F' L Fw' U' Fw2 D' Rw Uw' Rw F Fw' U' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 U2 R D2 F2 B2 L2 D2 R D' F L2 D B' D2 B2 R2 U' L Uw2 F2 R' F2 Rw2 U2 Fw2 U' L' B2 Fw2 Uw2 B' Fw D2 L2 U Rw2 Uw Fw Uw2 B' Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L F2 R F2 D2 F B2 R' F' U B2 R2 D' B2 L2 F2 U2 F2 Uw2 Rw2 D R' U Fw2 R2 L' U' D2 L' U2 Fw' L' U2 R' F' Rw B' Uw' Rw D' B2 Fw L' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 D B D F' L B R' U2 L2 D2 L2 F2 D' L2 B2 R2 F2 D R Rw2 Uw2 R U' L2 D2 Rw2 F2 B2 D Uw2 Fw' U R D L U Uw F Uw B' Uw Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F' L' D' R2 D B2 U2 L2 F2 R2 B2 U F B U2 L' F R D R' Rw2 U' D2 B2 Rw2 Uw2 F D B2 Fw2 R2 Rw2 F' R D' F' R2 Rw F' Uw' L' Fw' Uw F2 Fw' Rw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' F U2 L2 B' L2 U2 F2 D2 L2 F U2 L2 D' B' R F D' F2 U' F Uw2 B D2 L' Rw2 Uw2 B L' U2 L2 F R' Uw' R' L Rw2 Uw' R' D Fw' D' F R' Uw' L' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 F2 U2 F U2 R2 B' R2 F' U2 R2 U L2 F B2 D2 F' U2 R D Uw2 L D L' Fw2 R Fw2 U Fw2 L' U' D' Fw' R Fw' U R2 Uw' B' Fw' L Rw D R' F ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 U2 F' U2 D B' L2 B2 R' U2 L2 B2 D L2 D R2 D L2 F2 U2 Rw2 U' L Uw2 Fw2 U R L' F2 L2 U2 Rw2 U' Fw R2 Fw' L Fw' D' Rw Fw' R2 L2 D2 Uw Fw2 D2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D F2 L2 D B2 U' B2 U2 F2 U' L2 U2 B L D2 B2 U' B2 D Rw2 D' Fw2 Rw2 F2 U' F Rw2 B U2 F' L2 F2 Rw D2 L2 D F B2 Fw Rw2 Fw' U' Rw Uw B' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 F2 L' B2 R2 L' D2 R U2 L' D' F' U L2 B' L' D2 L' U2 L2 Fw2 R' F D2 L F' L Fw2 L' Fw2 Uw2 F' R Uw B L' D Fw2 U2 Fw' L2 Fw2 Uw R' Rw2 F ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B' D2 L2 D2 R2 B' U2 F2 B2 R2 F D' R' L2 D L' B D' Fw2 R2 Rw2 U' B Fw2 R2 F U2 Fw2 Rw2 U F2 R L2 Rw' B Rw' B Uw' Fw' U2 Fw Rw' F2 U2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B2 R2 U2 R' U2 L2 D2 F2 R D2 B' U2 R U' F D F L' B D2 Fw2 L' Rw2 B' Fw2 R2 Fw2 L' D2 Fw2 L2 Uw R F R D' Uw' Fw2 L Rw' D2 F Uw F ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 U L' U2 B' L B2 D' B2 D2 L' B2 R D2 L' F2 L F2 U2 R B Uw2 B Rw2 U Rw2 U2 Rw2 F2 R2 L2 F' D2 R' U Rw' B' D' L Uw F Uw2 Rw2 Fw' D2 R ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 F' D R D2 F2 L' B2 R2 U2 R' F2 L2 D' L' F' B R F2 R Fw2 Rw2 B' Uw2 B Uw2 B2 U2 R' F B2 Uw F D2 Fw2 Uw' Rw2 D' Fw U2 Rw F2 Rw U' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 L2 F' R2 U2 F R2 U2 R2 L2 F R' D' R2 F R2 B' D2 L' F2 B' Fw2 U' Rw2 F' U' L2 Rw2 U' B Rw2 D2 Uw2 Rw' U Rw' B2 Rw' Uw' B2 R2 Fw Rw2 Uw' B2 R2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' F' D2 L2 B2 R' D2 L2 B2 U2 B2 L' U2 F L2 B' D R U' B2 Uw2 Rw2 U F' Fw2 D B' Fw2 U' B D R2 Fw2 L' D' Rw' B' D' Uw Fw L2 U2 Uw' L2 Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U F2 L F' B' U' R' F U2 D L2 F' R2 B U2 L2 D2 B2 R2 F' R2 Fw2 U Fw2 U2 Rw2 F D Fw2 L2 Rw2 U2 B2 Rw F2 D' R2 F B2 Uw Rw' U Uw2 L Fw2 Uw2 L2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L D' B R2 U' R F' L2 B' R2 L U2 B2 R' D2 F2 L' D2 L B2 Fw2 R' D2 L' Rw2 Fw2 D2 Fw2 D' R' Fw2 R' Rw2 Fw L U Rw2 Fw' R U2 Uw' F Rw' F R2 U2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 L' D2 L F2 U2 F2 B2 R' L2 D2 F2 U' R L2 F D R F2 R' U' Uw2 B' R F Fw2 Rw2 F' Fw2 L2 U2 B R Uw R D' L B Uw' R' Rw' Fw R Fw2 R D2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L B D' R U2 F2 L' F2 R B2 R B U2 F' R2 D' B' U Uw2 Rw2 F' Fw2 L2 Fw2 U2 B2 R' Rw2 B R Rw2 Uw F2 D' R Uw2 R' Rw' U2 Uw Rw2 B2 Fw' D2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B R2 F D2 R2 F' L2 B' R2 F U2 B' D' L U F L B2 U2 R2 Uw2 Rw2 B' L' Uw2 F2 R2 B2 U2 R B' L' D' Uw' Fw2 D2 Uw' Fw2 L' U' Rw' Uw L2 Fw2 L2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B U2 F D R' B2 U F' L B2 D' R2 F2 U F2 D' R2 U2 L2 Uw2 B Rw2 Uw2 L' D2 B L2 Fw2 Rw2 B D' Uw' B2 Uw' F Fw2 Uw' Fw' L2 D Rw' D2 Fw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 D B2 D B2 R2 U2 B2 L2 B2 U2 F2 L D L2 U R L2 F R' B Rw2 U' B' D' Rw2 U' F' B U Fw2 Rw2 Uw2 Rw' U D Rw' B' Uw R' Uw Rw2 B2 Fw' Rw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' R F2 B2 D2 L' D2 R L U2 B2 L F U2 L' B U R' L2 D2 Fw2 L D2 Uw2 F L' B2 Rw2 B' Uw2 L B' R2 Uw F L' F' B Uw2 Rw F' D2 B U Rw2 Uw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L U2 L2 F2 U2 L' U2 R2 L U2 B2 U2 F B2 U D2 R U2 D F' U' Rw2 U Rw2 Fw2 R2 B2 Fw2 Rw2 D R D2 B2 Fw U' D' F R Uw' Fw2 R Uw Fw' D2 B' Fw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' B U' F' D2 B D' R2 F' B2 L2 F2 L2 D2 L D2 R U2 L' F2 Uw2 B' Rw2 Fw2 U F' R2 F Uw2 B' Fw2 U' Fw2 R' B U' Rw' U F' Rw' Fw R Rw' U2 Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 L2 U B2 U2 F2 R2 D' L2 F B2 L F' U' F' B2 L' F L Uw2 F' Uw2 L F2 Uw2 R U2 F2 L' D2 F' Fw2 Uw R U L Uw F R' Fw' D L2 Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 B2 R2 B2 U2 F2 D F2 L2 D F2 R' B2 D' F' D2 F' U L' D Uw2 Fw2 R Uw2 F2 D' F2 B2 D Rw2 U2 Fw' L' B2 U L' D' R' Rw U2 Fw Rw2 Fw2 D' F2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' F' D B' L B2 L B R2 L2 U L2 U L2 D2 F2 U B2 U2 L' Rw2 F R' F Rw2 Fw2 D2 Uw2 F' R' L2 F Uw' R' F D2 F Uw' L2 Rw' B' R Uw2 Fw' Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' B' L2 B' U2 D2 B D2 F2 B L2 F' D L F2 R B D' B2 L D' Uw2 Fw2 Rw2 D' Fw2 D' F' L2 B D2 L2 B' L' F' B Rw F' U2 Rw Uw' R2 B Rw2 F' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 U2 R' B2 U2 L' B2 D2 R' D2 B2 U' R2 B' R' F2 B' L U' B D' Uw2 B' Fw2 R' Rw2 F' R' U2 Uw2 R2 U2 Fw2 R U' F2 L Uw B Rw' B2 Rw2 B' D Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 F' D' B' R U' D' R2 F B2 R D2 R2 F2 R F2 D2 L2 B2 L D2 Fw2 D2 L D B2 Fw2 D Uw2 L' Rw2 B2 D2 R Fw' L2 U2 R' D Uw Fw U' D2 Fw Uw Fw2 L' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B L' D2 R2 D2 B R2 F2 D2 B U2 D2 B2 D L U2 L B' L2 D2 Uw2 R' L2 Uw2 Fw2 U2 B R Rw2 Uw2 R' F Uw' B2 R2 U' Rw2 Fw Uw2 B2 Uw L' U2 Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U F' U2 L2 U2 L2 F R2 F' L2 F' L2 F' R D' L' B2 U' B D2 F Fw2 L U Fw2 U' F2 R U' Fw2 D' R' Uw2 Fw L' Rw2 F B U2 Uw R' Rw Fw D' Uw R2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F D B2 D F2 D2 L2 U' L2 F2 D' L2 D' R L2 B' R' F2 B L U Rw2 D2 Uw2 F L' Rw2 F Rw2 F2 R' B' L' Uw' F' Rw2 Fw2 R2 Rw' B2 Rw2 Uw' F2 Fw R' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D' B2 L F2 R' F2 L D2 L' D2 B2 L' F D2 L2 B' U R2 F U' L' Uw2 R' D2 Rw2 Fw2 L' D B2 D Fw2 R' Uw2 R Fw' Uw2 R U B Fw' Rw' Fw' L B' U' Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L' B2 U F R2 U' B2 R' B L' U2 R' B2 D2 L' U2 R B2 L2 U2 F2 Rw2 Uw2 F L F2 L Uw2 L' F2 B' L2 Fw2 Uw' L2 Uw2 F2 R2 Rw' Uw Rw U' R2 Uw' F2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' D R2 U2 F2 B2 R' U2 R' F2 L' D2 F2 L' U B' R2 D2 R' F' R' Rw2 F L2 Uw2 Rw2 U' L2 D2 Fw2 U' F2 L2 Rw B' L Uw2 B' Rw U2 Fw Rw Uw B2 Fw' Uw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B U R2 F2 U' F2 U L2 U B2 L2 U2 F2 L F R2 U2 R2 B L' Uw2 Fw2 U' B2 Rw2 U' B' D Uw2 L2 Rw2 U2 Rw' Fw2 L2 U D2 B' R2 Fw L' Uw2 Fw Uw L' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 R2 L2 F2 R2 U' L2 D L2 U2 F B2 R2 L' B U2 L U D' L Rw2 Fw2 U2 L U' D L Uw2 F2 Rw2 B2 Fw' L' D2 Rw2 B2 L' Rw U2 D Uw2 Fw' L2 Fw' R ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L F' D' L' B2 D B U F L2 B' R2 B L2 D2 F2 R2 U2 F' L2 Fw2 L' Fw2 U F2 U2 D' Fw2 U' Rw2 B2 D' Uw2 F' Fw' L Fw2 Uw B' U2 Rw2 Fw' Uw' L2 D' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D L B L2 U2 B' D2 B2 R2 L2 D F L D' R F2 R2 Uw2 R F2 L' Rw2 D' Rw2 B2 R' D R F' Uw2 F2 Fw' D' Rw Fw' R2 L2 Uw' R2 D F ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L U' F2 B2 R2 U L2 F2 D F2 L2 F' B2 D' R2 D R' F2 L' U2 Fw2 Uw2 R' F' Fw2 R Uw2 B R2 L' Fw2 R2 Uw' F L Rw2 D' L Fw Uw' Fw Uw2 Rw2 U B ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 F2 D2 F' R2 F R2 L2 B U2 R2 F R' U' F' U2 D F U' F' B' Uw2 R' Uw2 F2 U' Fw2 R' B2 U' Rw2 U F2 U' Fw L F2 B' Rw F' Rw2 F Fw2 Uw F2 Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' F L' B2 L F2 R' L2 B2 L D2 B2 U2 L' D' R L2 F' U' L2 B' Uw2 R' Fw2 U D' B2 R Uw2 R2 L D Rw2 D' Fw D' F' L' U' Fw' U2 Uw' L2 B2 Rw F' Fw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 R2 D2 R' U2 F2 L D2 L B2 U2 B' L D F' B' U D' F' R2 Rw2 Fw2 D Rw2 D' F2 Uw2 R L U2 D' L' Fw' L2 Uw2 L Rw' F' B' Uw2 Fw' Rw F' Rw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 F2 U2 R' D2 F2 L' U2 D2 F2 R D' L F R' U' D F' D' B' D' Fw2 R2 L F2 Uw2 Fw2 D' Rw2 U R' U' L2 B' Fw2 D F' Fw' D' Rw' D2 L' U Fw' L2 Fw2 Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 F R2 U2 R2 L2 U R2 B2 D' B2 D' L F' B2 U2 D2 L B Rw2 Fw2 L U Rw2 U B2 D2 R' D2 Uw2 F2 Fw' L D R' L Uw L2 Rw' Uw F' Fw' D' B2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F D R' F2 L F2 U R2 F2 B U R2 D R2 U2 B2 D2 F2 U L2 Fw2 D' Fw2 Uw2 L F2 Fw2 R B2 R' Rw2 U D B' Fw R2 L2 U' B Uw L' B Fw' L2 Fw' D ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 U2 B' D2 F' R2 L2 F U2 F2 L' D2 L' B D R' L F2 D2 Rw2 U2 D Rw2 B' U2 D' B' Rw2 F Uw2 Rw' U' L' F Rw' U' F' L' Fw D' Rw2 Uw' R B2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B U F' U2 F' L D' F R U2 B2 U2 D2 R' B2 L2 F2 L D2 F2 Fw2 R' D2 Rw2 U2 D R' Uw2 F2 L F2 L2 B R Fw' Rw2 Fw' Uw R Rw' Fw' Rw2 F' U2 Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' F2 R' F2 U2 F2 D2 R2 D F2 R' D2 R' D B' R F R2 Rw2 F R2 L' F' Rw2 Fw2 L B' R B2 Fw2 Rw2 D' R Uw Rw2 U' D Fw' Uw' R Fw2 L Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 L2 F D2 B' D2 R2 F' U2 F2 U2 L U' F D R' F L2 U D2 L2 Uw2 L' F' R' Uw2 R' Fw2 U2 Fw2 D2 B' Rw2 B2 Uw B D F2 L U' Fw' R Uw' Rw Uw' F2 L ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R D' R L F L2 U2 F B2 R D2 L2 F2 L2 F2 R B2 L D2 B2 Uw2 R' Rw2 Fw2 R2 D L U2 R D' R' B L2 Fw U F2 L2 Rw' U' Uw' L Fw' Uw' R2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' L2 U2 R B2 L D2 R2 B2 R' F2 R' U2 F R2 U' B' R2 U B R Fw2 L U' Rw2 D F2 U2 Rw2 Uw2 B2 D F2 Fw' D' L D2 Uw2 F2 Rw U2 Rw Fw' Uw' L2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 F U' F2 U2 F2 U2 L2 B2 L' D2 R' U2 B' D' B R' B L B' Uw2 B Fw2 L' Rw2 B' R' L' F L2 Fw2 D2 L Uw' L Fw2 U' L' D2 Fw' Rw2 B' Uw' B' Rw' Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U B' R F2 R2 U2 L B' L' D R2 D' R2 F2 B2 U' F2 B2 L2 U L2 Uw2 Fw2 D' L' B2 L Fw2 Rw2 Uw2 F2 U' R' Fw' R' F B' R2 U Rw Fw Rw' B2 Fw' R' F' Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' F R2 U' R2 F2 U' F2 U' B2 U2 L2 D2 B U' B2 L' D B D2 L2 Fw2 D Fw2 D' Uw2 F2 R Rw2 F2 U L F' Fw' L2 U2 B' Rw Fw2 Uw' Rw D' B' R ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L' U B' D2 L D2 F' R' D2 L2 U' R2 F2 U' B2 D' F2 R2 D' L2 B' Fw2 Uw2 R Rw2 D2 L' Fw2 L D' Uw2 R2 Fw L2 Fw2 U' B Fw U Uw R' D' B' Uw2 L2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 F D2 L2 U' R2 L2 U' F2 U' F2 D L' F R2 U2 R F2 B2 U Fw2 U' Fw2 Uw2 F2 U B' Fw2 D2 Uw2 Fw2 Rw2 U' Rw' F2 Fw2 Rw' U' F Rw Fw R D2 Rw' B ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L U2 D' R2 F L2 B2 R2 D2 F' R2 B' L2 F2 R U F' L2 F B' L' Fw2 U' R2 Fw2 U B2 Uw2 B2 R Uw2 R D' R Fw L U2 Fw' Rw2 Uw' L' D Rw' D B Uw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B' D' B U' D' L' B2 R' F L2 D B2 U2 B2 D2 L2 U' R2 D L2 Uw2 R Uw2 Rw2 D R' D' Fw2 L' F2 Fw2 R U2 Fw' U2 R Fw2 U Rw' Uw' F B2 Uw' F2 R' Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F' B D' L F' R2 D L' D2 L2 D2 F2 R F2 R2 U2 R F2 B' D' Fw2 L' Rw2 D2 F Uw2 B' Uw2 Fw2 L2 F L2 Uw' L' B' D2 Rw2 B' R Uw R2 Fw' L' Uw F2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R F2 D' F' R2 U2 L2 B L2 B' D2 B2 U' L' F D L B R2 Fw2 D' Rw2 F Uw2 B2 D' Fw2 L2 F D F2 B Rw U' F' L2 Uw L2 Fw D Uw2 Fw' Rw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F R' F2 U' R2 L2 F2 U' D F2 D R2 U2 R' F2 L' B R' F' B2 U' Uw2 Rw2 U R D' Uw2 R Fw2 D' R2 Fw2 R2 U Fw' U R U2 Fw' L' Rw' Uw' B' R2 U Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 R2 L2 B2 U' L2 U L2 D F2 U2 F' U B2 L' D' R U2 B2 Uw2 R2 Fw2 L2 D Fw2 L U F2 Rw2 D2 F D Uw2 L Fw L' Uw Rw2 F B' U2 Rw' Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' D' L2 U R2 U' F2 U R2 U L2 U' B D L' D2 R L' B Fw2 Rw2 U F Uw2 F2 Fw2 R2 U2 F R2 U' Fw2 R' U Fw2 L' Rw' F2 Fw' Uw' R' D Uw' Fw2 L2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 L' U L B2 L F' D R F L2 U2 F2 D2 R2 B' L2 B2 D2 F' Fw2 L' D2 Uw2 R' Rw2 B' R2 U2 F R' F2 Uw L Fw2 R L' F Rw Uw Rw F2 B' Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B D2 R2 L2 F D2 L2 B D2 L2 F' U' D2 B2 D' F' B U' F2 R' L' Fw2 L' F2 Uw2 R B' R2 Fw2 D2 L Rw2 F' R2 D F Uw R B Rw' U2 Fw' Uw Rw' Fw' R ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U L' F U F2 B' D F B2 U' L2 B2 D' L2 B2 U' B2 U L2 U2 L Rw2 F2 Fw2 D R' Fw2 D Fw2 U' L' D' B2 R Fw' L2 B' D' Fw D L Uw F2 Uw L2 F' Uw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L U2 F2 R2 U2 R U2 D2 F2 L' D2 F2 B U2 R' B D' B' R F2 R' Uw2 B2 Uw2 F U Rw2 D Rw2 B U2 Fw2 Rw U2 B Rw2 D2 R2 D2 Fw' L' Uw2 F' L Rw Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D B2 U R F' L2 B' U' R' U2 L2 F2 D' L2 F2 U' L2 B2 U Uw2 Fw2 U2 D' Fw2 L' U Uw2 L2 Fw2 D2 R2 B Fw' Rw2 Fw' D' F B2 Rw B' Uw' Rw2 U' D ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D' L D2 F2 B2 L' D2 L U2 D2 R' L D' F' B2 D' R2 L U R L Rw2 U Fw2 R L2 D L2 Uw2 B2 Fw2 U' Fw2 Rw2 B' U Fw2 R2 Fw Uw' F' Uw2 R2 Uw Rw B Fw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F' U B2 U L2 D2 R2 D' B2 D' R2 D' F2 B' D2 L' D2 F D2 B D Uw2 Rw2 B2 U B Rw2 F' D B2 D2 B Fw2 D' Rw F U D Rw2 B Uw B2 Fw' Rw2 Fw Uw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 D2 R2 F' L2 D2 F' L2 F2 U2 F2 U2 L' B' D' B2 R F R' L2 U2 Uw2 F Fw2 U' B2 Rw2 F U2 Fw2 D' R2 F2 U Rw F2 R2 B' Uw2 Fw' Rw Fw R D2 Uw2 Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B2 L' U2 F2 B2 L' U2 R U2 F2 L B' D' L F' U2 R F2 R' D Fw2 U Rw2 U R' L Fw2 U2 F2 D' B2 R' L' Fw' Rw2 B2 D2 Uw Fw Rw D R' F2 Rw U ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 F2 L' B2 L F2 U2 L2 U2 R' B2 R D' F U L' B2 R2 L B Rw2 B U' Fw2 U' B2 Rw2 U Fw2 D F' Rw Fw2 L' U' F' Rw U L2 Fw' Rw' D' Fw' U ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 R2 B' L2 D' F2 D R2 B2 L2 F2 L2 D L F' D' R' F' L U' Uw2 Fw2 D' Uw2 F2 Fw2 U F' Fw2 L2 B' L2 U2 Rw B2 D' F R' F Fw' Rw2 Fw' R2 Rw' Uw' R2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' L' B' L2 D2 B2 R2 B' R2 F D2 F2 L2 D' F' B2 U' F' B2 L2 Uw2 Rw2 D2 R U' R2 Rw2 U' Fw2 U2 Rw2 Uw2 L' Fw' L' F' L Fw2 U' Uw Rw' Fw2 Rw Uw R' Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 L' D2 R2 D' R2 B2 R2 F2 U L2 D' B2 R2 F' R' U F D L B Rw2 D Fw2 R' F2 Fw2 U Fw2 U R U R2 D Fw' U' F L2 D Fw U2 Rw Fw' Uw2 B2 U2 Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L U' F2 L2 D L2 U' F2 L2 D' R2 U2 B D R U' F2 L D2 R2 B2 Fw2 L2 D L' Rw2 D' Fw2 L2 Uw2 F2 D' Rw2 F' U' Uw2 Fw D L' Rw U Rw2 Fw Rw U' F ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 U R2 U' R2 D' F2 D2 R2 B2 D' B' L F L U B D' F R F' Fw2 L Fw2 D Uw2 Rw2 U2 R D' F2 U D' Fw R' B Fw' D L Uw' Rw' B' Fw' Rw2 B' R ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B2 L U' R2 L2 B R L2 U' B' L2 F D2 F' D2 F U2 B' R2 D2 F Fw2 L B D2 R' Rw2 F2 Uw2 R' B2 L Uw' Fw2 Uw L' U2 B R2 F2 L Fw Uw' Fw' U' Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U F2 L' D2 L' U2 R2 U2 D2 B2 L B2 U2 B L U' D2 F2 D2 L2 F' Fw2 Uw2 L2 B U2 L Rw2 Uw2 Rw2 B' R L Uw' Rw2 U2 R' F D Fw R2 Rw' Uw' F' B' R' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L D L' F2 L2 B' U F R2 L2 B' U2 B2 R2 B D2 B R2 U R' Uw2 R2 Uw2 R2 F Rw2 D' Fw2 D' Uw2 B L2 Rw F L Rw2 U2 B' D' Uw' Rw2 B Uw' Rw2 U' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B2 L' D2 R F2 B2 D2 F2 R' U2 R' D2 F U' B D' L2 U' R U2 D' Fw2 Rw2 U' L U' F2 Fw2 U Uw2 F2 L2 Rw2 B U R' Rw2 Fw D' Rw F2 B2 U Uw2 Fw Rw' D' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 B L2 U2 B U2 L2 B2 U2 L2 D F R' U2 F' U B2 U2 D' Fw2 R' U' L2 F2 Fw2 D' R2 L' Fw2 D2 L' Fw' R U' D2 R L2 Rw' D2 Rw D Uw F2 Uw F2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 L2 D2 B2 D R2 U2 D' R2 F2 B2 U2 F' U' R L2 U2 D' B U2 L Rw2 D' Uw2 Rw2 U Rw2 F Rw2 B' D' R2 D' Rw' D' B2 D Fw2 R Fw' Uw Fw2 Uw R' B2 L ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' F2 R2 D' F' L2 U D' R B' R2 U2 B2 L2 D2 F2 L2 F' D2 R2 F2 Uw2 B D' Rw2 D' Rw2 B2 D' L2 U2 Rw2 F' B Rw' B' L F2 Rw' D' R2 F2 Uw Fw' D Uw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L' F' U2 D2 R2 F2 B' U2 F' L2 B2 L2 U L' B D' F' U2 L' U Rw2 F R2 B Uw2 B R' Uw2 L2 B2 L' F' Uw' B L' Rw2 F U' Rw U2 R2 Uw' B Rw F2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' D2 B2 L D2 B2 L' U2 R2 B2 R2 F' R2 U L' D' B U2 B' R Rw2 U B R2 F Fw2 U' D Fw2 U F2 D Rw U F2 D Rw' Uw R' B2 Fw' R U2 Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 B U L D2 F L' F R2 L U2 D2 B2 L D2 L2 U2 F2 U Fw2 R' D2 Fw2 U' R Uw2 Fw2 D' F2 D' F2 L2 Fw' L B2 U Uw2 B Rw Fw' D2 R' Fw L' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L B' L2 F2 B R2 B2 R2 U2 B2 U2 F' L F L2 B U D' F' R' Uw2 Fw2 L B2 U L' Fw2 Rw2 D' R2 L2 Fw L B' R2 Rw2 B Fw' L2 Rw D2 Rw' B' Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U F2 U' R D' R2 B U2 L' B2 R B2 R D2 L2 B2 D2 F B2 U' Rw2 F' Fw2 D2 Fw2 R' B Rw2 Uw2 R2 B' R' L Uw' F2 Uw' L F2 Uw Rw Uw F2 U' Rw D2 Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' D B2 L B2 R U2 B2 R F2 D2 R' F2 L D' F U F' L B R' Uw2 Fw2 L F2 D Rw2 U2 D' R' L' U' F Fw L U B L Uw' Fw' L' Rw' Fw R B D2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' U2 D2 L' F2 L F2 R B2 R2 U2 D2 F U' R' U' B' U L U F Fw2 D Uw2 L' U2 Uw2 B2 D2 L Uw2 L2 Fw D' B' Uw2 F D L Uw' Fw R2 U' L2 Fw' D' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 U2 R' F2 L2 U2 F2 D2 R B L' F' L2 U' B D L B Uw2 L F' Uw2 F Fw2 R F2 Rw2 F R Fw2 L2 Uw' F R L D Fw2 Rw' Fw Rw F' Rw' F' Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' F D2 F L' U D2 B R' U2 F' D2 F' D2 F' L2 U2 B2 L2 B2 Fw2 Uw2 B' L2 U Rw2 F Uw2 R2 B2 U2 F B Rw U Uw2 R' Rw2 Fw' Rw F Fw' L Rw D2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 L D B' U2 R2 D' L U2 L2 D F2 U2 D L2 U2 L2 B2 L2 D' Uw2 F Rw2 Uw2 B2 R' Fw2 L D2 L2 B' Uw B R D' L U' Uw Fw R' Uw2 Rw2 D' L ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F U2 F R2 B' D2 F2 D2 B' L2 B U B2 L' F2 R' U2 B R2 L U' Rw2 D L2 U' Rw2 Uw2 B D' Rw2 U2 F D Rw Fw2 L D' R' Rw Uw' Rw U2 D F' Uw' Rw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D' B' D F2 D L2 D B2 D R2 U2 L2 U' F L' D' F' U' R2 F2 Rw2 Uw2 L U' Uw2 L D' Rw2 U2 R L B2 Fw U' Rw2 F U' D2 Rw U Uw2 Fw' U' Uw' Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' R B2 U L2 B2 U F2 D2 R2 D' B2 R L2 U' D' F2 B R' D' Uw2 R' F2 Fw2 Rw2 Uw2 F L U2 B' D2 L Uw' R' L B L' Rw F2 Uw F2 B Rw' Fw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 F2 B2 D F2 U L2 U2 R2 U R2 U' R' D2 F' L2 F' B R' D Fw2 R' F2 L2 D Uw2 R Fw2 U2 D B L Uw2 Fw L2 D2 Rw' Fw2 Rw' Fw2 L' Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' B' L F2 U2 R2 B2 U2 L F2 R' B2 D2 B U2 D B D' R F2 Fw2 L2 Fw2 R B' Rw2 B' Uw2 L' F2 R' B Fw2 U' B Uw' B' U Fw' L U' R' Rw2 Uw' R2 Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 U' L2 B U2 B R F L F2 U' D2 L2 F2 L2 B2 D2 L2 Rw2 U2 F R Uw2 B' Uw2 Fw2 L B2 R2 L' Uw' R2 B' R' Uw2 B2 Rw F Rw U Rw Uw' Rw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' F U2 L2 F2 U' B2 U2 F2 D' R2 L2 D2 F2 B' L2 B' L U' F Uw2 L2 B2 Fw2 D' F B Uw2 L2 B' U D Rw' F' L' F2 U2 Fw' Rw2 D' B2 R' Fw D2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 B U2 R2 F' B' D2 F R2 D2 R' U L2 B R' U2 D' B2 R F2 Uw2 F2 L' Rw2 D2 B' R' U2 L' B2 Uw2 B Fw2 U' Uw L U B2 Uw' B2 Fw R' F D Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L' U2 D2 R U2 R' U2 R' D2 R2 U F' D' R' U D B' D2 B2 L' Rw2 U' L' Uw2 F2 Uw2 Fw2 D2 Rw2 U' F2 L' Fw R Uw2 L U F Rw' Uw' L' F' D R' Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L' U' L2 F2 U' L2 U F2 U2 B2 U' R2 F2 B' L2 F R U F L' Uw2 R' Uw2 R2 F Rw2 D2 B Uw2 Fw2 Uw2 F Uw' L' D2 F2 R' Rw B' D B' Uw Fw' U2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 L2 F2 L2 B2 D2 B2 U R2 U2 F D R D F2 R2 U R B' U2 Rw2 U' D2 Uw2 L' Uw2 L' D2 L' Rw2 U' R2 L2 Fw' Rw2 B2 R D' B Fw Rw F Fw L' Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 D2 F2 L2 B' L2 B2 U2 L2 B2 L F2 U D F' L B U2 D' B' Fw2 Rw2 U2 B D' Fw2 D2 L2 U2 Rw2 F U2 Rw B' U2 B' L2 Fw2 L Uw' Rw U' Uw' Fw2 R ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B' L2 D' L2 U' F2 U2 D' L2 U' L2 F2 R D2 R F L B2 L2 B2 Fw2 U F2 L2 Rw2 D R D Fw2 U' D' Fw' U Rw2 U' R U' Rw U2 Uw' Rw2 B' R' F ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' R B2 L U' B' D R' F' R' U' R2 D L2 D R2 D' B2 U2 B2 L2 Rw2 U' Uw2 B2 R2 D' Fw2 L U B2 U2 R' F' Rw2 Fw U2 F' L2 Uw' F' Rw' F R' Fw2 D ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B' L' D2 R D2 L F2 D2 B2 U2 R B2 L B' D R2 F U2 B' D' R2 Fw2 Rw2 F2 B' R Uw2 Rw2 U2 L2 F B' Uw F2 L2 D' F D' Fw' Uw L' Fw' D Uw Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B2 U2 L2 F2 U2 L2 U2 B2 L' D2 R F' L' B2 D R2 F2 U' D' R Uw2 Rw2 U' Uw2 L2 Fw2 U' L Rw2 U2 R B2 Uw2 Fw' D Rw2 U' Rw' F' L Uw2 B2 Uw B2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' R2 U2 R' B2 D2 F2 U2 D2 R2 L' B U L F R U' R2 L Uw2 R2 F U2 Fw2 L B2 L2 F Fw2 L' Rw2 F D B D' Uw' L Rw Uw R' U2 Fw' D R B2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L' B R2 U2 R F' B D2 R2 L D2 B2 U R2 F2 R2 D2 F2 L2 B2 Uw2 R D2 R' Fw2 L2 F' Fw2 U2 Uw2 R Uw' B U2 Uw2 R2 B2 R2 Fw Uw B' U2 Rw Fw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U F R' D F L2 D' B R' U2 D2 R' U2 L B2 D2 F2 B2 L' F' Rw2 F R' L Uw2 B' D2 Fw2 R' Fw2 R Uw R2 U2 B' R' F2 R' Uw' Rw' Fw2 U' B' Rw' Uw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 F' U' F2 L' U B2 R2 L D2 L' B2 R' D2 L' B2 U2 F Uw2 Fw2 Rw2 U' Uw2 L D F2 B2 Fw2 L Fw' R' L2 D Rw2 Fw2 Rw D2 Fw' R' U' D' Uw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 U2 L' F2 R F2 R2 B2 R' U2 R' L' U R' B R' L2 U B2 U F Uw2 L Rw2 D2 R' U' L2 Fw2 U F Fw' L' D' B' Rw2 B2 D Fw2 Uw B2 Uw Rw2 Uw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B2 L2 F2 R' B2 R U2 D2 R F' B2 D' F' U2 F' D' L' B' L2 Fw2 L' Rw2 U R' U2 R' Uw2 Rw2 Fw2 L2 Fw2 R F' Fw L D Rw2 Uw' R' F R' Uw2 Fw Uw2 Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' D F U2 R2 U B2 D2 B2 D' F2 B2 L2 B' D' B2 R' D' B R2 Rw2 U D L' D' Rw2 Fw2 U' R' D Uw2 B' R' Fw' D' F Rw' Fw U B' R2 F2 Uw' F' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 R' D' B2 D2 B2 D2 L2 F' L2 F R2 U2 R' B2 U2 B R' F B' Rw2 B' Fw2 R' D2 B2 L B2 R2 Fw2 R' F2 U' Uw B' D R2 D2 Rw Uw' Fw' D2 B2 Rw B2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 L F2 R2 F2 B2 D L2 U D2 R2 U' L F U D B D L' Rw2 D R2 L Fw2 R D2 L Rw2 Fw' D' F' R F L2 Rw2 Fw Uw B2 Fw2 Uw' Fw2 Uw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R B2 U2 B' R2 D2 R2 L' F2 R2 D' R2 U F2 D2 R2 L2 B2 U' R' Fw2 Uw2 L Uw2 Rw2 F2 U R Rw2 D B2 Fw2 U F' Uw2 Fw L' Uw2 L' B Rw Uw' Rw U F2 Fw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B' D B L U F' R' F2 D2 L2 B2 R2 F L2 U2 F R2 B2 R' L2 F' Rw2 Fw2 D' Rw2 Uw2 L2 F' D' Rw2 F2 Rw2 Fw2 D Rw' F B2 Rw F2 D B' Uw R2 Rw D B' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B R2 F D2 L2 D2 R' U B' D2 L2 D2 R' U2 F2 L D2 R' B2 D2 Fw2 D R2 U' Uw2 R B2 Rw2 D2 F2 Fw R' B Fw2 D' F Uw2 Fw' Uw B2 Uw2 L2 Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L' U2 R U' R2 B U2 F' R' U2 L2 D2 B' L2 B2 L2 F D2 Rw2 Uw2 B' R B2 R' Uw2 R Uw2 L Uw2 B D2 Uw F' L Rw2 Uw' F' Fw U L' Uw B' D' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B' D' R2 U2 L2 F2 U2 F2 U' R2 B2 R2 B R' U2 R' U L F R' U' Rw2 Uw2 R' Uw2 L' U Uw2 F2 U2 D R' Fw2 L2 F' U D' Fw' R' Fw Rw' Fw' Rw2 D' Uw2 F ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 L F2 L D2 B2 D2 R2 L B2 L D' F R D' R' B2 U L2 Fw2 Uw2 B' D B' Fw2 Rw2 Uw2 B2 Fw2 R2 U2 Rw' D2 B D' Uw' R' Fw2 Rw' D2 L' Fw' U2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 U' L2 F2 D2 R B2 D2 R2 D2 R B' U R' L D L' D2 B2 Rw2 B' Rw2 D2 Uw2 F2 R' B' Fw2 U2 L' F2 U Uw2 B2 Uw R U Uw2 Rw Fw R' Rw' F' D L' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 B R2 F B2 L2 B U2 R2 L2 U' B R' B D R2 B2 L B U' Fw2 L Uw2 B R' D2 F Uw2 B R U2 L Rw2 Uw Rw2 D2 R' F' Uw' Rw B2 L' Rw2 U Fw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 L2 F2 U2 B U2 B D2 R2 F2 U D2 F2 R2 L D B' U L' U2 R Fw2 Rw2 U' R2 D2 Rw2 F' R2 B2 Rw2 Fw2 U' R' Rw' U' R Rw F Fw' U R' Uw' L2 Rw' B ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F D L U' F U' L B2 D F2 R2 F2 L2 D F2 U R2 D2 F2 L' Fw2 R B' Uw2 Rw2 D2 L B' Rw2 U2 B R2 F2 U Uw F' R' U Fw U2 Rw B2 D2 R' Rw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F' D' R2 B D' L' F2 U R2 L2 U L2 D B2 D F2 U2 B' R' Rw2 U Rw2 F2 D R Uw2 L Fw2 L D2 Fw D2 Uw2 Rw2 F2 Uw R Rw Uw2 L B U' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 D2 R2 U B' L D' F B2 U2 R' U2 R U2 D2 R' B2 R' U2 R' D' Rw2 U D' Rw2 D L' U Rw2 U Uw2 R Fw' L2 F L F2 Uw' Fw' L Uw' Rw' B R' U2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L' B L' U D R' U2 L' D F2 L2 F' R2 F2 L2 D2 F' L2 U2 F L2 Uw2 R Rw2 U F2 Uw2 R2 L2 U' R U' L' Fw' Rw2 Fw' R' L2 Fw' Uw Fw Uw' Fw L2 D2 R' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D' R B' U2 F2 L B D B2 U' R2 U' F2 U2 D L2 U2 L2 B' Uw2 F Fw2 Uw2 R Uw2 L' F' B Uw2 Fw2 R2 F2 Uw L2 Uw' F U2 Uw' Rw' B' Fw U2 D2 Uw Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 D B2 D2 B2 U' L2 D F2 D R' U R F' U' D2 B2 R L U' Rw2 B2 D L Fw2 R' U' D2 Rw2 B2 U Fw R2 F' B' Fw U' B Rw' Uw F' Rw2 Fw' Rw2 B' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 U' F2 L F2 D2 R F2 L2 U2 L' B2 R2 F2 B' D' F' R' U' L2 F Uw2 L D' Rw2 U L F2 L Fw2 Uw2 B2 L' U F2 B Fw' L' D' Uw' L B' R' Rw Fw' L' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 D2 R' U F2 B R U2 R2 L2 B2 R2 F R2 B D2 R2 U2 L U' Rw2 F2 Uw2 L' Fw2 R' F' Uw2 L2 D2 B' D B' R2 D2 Uw L' Rw' Fw' Uw2 L2 Rw' U' Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' B2 L2 U' B2 D' R2 L2 U' R2 D2 F2 L' F L B U2 B2 Uw2 B' L2 Fw2 U2 D' B' U2 Rw2 U2 F' B' Rw' B' Rw' B Rw B' Fw' Rw' U' F2 R' B L2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U D2 R D F U B' R' D' B D2 L2 B' U2 R2 D2 R2 U2 B2 R2 B2 Uw2 B Uw2 F' R' U2 Fw2 U2 L' Rw2 B2 Uw' L2 Fw2 R' U2 B2 Uw Rw' D Uw' Fw2 L2 Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B U B2 L' U2 R F2 R2 D2 F2 L' U B2 U R' U F R' Fw2 R' L' Uw2 F' Rw2 B2 L F' Rw2 F R2 Uw' B' D F' Uw' F' B2 R' Fw' L' Fw Rw' B2 Rw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' U R2 F2 L B2 U2 L2 U2 L U2 R' D2 B R U2 B2 L' U R Fw2 Rw2 D' R D L B2 Fw2 R2 U' Fw2 U2 L2 F' D' Fw U R Uw Fw' U' R' D2 Uw' Fw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 U' B R' F U' D F' L B D2 R2 U2 B' L2 F' D2 B' L2 B2 L2 Uw2 F' L Rw2 F R2 B L2 Uw2 R' F B2 Uw F2 Uw R Fw2 R2 Rw' Uw' B Uw2 F2 R' U' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B D F B U' R F U2 L' U2 F2 D2 F2 R B2 L' D2 L B D' Rw2 F L2 Rw2 B Rw2 D' R2 Uw2 R2 F R2 Rw' Uw2 R' L Rw' B Uw Rw F R2 Rw' B2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 R2 F L2 B' U2 F2 U' R' D2 L2 F R2 B D2 F U2 B' Fw2 L' D2 L' F2 Fw2 R U Fw2 U' B2 Rw2 Uw2 Fw' U B' D2 L' Rw' Fw2 Rw' F2 Uw' L2 Rw' D ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L U' D2 B U2 B D2 F' B2 D2 B2 L2 D' R B D2 F' U' R U Fw2 Uw2 F R2 D' Rw2 B' L2 B2 L2 U' Rw2 Uw2 Rw' B' U2 Rw2 F Uw' Fw Rw F L2 D Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 B2 R' D2 L2 D2 B2 R' B2 D2 B2 D' F' D2 R D B2 R2 B' U Rw2 F2 Fw2 D' B2 L' Fw2 Rw2 D Uw2 R U R Fw' U B R2 B' U Rw' Uw B' Fw2 Rw' Fw2 Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 R' D2 R B2 D2 B2 L U2 B2 R D B L' F' L B R' U B' R Fw2 L Rw2 U2 D' B2 Rw2 U' Rw2 F2 D' L' Fw R B' D' F Rw' Uw R' Uw2 Rw' F' Rw D ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 U F' L2 F' R2 L2 U2 B U2 B2 R2 L' U B D L2 B R B Rw2 U' Uw2 F2 Uw2 F Fw2 L2 U2 Rw2 F' U Rw' Uw2 F B R B' Uw B Rw Fw D2 B2 Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' B' D2 B L2 B2 D2 B' L2 D2 R2 F2 R U' L2 D' L' F' B R' U' Rw2 U2 B' L Uw2 L D2 F' U2 R' Fw2 L2 D F2 Uw F' Rw' Fw2 R' Rw Fw' D2 Uw' L ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 D2 L' B2 L D2 B2 R' D2 B2 L' D B U2 D' L2 F2 L U D2 Rw2 Uw2 L F' L2 B' L2 B2 Uw2 Fw2 Rw2 B D2 Uw Rw2 Uw' B2 D2 Fw2 R D2 Fw' R2 Uw2 Fw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B L2 B2 U' B2 D R2 B2 R2 U2 R2 F L2 D2 L' U L B' L2 Uw2 Rw2 D Fw2 D' L2 D' R' Fw2 R2 U2 Fw R2 L2 U L Fw' U B Rw Uw' R2 Fw Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' B U' R L2 B R' U' B U' R2 F2 D' B2 D' R2 F2 D2 R2 B2 D Uw2 L F D2 Uw2 F' U2 B2 R' F' Fw2 Rw2 B' U L F2 Uw B2 Fw' Uw Fw' Uw2 R Fw' U2 Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U R' L2 D2 F L2 F D2 B L2 F2 D2 F' R2 D R D B2 R L B Uw2 Rw2 U F B2 Uw2 Fw2 U' D' B Uw2 B Fw2 Rw F' Uw2 R2 L Uw' Fw U Rw D' L Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 D' R2 L2 U L2 U R2 D2 B2 U B' L' U L D F U2 L' F' B2 Rw2 Uw2 L F2 Fw2 R D' Fw2 Rw2 B2 L Fw2 R Fw' D2 R' Rw2 U2 Uw2 Rw Uw' F2 Fw' Rw Fw' R ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 F D2 R2 B' D2 L2 F2 U2 B2 U F' R D' B U' D F' R' Rw2 U' L' Uw2 Fw2 L2 Rw2 B2 L' U' D R Fw' L' Fw' L2 F Fw' U R Rw Uw' L B2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L U' F2 B2 L2 U F2 U2 D' B2 R2 U L2 F D2 B U' D2 B' R' F Rw2 U Uw2 L Uw2 F2 B2 Fw2 L U F2 D2 L' Fw' D B' Uw2 F' Rw' U Fw Rw' Fw2 Rw Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 U B' L2 D2 L U2 L2 U2 B2 L F2 B2 U D2 F U2 L' U' L2 U' Uw2 L F D2 B2 Uw2 F U2 Rw2 B' D2 Uw2 L Uw Rw2 Uw' R2 B2 U' Fw' Uw' Rw' D' Uw2 Fw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L F2 U2 D L2 D F2 U2 F2 U' B2 U' R' F2 B L F L' F' R' Fw2 R' D Fw2 L' F2 Uw2 R Fw2 R B2 U' D B' Fw D L' D' Rw' F Uw2 Fw2 Rw' B' Uw R ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U R2 L2 D' F2 B2 D2 F2 R2 D' B2 U L' F L' B2 R2 D2 B' R Fw2 U' L' U' D' F2 Rw2 D' R B2 Fw2 U' L2 Fw R2 D R' Uw2 Rw2 B2 Fw Uw F2 L Fw D' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B' U F U' R B' R' D2 L U2 R2 B2 L2 B R2 B D2 F' L2 F' R2 Rw2 F' Rw2 U D' R2 Rw2 F D R2 U Rw2 Fw2 L Rw D Uw2 B2 Rw' Fw U' L Rw Fw' D2 B' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L F2 R U2 L2 F2 L' F2 U2 D2 R2 D' F D B R U' R2 B' U Uw2 Fw2 U' R Fw2 U D2 L2 D2 R2 D' Fw' L2 D' Fw' R2 Rw2 U' Fw' Uw' L Rw' B' R2 Uw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 R2 D R2 U2 B2 R2 L2 D B2 D' B' U2 L' D' L' F' B' R2 B Fw2 Uw2 F D2 Uw2 B2 U' F' B' U Rw' D R2 D' F B2 Uw Fw' Rw Fw L2 F Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 U2 F2 D2 B' R2 B D2 F R2 F2 U' R L' D2 R F' R2 U L2 F' Rw2 F Fw2 L2 Uw2 B U2 D Fw2 Uw2 R2 D2 B R Rw F2 D' F2 Fw Uw R2 F2 Uw Fw' Uw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 D2 F B2 U L B U' L' F2 U2 F2 B2 L' B2 L D2 R U2 Rw2 D' Uw2 L' D2 Fw2 R' B2 D' F2 R' L' D' Fw' U D' F D' Fw Rw' F R D Uw Fw R ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U F2 U B2 U' L2 D' B2 U2 L2 B2 L B' U' F2 R2 D2 B' U2 D Fw2 D' B Uw2 R2 D' Rw2 B D L2 U' Fw2 U Rw B2 U' D2 Rw Fw' Uw2 L' U D2 Fw Uw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' D2 R2 F' D2 B' R2 F B U2 L2 D' R' F2 D' B' U' F' U2 D' Fw2 R Fw2 L F' B' Uw2 L D2 L2 Uw2 L2 Fw2 Uw' F' U R' Fw Rw2 Fw U' R' Rw' U2 D' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' B2 U' L2 U' B2 D' L2 U2 B2 D R' D' R B R' U R2 F' B L' Uw2 Fw2 Rw2 U Uw2 L' D2 L' D Uw2 Rw2 U' F2 Fw' R2 U F Uw' Fw2 Rw U' D2 Uw L' Rw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D R' B2 D2 F' U2 F' D2 R2 F' U2 R2 F2 L2 D' R B' U' F U' L' Uw2 Rw2 B U Rw2 D' R2 L2 Fw2 D B' U' Rw' F' L2 U Fw2 R2 D2 Uw L2 Rw Fw2 U Fw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D B D' F2 D R2 B2 R2 F2 D' B2 D2 R2 U' L' D' F' R L U Rw2 U B2 Rw2 B2 Uw2 Rw2 U2 F2 R' U' Rw2 F' B' Fw L D Fw' Rw' F' R Uw Fw L2 U' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F' B2 D R2 U R2 U' R2 U2 R2 F2 L F' D F R2 F' U2 R B Rw2 F' U D L2 D' Rw2 U' D2 F Fw2 L' B2 Rw' D R Fw2 R2 Uw Fw' U' R Fw2 Uw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 D' B2 R2 U2 L2 D R2 F2 B2 D' F L2 U B2 R2 D2 R' F' R2 L' Rw2 U' F' Rw2 D2 F' Uw2 R2 D B' L2 B2 D' L' Rw B L Uw2 R F Fw D R2 Rw Uw' L ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 D2 F2 U2 B2 L' B2 L' U2 R2 D' R' F2 U D2 R2 B R' U2 D Uw2 L B2 Uw2 F' Uw2 B L2 Uw2 L B L2 D2 Uw Fw2 L' Uw' F' Rw' Fw2 Uw' Fw R2 D Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 L' U2 L' U2 F2 L2 D2 B2 L2 D2 R D' R D2 R' U' B R L2 B Rw2 B L' Uw2 Rw2 B L' D2 B2 R2 L' B Rw2 Uw' L D F2 Fw L2 Rw' D Fw2 R Uw2 L' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D' L D2 L' U D2 R' F U' B' D2 B' L2 D2 B L2 B' U2 B D2 L2 Uw2 B' L2 Fw2 D L2 U2 L2 Rw2 F' U2 B' R' Rw' F2 U L' D Fw' Uw' Rw2 Uw R2 Fw D' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 L F2 D2 B2 U2 L' F2 R2 D' B U D R' B D' L B2 R2 Rw2 U R2 L Fw2 L2 Rw2 U Fw2 D2 Uw2 R' Fw D B D Uw2 Rw U F' B2 Uw' Fw' Uw2 Rw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' B' L2 F2 L B D L D2 R2 L B2 L D2 R D2 L2 D' B Uw2 R Rw2 D B2 L2 U2 L' Fw2 L' Uw2 Fw U B2 U2 Fw' Uw' B2 Fw' Uw F2 Uw2 B2 U' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B2 U2 D B' L' F' D2 R U' B' U B2 U2 L2 D L2 B2 D2 R2 L2 B2 Uw2 F Fw2 U Fw2 R2 D Rw2 B Fw2 U' D Uw2 Rw' U F D2 L' B2 R' Fw Rw' U' D2 Rw' D' Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B' R' F B L2 D2 F2 L2 U2 F' R2 B D2 L2 D R' F B' U L F' Fw2 Uw2 L' D' B2 R' Uw2 R' B2 L' Fw2 R B' Fw2 Uw2 Fw' L2 F Rw B R2 Fw Rw2 D' Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R2 L2 B' U2 F2 B R2 B' D2 L2 U' L F' R2 B2 U' D R F2 B2 Rw2 U B Rw2 Fw2 L2 U' B2 Uw2 F2 Fw2 D' L Rw' D Rw2 B2 Fw Rw' F2 L Rw U2 B ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D' R2 B2 D2 L' B2 L' D2 F2 D2 F2 U2 R D R' L2 D B' R' F2 U Rw2 F' Uw2 F' L' B Fw2 R2 L D2 Fw2 L F2 Uw' F2 Uw F B' U2 B' Rw' Fw' Uw B' Rw Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L U2 L U2 L' U2 B2 D2 F2 U2 L F R' D' R' L' B U2 R F D Uw2 F R L2 Fw2 Rw2 B U2 Uw2 L D2 R F2 Uw F' R' D B' R Fw' Rw F' Fw' Uw2 Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 B' R2 D2 R' D L B' U' B2 U2 F L2 F R2 B' U2 F2 R2 Uw2 B R' Rw2 U2 F' B Fw2 R' L2 F Rw2 D' Uw2 R2 Uw Rw2 Uw L' Fw Rw Uw' F2 Rw U2 D2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F B U R2 F' R2 D F2 R' F2 R F2 L' D2 L B2 L' U2 L2 B' Rw2 D' B' Uw2 B2 U2 L2 F Fw2 D B2 Uw2 L Uw2 Rw U2 Uw2 Rw' B' R Uw Rw' Uw2 L' F' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 R D2 B2 D B2 L2 U' L2 D' L2 B2 D2 L' B2 U' B L2 F' R2 Fw2 R Fw2 Rw2 Uw2 F2 D' L2 Fw2 L Fw2 D2 B' U B Fw U' L' Uw' F Rw2 B' Fw' Uw' B2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 B2 U2 L D2 L U2 R U2 R2 D2 F' U' F2 D' L' B' U L B2 Rw2 F' B Rw2 U Fw2 D' B' L2 Rw2 U' Uw2 F' Rw B2 D Rw B Fw Rw' U Fw' D2 F2 R' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 D2 R U' F B2 R' L2 F2 R2 U2 B D2 B2 U2 L2 F' R F' Uw2 F U2 Rw2 D2 F2 Uw2 Fw2 U2 R' F' B2 Uw L' F2 Uw' F' R2 Fw L2 Fw2 Rw2 Uw' Rw D ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 R2 L U R2 D' R2 L2 U' B2 R2 D' R2 D F R F2 D F2 B2 L Uw2 Fw2 L U' Uw2 Fw2 R' F2 D2 Uw2 Rw2 Uw2 B D2 R Fw Rw' Uw' R' Fw Rw2 F2 Rw' Uw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 D' L2 U B2 R2 D2 R2 U D2 F B2 R' D B2 R' U' B' R2 F' Fw2 Uw2 R U' L2 F2 Fw2 Rw2 D L2 D' Rw2 D Fw D' B' R' F' Fw2 Rw Uw F Fw2 D R U ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R F L U D' F' U' R2 D2 B2 R2 L2 U2 B' D2 F L2 D2 L' Rw2 U Fw2 U2 L' U F2 D' L U' Fw2 U' B' D' Fw' Uw2 L U' Fw' R2 Uw Rw Uw' R Rw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U F R' U2 L2 F2 R B2 R2 U2 R' L2 U2 F2 B R2 F' U F D' F Uw2 B Fw2 L' F' R' L' B Fw2 Rw2 F2 L' F' Uw' B2 Uw' F' L F2 Fw R2 Rw' Fw' R2 Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L F2 D2 B2 D R2 U2 B' U' D2 B2 L2 U2 R' D2 R' D2 L2 Fw2 U2 Uw2 R' Rw2 F2 D Fw2 D B2 U2 L2 F' D Fw L' Fw D2 Uw' R2 B Fw' R2 Uw Fw2 U ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D L2 D B2 R2 D' B2 R2 B2 D2 F' D R' D2 R' D F B2 U' Uw2 L2 Fw2 D R2 B' Rw2 D Rw2 U' R2 F U L' U' Rw F2 B' Uw' Fw D2 Rw2 Fw2 U2 R ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 U' F2 B2 R2 F2 U L2 U F2 U' B2 R B R D' F' D' B2 U2 L' Fw2 U2 L Rw2 F2 D R2 Rw2 U' L Uw2 L' Fw U D' F2 U' R Rw Fw R' B' Rw D' L' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 L B' R' L B2 R D' B2 D2 R2 F2 B' U2 L2 F D2 L2 U2 B2 Rw2 D2 F' B' L' B' Fw2 R D2 L' Fw2 Rw2 Uw' Rw2 U2 F2 R B2 Uw Rw' B U2 Fw' U Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D R2 D' F U B' U2 F' B2 U2 D2 L D2 R U2 B2 U2 L U Fw2 Rw2 F' D' Uw2 R2 D Fw2 Uw2 F' B' D L Rw' Fw2 D B U2 D Uw R' Fw Rw Fw2 U' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 B' U2 D' R2 L B L2 U' R2 B' U2 F' L2 U2 D2 F L2 U2 F Rw2 F' R2 Rw2 F R' Uw2 F R' Fw2 Uw2 R2 U Fw2 Uw B2 D L' Uw' Fw U2 Rw' Fw' D B' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 F2 U2 B2 U F2 R D R2 F2 B2 D' F2 D2 B2 R2 F2 R2 B' U' Rw2 D' Fw2 U2 L' Uw2 Fw2 D' R' L' U' Uw2 R' Fw' R L' U B2 L2 Uw' F Rw' U B2 Fw Uw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' R' U2 F' D' L F U2 D R2 L2 U R2 L2 B2 U' R2 L' D2 B Rw2 D' F2 Rw2 Fw2 Rw2 F' Rw2 D' B2 U2 D2 R Rw D F2 Rw' F2 Fw' Uw' L2 U B2 Uw' R ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D F2 R' L2 U2 F2 B2 R2 L' F2 U2 F2 U R' D' R2 D2 F B2 Rw2 U R' U D Fw2 R Rw2 U Fw2 D L U' F' U2 Rw2 Fw L2 D' Uw' F2 Uw Rw Fw' Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 L D2 R' U2 L' B2 U2 L' U2 B2 L' B U L2 F' L' F2 B L' F2 Rw2 Fw2 R2 Fw2 D F' Uw2 R2 F' U' F2 B R' Rw U2 Fw2 Uw2 L' Fw' R' Rw' U' Uw' Rw' Uw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U2 L2 D' R2 U2 R2 B2 R2 F' R2 F2 D2 R2 B L' F L U L' F2 R' Uw2 B L' Rw2 B2 Fw2 D2 L' Rw2 B R' U2 F' Uw F U2 R' L' Rw2 B' Rw' B' Fw D Rw Uw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B L2 F' L B2 U2 D2 L' D' F2 R F2 L2 D2 F2 L' U2 F2 B2 D2 Fw2 L' D R2 D2 Uw2 L U2 Uw2 R' Rw2 Fw2 L Fw' U R Fw2 L' Uw' R2 L2 U2 Fw2 Rw' U' Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F D2 R2 D R2 U' F2 U' B2 L2 B2 D2 L' D' B2 D' F' D2 R U' F' Fw2 R' D' Fw2 D2 R2 Fw2 U F2 D' Uw2 L' Fw' Uw2 F' L' U' Fw D' Rw Uw F2 B2 U2 F' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 B D2 B U2 F U2 F' L2 B D L F' D R2 D' R' U' F2 B Rw2 F' D F' Uw2 F B2 Rw2 D F2 Fw2 Uw2 L F B' Rw Uw Rw Fw R Rw D2 Uw2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D' F' R2 U2 D2 B' U2 D2 F' U2 L2 B R' F' B2 R' L' D' F U' Fw2 R2 L' D Fw2 R D2 Rw2 D B2 R Rw2 F R2 Rw2 Fw L2 Uw' B' Uw R2 Rw F Fw' Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 U2 D F2 R2 U' L2 B2 R2 B2 U L2 F' L B L U' F D L D Rw2 F D' Fw2 Rw2 U' F2 U' Uw2 F2 L2 Uw2 Rw B R' Rw2 B L2 Uw' Rw Fw Uw' R ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 L2 U B2 U L2 F2 U2 F2 B2 R2 L U' F B2 R2 L' F2 U' R2 B' Rw2 Fw2 R2 Fw2 Uw2 B Uw2 F R2 D' Rw2 U2 Rw D' B R' Rw' B2 D2 Fw' Uw2 Fw Rw B ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D B' U' B2 L2 B2 U2 L2 B L2 B R2 F' U2 L B' D' R D' B2 Fw2 U L2 B' U Uw2 F R2 B2 D' Uw2 Rw U B' Rw' Fw2 L Uw' L B Rw' D2 Uw F' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U F' U2 B R' L2 U B D' L D2 L D2 R2 F2 U2 F2 L' B2 R2 D2 Uw2 F Fw2 U Rw2 U' F B' U D2 Fw2 D R' L2 Rw' B' Fw2 L Uw' B2 Fw2 Rw' F2 B2 Fw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L' D2 L2 U2 R' U F L D2 F2 B2 R' D2 L' B2 R L F U Fw2 R D Uw2 F2 Uw2 Fw2 L D L U2 Fw2 U Fw L D' F2 Fw Rw Fw2 U B Fw' U2 Fw' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F U B2 R2 U R2 L2 B2 L2 U B2 U' R2 F L' B2 R2 U L2 U D Fw2 U F2 Fw2 Rw2 F' D' F B' R2 Rw2 U F2 R' Rw D' L2 Rw Fw' R2 L2 Rw Uw' R' Uw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R' L D' B L' B U' R U' B' L2 U2 D2 F' B' U2 B' L2 B2 Uw2 L' Uw2 B Uw2 F U2 Uw2 Rw2 B L D2 L Uw' L' F2 Uw L2 B2 Rw D2 Rw2 Uw R2 B ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 D R2 B2 U2 L2 F2 U2 F2 D F2 D F' U' R' U' R2 L U2 B' D' Fw2 R' F2 B' Rw2 F2 Fw2 L' B' R Fw2 Rw2 D' R Fw2 Uw' L' Fw2 Rw' D' F2 Rw2 D Uw2 F' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("B2 U2 R2 F D2 R2 F' U2 L2 B D2 R2 L F2 U2 L D L U' R F Uw2 R' U L' Uw2 B2 Fw2 R B2 Fw2 U R' Fw R F' Rw2 U2 Fw' R Uw2 B' Rw' F2 Uw Fw Rw ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D L2 D' R2 D' F2 B2 U' L2 U' B2 R' U' F' L' D' R' L U D2 L Rw2 Uw2 F U' B Rw2 B2 D Uw2 R2 B Rw B2 U L2 Fw2 L' Fw Rw' Uw2 Rw B' U' L2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("L2 F' U2 D R U B' L B R2 L U2 F2 L2 U2 B2 R' B2 L' U2 R Fw2 U2 L' Rw2 D L' Fw2 D Uw2 R B2 L2 Uw2 Fw' L U2 L2 Rw' Fw2 Rw U Uw' R' D' B2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 L' U2 B2 L2 U2 F2 L' U2 R2 U' L' U D' F2 L2 B' U' D2 Fw2 R Fw2 D' Fw2 D R' Uw2 F2 L' U L2 Fw' L2 F2 R2 U2 Rw' U Uw Fw' R2 L' F' D' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 L2 F' U2 R2 F' B' R2 F D2 F2 R' D' L2 F' D' F' L' B U' Fw2 R2 Rw2 F R' U2 Uw2 F2 R' F2 R' Fw2 Uw' Rw2 D' L B D' Fw2 Rw F2 Uw R B2 D' ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F2 U2 R2 D L2 F2 B2 D2 F2 U' B2 D L B' U2 D L D' F2 R' Fw2 D Fw2 D' R' F2 Rw2 D Uw2 Rw2 U B Fw' R' F B' Uw2 Rw' F2 U L Fw Uw2 R ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("D2 L D2 F2 L' B2 D2 R U2 F2 D B R L2 D L2 D' F' R Rw2 Fw2 U2 D B' U' D2 Fw2 Rw2 F' R2 F U' Rw D' L Rw2 Fw R' B Uw' F' L2 F2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U' L2 F D2 B R2 F L2 B' R2 F2 R L2 F' R U' R' L D F2 Rw2 F' U2 B R L2 Uw2 F' U2 Uw2 R' Uw F2 D' L2 D' L' Rw D Uw' L Fw R2 Fw2 D2 ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("F D R2 U2 R D B U B2 U2 R2 U L2 B2 R2 L2 D2 F' L2 Rw2 B Uw2 B L' F2 L2 Fw2 L' F2 L2 F2 Uw B' U L' Uw' B2 Rw' D B2 D Rw Uw' F ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("R D R2 U' F2 D' L2 B2 U' F2 U' B2 U' B' U D2 F L2 D' R' Rw2 F' U' Rw2 B Rw2 B2 R2 Rw2 F L2 F' U' Rw' D2 F L2 Uw2 Fw' Rw Fw' D' F' Fw L ", "derivation_7_to_8.txt", moves);
find_solution_with_extra_moves("U B U' D2 B' L U' L' B' L2 F2 L2 D2 F B2 R2 L2 D2 B' R2 U Fw2 D2 R' B Uw2 F2 Fw2 L Uw2 F L2 Uw2 F2 Uw R2 B' Rw2 F2 R2 Fw L' Uw' R2 Fw' R L2 ", "derivation_7_to_8.txt", moves);*/


    cout << "\n🔢 Total solutions (d0) found: " << solution_counter << endl;
    cout << "\n🔢 Total solutions (d1) found: " << solution_counter1 << endl;
    cout << "\n🔢 Total solutions (d2) found: " << solution_counter2 << endl;
    cout << "\n🔢 Total solutions (d3) found: " << solution_counter3 << endl;
    //cout << "\n🔢 Total solutions (d4) found: " << solution_counter4 << endl;


    // Stop the timer
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    cout << "Execution time: " << duration.count() << " ms" << endl;
    return 0;
}


//next steps: have one big dataset with all depths
//load the full dataset into arbeitspeicher once, then use it constantly