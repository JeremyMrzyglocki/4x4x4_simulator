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
int solution_counter1 = 0;  // Global counter to track the number of solutions found
int solution_counter2 = 0;  // Global counter to track the number of solutions found
int solution_counter3 = 0;  // Global counter to track the number of solutions found


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


// Trackers
/*
int y_rotationTestFailures = 0;
int y2_rotationTestFailures = 0;
int y3_rotationTestFailures = 0;
int m_mirrorTestFailures = 0;
int s_mirrorTestFailures = 0;
int e_mirrorTestFailures = 0;
int sm_mirrorTestFailures = 0;
int ms_mirrorTestFailures = 0;
int z_rotationTestFailures = 0;
int z2_rotationTestFailures = 0;
int z3_rotationTestFailures = 0;
int x_rotationTestFailures = 0;
int x2_rotationTestFailures = 0;
int x3_rotationTestFailures = 0;*/


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
void search_this_scramble(const string& scramble, const string& search_filename) {
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
}

void find_solution_with_0_extra_moves(const string& scramble, const string& search_filename) {
    cout << "\n🔍 Searching for solution to scramble: " << scramble << endl;

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

void find_solution_with_extra_moves(const string& scramble, const string& search_filename, const vector<string>& moves){
    find_solution_with_0_extra_moves(scramble, "all_five_move_scrambles_unique_states.txt");
    find_solution_with_1_extra_move(scramble, "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_2_extra_moves(scramble, "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_3_extra_moves(scramble, "all_five_move_scrambles_unique_states.txt", moves);
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

    string initial_state_after_scram = fixLetterOrder(translate_scramble_to_moves("Uw F Rw' F' L2 Bw"));
    cout << "Initial state: " << initial_state_after_scram << endl;
    //search_this_scramble("Fw' Lw D Rw Fw Rw", "all_five_move_scrambles_unique_states.txt");
    //find_solution_with_extra_moves("Fw' Lw D Rw Fw Rw", "all_five_move_scrambles_unique_states.txt", moves);
    //search_this_scramble("Fw Lw D Rw Fw Rw", "all_five_move_scrambles_unique_states.txt");
    //find_solution_with_extra_moves("Fw Lw D Rw Fw Rw", "all_five_move_scrambles_unique_states.txt", moves);
    //search_this_scramble("Fw Rw D Rw Fw Rw", "all_five_move_scrambles_unique_states.txt");
    //find_solution_with_extra_moves("Fw Rw D Rw Fw Rw", "all_five_move_scrambles_unique_states.txt", moves);
    //search_this_scramble("Fw Rw D2 Rw Fw Rw", "all_five_move_scrambles_unique_states.txt");
    //find_solution_with_extra_moves("Fw Rw D2 Rw Fw Rw", "all_five_move_scrambles_unique_states.txt", moves);
    //search_this_scramble("Fw Rw D' Rw Fw Rw", "all_five_move_scrambles_unique_states.txt");


    /*
    find_solution_with_extra_moves("D B2 L D B L U' R F B L2 B L2 D2 F2 U2 R2 U2 F U2 Uw2 L B Uw2 F' D2 Rw2 F B2 Fw2 U2 Uw2 B2 D' R Uw R' F2 U Fw' L' U Rw U Uw Fw2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("R2 U2 L2 U' L2 F2 U D2 F2 L2 U2 L F R2 U' D' B' L2 F' L B Fw2 L2 Uw2 R' D F2 R D' Fw2 D Uw2 L2 Fw U B L2 Fw' U' Rw' F2 Fw' Uw F Rw2 Uw2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("R B' L2 U2 R' D2 L' B2 R' B2 U' R2 U F B D' F2 B2 Fw2 L' D' Uw2 L2 B2 R Fw2 U Rw2 D R2 D Fw' U2 R2 U' B' Rw U' Uw' R' Fw L D' Uw2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("L D F2 L U F B U R2 D2 F2 U2 R2 D R2 U F2 L2 F' B2 Uw2 F' B Uw2 Fw2 D' R2 F' U2 Uw2 Rw2 D F L B2 Rw F2 Rw' B' Uw R' F Fw Uw2 Fw' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("U F2 R2 U' B2 U2 R2 D B2 U2 L2 F L' B' U' R' L2 B U D R Fw2 Uw2 R2 L F2 U' Rw2 F2 Fw2 D' L' U Fw' Uw2 L B2 U Rw' U2 B' Fw' Rw' U Uw R' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("U2 L' U2 D2 F2 R F2 R' F2 U2 L' U' F' L D' R D' F2 B2 R Fw2 R Rw2 U' Fw2 D2 R' D' L' Fw2 U2 D' B' Uw2 L Fw Uw R' Rw' F2 U Uw2 Fw2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("R B2 U2 R D2 R' D2 L D2 R2 U F2 R F' B L B D R B2 Fw2 Rw2 U2 L' Fw2 U Fw2 R B2 L' Rw2 Fw L2 B R' Fw' Rw' F U' R2 Fw Rw2 U2 L2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("D' R2 B2 U B2 U2 R2 D' F2 U' B' L2 B2 L U' D2 R2 U' R' D Fw2 L Rw2 Uw2 R F2 U' R2 U2 Fw2 D2 Uw2 L' Fw D F' L' U Rw' Fw' Uw' F Rw B' R2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("B2 U2 L2 D R2 D L2 F2 U' L2 U' R2 B' R2 D' L F U R U F Rw2 U Fw2 D2 Rw2 F' U' R2 F' Fw2 D' F2 D Rw B' D2 L' Rw Uw' R' Rw' U B' Uw' B2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("D2 R' U F2 D F2 B2 R2 U B2 D B U L F R2 U D F' R2 Rw2 U Fw2 U' D2 Rw2 B' L2 B2 D2 B Rw2 D2 R Rw B' Rw2 U2 R Fw2 U' Fw' U Fw2 Uw' F2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("D2 L2 F2 L2 F2 D R2 B2 U R2 D2 B U2 F D' F2 U' L D' F R2 Rw2 Fw2 L U R2 F2 Uw2 B2 L' D' Rw2 Uw2 R' Fw' L D2 F' Rw D2 Fw L2 D' Uw2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("B D2 B2 U2 L' D2 R' B2 R' F2 B2 R U' R2 F' D2 F' R U2 R Uw2 R Fw2 R D2 Uw2 Rw2 F2 Uw2 B L Fw2 D B Fw2 R Uw' L2 Uw' Fw Rw' F' Uw Rw2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("F2 U2 F2 U' R2 U' R2 F2 D' B2 U F2 L U2 L' U' R' F L D' Uw2 F' D' L2 B Uw2 B' Fw2 U L2 D Rw' D' B2 Rw2 D2 Rw' Fw' R Fw D' B Uw B' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("U' R B2 D2 L2 D2 B U2 F' L2 F D2 F' U B L' D2 F B2 D Fw2 D L2 Uw2 Rw2 D2 Fw2 D' L' Fw2 R2 U' Fw R F2 B2 L2 Fw Uw' B2 R Fw' Rw F ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("L' D' R' B' L2 F' U F D' F2 D2 R D2 F2 R2 U2 R D2 R' Rw2 Uw2 Fw2 D2 L' U Uw2 R D2 B2 D' Fw R2 U' F' Fw' D' Rw Fw U2 F B Fw2 L ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("L' U2 D2 B2 U2 R F2 L' B2 D L F L B U' L2 B R B Rw2 B2 U' Uw2 F2 U2 Rw2 U' L U' R' B Rw2 Fw' L2 F2 Uw F' B2 Fw D2 L2 Uw Rw ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("B R2 D2 F' R2 B' U2 B' U2 L2 F' D' B' D R2 F2 L' D' R' F' Fw2 L U' Fw2 D' R' U2 B2 Fw2 L' D Uw2 B2 Fw' U2 R2 U Fw2 Rw' D2 Fw D B' Fw Rw U' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("D2 B2 L2 B2 U B2 L2 U D2 R2 U' D' F D L2 F2 L2 F R D' R Fw2 R2 Uw2 F2 U' Fw2 U L2 B' Uw2 L2 Uw2 Rw' Uw2 Rw2 B2 D2 Rw Fw' L Fw' D' R' Fw Uw' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("R2 L' U2 R2 U2 F L2 B D2 L2 F2 D2 B' U' B2 L' F' U L' D Rw2 Uw2 F Fw2 L' Uw2 F2 R L' Uw2 F Uw2 R Uw' Fw2 D' B D Uw Rw' D2 Fw' D' Rw' B2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("B L F R2 F' R D2 F U R2 D' L2 D F2 U L2 D F2 L2 F Rw2 B' Fw2 Uw2 F2 R D2 F2 R Uw2 L D2 R' D Rw2 Uw Fw2 R L2 Rw B2 Fw2 Uw' Fw' D F' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("D2 B U2 R2 L2 F L2 F2 U2 L2 F' U F2 R' U' R2 U' D2 L' U2 F Fw2 U' F2 Uw2 B' Fw2 L2 Rw2 Uw2 L2 B' U' B' L U' Rw' U2 R' B Uw' Rw' Uw2 L D F2 Rw2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("U' B R B2 L F2 L D2 R' L' B2 U D2 F L F' U B L' Fw2 L' U2 F2 Fw2 D2 R2 U Rw2 Fw2 D' Fw' R Fw2 D2 F2 Rw D2 B Rw' Uw F2 Uw' L2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("L' U' F2 B2 L2 F2 B2 L2 D2 R' F2 R F2 B U R F B L D' Rw2 B U2 Rw2 U D' F' D' B2 Rw2 Fw2 D Rw' F Rw F B2 R Uw' Fw2 Rw' F' Uw2 L B' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("U2 F R2 D' B L F' D' R' B2 U F2 D L2 F2 L2 D B2 R2 U B2 Fw2 D' R L2 Uw2 Rw2 Fw2 R' D R2 B2 D' Fw' D' F L' U' Rw F2 Uw' F2 L Rw Uw' F' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("L2 B2 D B2 U' R2 D2 L2 D L D F' L' F2 R' U B R Rw2 U2 B' U' Uw2 B Fw2 D2 F D F' B Fw2 Rw D2 R' Rw' B' Uw Fw D2 Rw2 B' U2 Fw' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("B2 D2 B2 R2 B2 U' L2 D2 F2 U' B L B2 U2 F R' L U2 R2 Rw2 Uw2 F L B R2 L B' R' Uw2 B' D2 R2 Uw F U' Rw2 Uw2 Rw' Fw' Rw2 U' Uw Rw Uw2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("B U R F' U2 D' R' U L' U' B2 D2 F2 U L2 U2 R2 B2 D Uw2 R Rw2 U L' U' D' B2 Fw2 R' U2 F' Fw2 Uw2 R' Fw' R Rw Uw' Fw' D' B2 Uw L' Rw ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("F' U2 R2 D2 F2 D2 L' F2 B2 R' B2 U F' D2 R D' R' B2 R Fw2 R U' D2 L2 Uw2 Rw2 B2 Fw2 R U2 Fw D2 Fw' R2 D2 L' Uw Rw' F' Fw' D' Fw R' Rw2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("F' L2 B2 L D2 L2 U2 R' U2 B2 U2 D2 B R' L2 B U' R U2 D F2 Fw2 R' Uw2 Rw2 U Rw2 U R' Fw2 R2 D' L B Fw L D' B D' Uw' F2 B Uw2 Rw' Uw F2 D ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("B' U2 D2 R' B2 R F2 R U2 B2 R2 L' F2 B R2 L' F L2 U2 D' B' Fw2 Rw2 D Uw2 B2 L D' Rw2 Fw2 L2 D2 B' L Fw L U' Uw' F' B' L' Fw Uw Rw F2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("L2 U2 F2 R' L' F2 D2 R F2 L B2 R' D L D B' L D2 F2 D' B2 Uw2 B2 L D Fw2 U2 L' Rw2 Uw2 Fw2 U F2 Fw' Uw2 F2 B Fw2 Uw F2 Uw Rw' D2 B L' U ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("D2 L2 B2 U' F2 L2 D R2 U' R2 U F2 R' F' U2 L D2 B U D2 Rw2 D B U F Uw2 F' R2 U Rw2 U R2 Rw' B' Uw2 R B' Uw' F R2 Rw' F2 Rw2 Fw' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("L' U R2 D F2 D2 B2 D' R2 L2 D B2 D B' D2 L' U R' L' D2 Uw2 Fw2 D R2 L' Fw2 R' Fw2 U' F2 R' Uw2 L' F' Fw' R2 D R' Uw' L2 Rw F' Uw F' Fw L2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("F' D2 L D' B2 D F R U' D2 B' R2 D2 F2 B' R2 L2 D2 B' R2 B2 Uw2 L' Rw2 Fw2 R' D R2 Fw2 L U2 D' B Fw2 L2 D Fw' R2 F U' Rw' Fw D' Fw Rw2 Uw ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("L U B2 D' F2 D R2 D2 R2 B2 R2 D' R L2 D F2 R2 D F' L2 Fw2 L U' Fw2 D2 L Rw2 Uw2 L' D' R Uw2 Fw L2 U R F2 Uw' B' Rw' F Uw' B2 L' Fw2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("U2 L U2 D' F2 B' R U D2 B2 L F2 L B2 D2 B2 R' F2 D2 R2 B Rw2 U' Fw2 D L D2 Fw2 L' Rw2 U2 R2 F2 R' Fw L B U D2 Fw L Uw' Rw D F2 Uw Rw ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("U L D2 R' B2 D2 B2 D2 L2 F2 L' F2 R B' L' F' B' R2 D' F' L' Fw2 R' L U Fw2 L U2 L Fw2 R D2 R Fw U2 Fw' U F' Fw Rw' D2 Uw L' Fw Uw2 Rw ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("B2 R B' U2 L2 U2 F2 U2 F' B' D2 B' D R' F2 U2 F' B' U Uw2 Fw2 L2 F Uw2 R2 Rw2 F R' L2 F' L Uw F2 D' Rw2 Fw2 Uw F' Fw' Uw L2 Fw' L2 U' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("F' D' B U F2 D B2 U R2 U B2 U2 L2 D' B D2 B2 R' F L' B' Fw2 Rw2 B' U' Rw2 D' Fw2 D2 F2 Rw2 B R2 Rw B2 Fw2 L U' R2 Fw D' Uw2 R Fw ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("B2 L2 U' B' R' B' U' D2 F U2 D2 F B2 D2 B R' L2 F' Uw2 B D' Uw2 F' Rw2 Fw2 D2 F R2 L2 B' Rw U' F2 D Rw U Uw R2 Fw R' D2 R' F ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("L2 U' L' D' F L' B D2 L' F2 B2 D' B2 D2 R2 U2 L2 D' F2 D Uw2 R B2 Uw2 Rw2 B U2 F2 L' D2 F R2 Uw F D2 B2 U2 Rw' Fw' Rw2 Uw2 B2 Fw U2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("F' R' B L2 F L2 F' R2 U2 B2 L2 B' U' D2 F D L U B R B2 Uw2 Rw2 D R' F2 Rw2 Fw2 L' U2 B2 D' F Fw L Rw2 Fw' L Uw' L B R' Uw' R Uw' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("R2 B2 R2 B2 U2 L U2 R' F2 R2 F B' L D' L2 D' L' D B2 U Fw2 Uw2 F' L Fw2 U2 L' Uw2 Fw2 R U2 R2 U' Uw F' Uw' Rw2 B Rw' U Fw' R2 Uw' Fw2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("R2 D' F2 U' L2 U F2 D2 L2 U' L2 F2 B L F' U' L' D' B2 R B Uw2 L Uw2 B Uw2 Fw2 R2 B' U2 F' L' F' D F B2 Fw2 Uw B' Fw R' Uw R2 F2 L Rw ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("F U L B2 U F' R' B2 L2 F L2 U2 L2 D2 F' L2 B' D B2 Uw2 F R' Fw2 L' Rw2 U2 F' L Uw2 B Fw2 D2 Uw' F2 B2 L' Uw' F' U2 Rw D' Uw' Rw2 D ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("B R' B2 R2 D2 F' U2 D2 F2 R2 F2 R F' B2 U L D R' U2 Rw2 Uw2 F2 U' Rw2 D' Uw2 B2 D' F' Rw2 U' B Rw F2 R2 Uw2 F B' R2 Uw L' Rw' Uw' F ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("D' L D2 R2 F2 U2 D2 R2 F U2 R2 B' R' L' U' R F2 U D Uw2 F' Fw2 U R2 D' Uw2 R2 B D2 B' Fw2 L Rw' B' Rw2 D' Fw R2 Uw' F' D2 Rw2 Fw' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("B2 R U2 R D2 R2 U2 D2 L' U2 L B D' B2 U F' U2 F L D' L2 Rw2 B2 U' Uw2 L B2 L F2 Rw2 Fw2 D' F2 L2 Fw' L' Fw2 D R2 Fw' U' Uw' F2 Uw2 Rw' B' Uw' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("F' L' B2 U' R2 U R2 D R2 U L2 F2 U2 F2 L U2 B' R F' R' B Rw2 F' Fw2 L2 Uw2 B Rw2 D' Rw2 F2 R2 L2 Rw' F' R2 U2 Rw' B Fw D' L2 U' Uw' R2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("L' B D2 L D2 R2 D2 F2 L U2 F2 B2 R B2 U L2 D F' D2 R' D2 Rw2 F Fw2 U F2 L2 Fw2 Rw2 B D Fw2 U' F' Rw U' L' U' Fw' Uw2 Fw' Uw Fw Rw ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("B2 D R2 F2 U' L2 F2 U' R2 D2 B D2 B' L' F' B2 U B L U Fw2 R B2 Fw2 D' F2 Fw2 Rw2 D2 Uw2 L B2 U' Fw' L D2 F2 L2 Rw Fw' L Rw2 U2 Fw2 L Rw ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("D L' F L F' U' R' B' U F2 D B2 L2 U L2 F2 R2 U D2 R Rw2 F' R2 Uw2 L2 U B L2 Rw2 U' F Uw2 L2 Rw F Fw2 Rw F' Rw Fw' D L2 Uw Fw U L2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("B L2 U2 L U2 D2 R D2 F2 D2 L2 U F U' D2 R2 F' D R' Uw2 F2 R F2 U Fw2 D B2 R Rw2 D Fw2 U' Fw U L2 B2 Rw Uw' B' Fw L' D2 F B2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("D R L2 F2 D L2 B2 D' F2 U D2 B2 U F2 L' U F' B2 U' L2 F Uw2 R F2 Uw2 B' R D2 Uw2 B2 L B Uw2 F2 Uw F L' Rw2 Fw2 L' Fw Rw2 Uw F Uw F2 U' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("R2 B D2 L2 F' B' R2 B D2 R2 L2 F U L F2 L' D2 F' R' D B Fw2 D Uw2 L2 B' L2 Rw2 B2 R2 L2 U' R Rw2 U' Rw' U F Rw2 U2 Rw Fw U2 Fw2 L Uw2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("D' R2 B2 R B2 R' U2 L B2 L' B2 R' B' D' R' B U' R2 U' B D' Fw2 R' Fw2 U2 D' Fw2 U Fw2 L' Uw2 L2 B2 Fw' Rw2 D' Fw' U' R B D' Rw' Fw' Rw B' L U' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("L2 F' L2 F2 B2 L' F2 B2 L' B2 L B2 D2 R2 B U2 R' D L' U' F' Rw2 U Fw2 L' Uw2 Rw2 F2 D' R2 U' R D2 Fw' Rw2 D Fw2 R' F2 Rw U' B' Uw' Rw' B' Uw' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("B' D2 F2 B U2 L2 F' U2 D2 B' L2 D' L' F' R2 B2 D' B' U' F R Rw2 B Uw2 Fw2 Rw2 F' R Fw2 R2 Fw2 L F Uw R' Rw2 Uw' F2 U F2 D' Fw' L' Rw' U Uw2 B ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("U R2 L2 U2 D' F2 D F2 D' F2 B R' F2 L B' U' D2 B Uw2 B U2 Uw2 Rw2 F' L2 U' D' Uw2 Rw2 F D Rw' U' F' U2 F2 D2 Fw' D Rw Fw' L' Fw2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("L2 D2 F2 L2 U2 B L2 F' R2 B D2 R2 U F' B R F B2 R F' L' Rw2 Uw2 R B' Fw2 U2 F Fw2 R2 Rw2 Uw2 R' D Fw2 U B Uw' L2 Rw F Uw' R' Rw U2 Uw ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("L' U2 R' D2 F2 R' D2 R' L2 F B2 U2 F U B' L' F B D L Uw2 Rw2 D' Fw2 L' U2 D R Fw2 U' Fw2 D' F Fw' R' U2 F2 Rw D Fw U2 Uw Rw2 D' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("D2 F U2 B' R2 D2 L2 B' U2 L2 B D' R' U2 F' D' B R2 U L2 Uw2 L' Uw2 R D2 Rw2 D' F2 D2 Fw2 R U' B U2 Fw' R2 U2 Fw2 D2 Uw R2 Rw' Uw2 Fw R' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("R' F D2 L2 U' L2 U' B2 U2 D R2 F2 B U R' D' L2 F U' R Rw2 Fw2 R U Fw2 L' U2 Fw2 R U R2 U' R' Fw Rw2 B D' F2 R' Rw' D Fw D R' Fw Rw ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("U R F2 L F2 R2 U2 L' F2 U2 R D2 F' R F2 U' D2 R F B2 Uw2 B Uw2 F' R2 Fw2 U' Rw2 U F2 Fw2 U L D' B2 Rw' F U Fw Rw' D' Uw2 F' Rw' U ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("L D2 B2 U2 L2 D2 R2 B2 L' D2 B2 R' D R F D L2 U R B' L Fw2 R U Fw2 Uw2 F2 U F2 Fw2 D L' Fw L2 F L2 F R Uw L' Rw' Fw2 U2 Uw L' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("R U B2 U2 D2 B L2 F' D2 F' U2 F2 D2 F' R F' L' D B2 R B' Rw2 F Fw2 L2 Uw2 L2 B Fw2 U' Rw2 U B' Rw' Uw2 L2 U2 B2 D Fw U Rw2 Fw2 U' Uw' B ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("R2 D' L2 U2 L2 D F2 R2 F2 B2 U' L F L B' D' F R' F2 L' Fw2 Uw2 L B2 U' Fw2 Rw2 D R L F2 D2 B' Uw2 F2 Fw' L2 Rw' D2 F Uw Fw' Uw' Rw F2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("L' D' L B2 U B2 R F2 D' B R2 U2 F D2 B2 L2 B R2 B D2 F2 Fw2 L' Uw2 F2 Uw2 Rw2 B' R Fw2 R B2 D2 B Uw Rw2 U2 R' B2 U2 Fw Uw L B2 Fw2 R2 L ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("F' R' B' R2 D B U F U' L U2 D2 L U2 L' D2 B2 R' B2 R' Fw2 R' U Uw2 Rw2 F2 U' L2 U' B2 Fw2 L F2 Fw' D2 F' Fw Rw' F' U2 Fw D2 B' Fw Rw2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("D2 B R B2 L' U B D B2 L2 B' D2 R2 L2 F' R2 B R2 U2 B2 R Uw2 B Fw2 L' B L' Rw2 Fw2 R' F Uw2 L2 Fw2 Uw R L2 U2 L2 Rw Fw L' Uw Rw D' Fw2 U ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("R F2 R B2 D2 R' U2 L' B2 R U2 L D' L' U' R U' L2 B' L U' Fw2 R U Rw2 Uw2 R2 D2 B2 Uw2 F2 L' F' Uw2 Fw D B2 Rw2 Fw' Uw Fw L Fw Rw' Fw ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("B' R2 F2 R U B' L B U2 R' D2 L B2 R' L' F2 D2 B2 U Fw2 R' Rw2 Uw2 F2 B Fw2 U2 Fw2 R' Uw2 F' Uw F2 Uw Fw2 D' Rw' D2 Fw L2 F2 Rw D2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("U2 R' B2 R' U2 D2 R F2 L D2 L' B2 U L' B D2 R U' F U2 B Uw2 R' U' Uw2 L' U Rw2 U R' Fw2 R B' Fw' R' U2 L' Rw F' Uw' R' U' Rw' F Uw ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("U' F2 D F2 B2 L2 U2 D L2 B2 R L2 U2 L B L2 F D L2 B L2 Rw2 D R2 Fw2 D2 L' Fw2 U F2 L D R' L' Fw' D' R L2 Fw2 Uw' L' U Rw Fw2 U Rw ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("L F2 U2 L2 B2 R' B2 R' U2 B2 L' B2 D F B2 D' R2 F L2 F' R' Rw2 Uw2 F' U2 Fw2 Uw2 L F2 U2 L' F Rw2 F' D' Uw L2 F' R2 F' Rw' U' Rw2 D' Uw' Rw B2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("F2 U' L B' R D2 B2 U' F' R U2 R' U2 L D2 B2 L2 F2 L' B2 Fw2 R' Rw2 Uw2 Fw2 D2 B' R B' Rw2 D2 F D' Uw' R2 B Uw2 Fw L' Uw F D2 R2 D' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("L2 U2 L2 F U2 B2 D2 L2 F D2 F' B' L' F R U B D R2 U' B Uw2 Fw2 U' D2 R2 B' L2 U' Rw2 B U Uw2 B2 Rw' D' Fw2 R F2 Rw' Uw F R L2 Fw2 R' Rw ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("L' U2 F2 D2 F L2 F' B2 R2 D2 F R2 D' R F' R' B D2 L F' Uw2 R2 Rw2 D R2 L' F2 D' Rw2 D' Rw2 Uw2 L2 B' R' Rw2 D Fw' Uw' F2 D R' Rw' B' Uw' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("B' R2 L F2 R2 F2 U2 R B2 R2 F2 B' U' L' F2 D R D2 R L Fw2 D Rw2 U' R2 F2 Uw2 F2 L Fw2 L U2 D2 Fw' L2 U2 Fw U' R2 Uw' R D2 Rw2 Fw Rw ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("F' U' L F2 R D2 L' D2 L U2 B2 L' U2 R2 F' D' R2 F R D' F' Uw2 L' U2 Fw2 R' B U2 R' D2 Uw2 L' Rw2 B Uw' F2 B D Rw' Fw Rw2 Uw R' D2 L Fw ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("R B2 L2 F U2 L2 U2 B2 D2 F' D2 B' U2 L U2 B' U' L' F U L2 Rw2 Fw2 R' B' U2 D2 Fw2 U2 R Uw2 L' B2 Uw' L D' L2 F Uw L' Rw U' Fw' R2 B2 L2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("D R' L' U2 L' F2 D' B L2 D2 F2 D2 B R2 F2 L2 B' R2 B L F' Fw2 U2 D' B' L2 Rw2 U Uw2 B Uw2 L2 U' B2 Rw Fw2 U' R' U2 L2 Fw' D2 R L Fw D ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("R B' R2 U2 B2 L2 D2 B' U2 L2 B' R2 B2 U' B2 R' B' L2 D2 B2 L' Uw2 Fw2 U' L2 Rw2 U' L' Rw2 D' R2 B2 R B Fw' U2 B R Uw' Rw' B Fw Rw2 D2 Fw D' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("D' B' L2 F R2 D2 F2 U2 R2 F' L2 F' L D' L2 U' R U F D B Rw2 U' Uw2 Rw2 B D B2 U' Rw2 F2 U2 D' Fw2 Rw' B2 Fw2 D Rw F Uw Rw' Uw L2 F2 Uw ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("B R' L B' R D F B2 L U2 F2 L B2 U2 F2 U2 R' U2 D2 L' Fw2 Uw2 L U2 L2 B' Fw2 L2 Rw2 Uw2 R2 F' Uw F' D' B' U R' Rw' Uw Rw2 Uw R2 F2 L2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("R B2 U2 R2 F2 D2 L' F2 D2 L F2 R' U F U F2 U' L' F' L' D' Uw2 R D R Fw2 U R U' Rw2 Fw2 D B2 R Fw D2 L2 U2 Fw2 Uw' Fw2 L2 F2 Fw' Uw2 Fw2 ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("R F2 D2 L2 U' R' L D B U2 L2 U2 F2 D' B2 U' F2 R2 U B2 Uw2 Rw2 D' B2 Uw2 L U2 L Rw2 Fw2 D' F2 Uw2 B' D2 Fw' R2 Fw Rw' U' Uw2 Fw D R' Uw' ", "all_five_move_scrambles_unique_states.txt", moves);
    find_solution_with_extra_moves("D2 B' U2 R U' L' B D' L2 F2 L2 U F2 D F2 D L2 B2 R D2 F' Fw2 D' B2 R' Rw2 Uw2 L U' R2 B2 D B Fw U F2 B L' Rw2 Uw Fw' U F2 D2 ", "all_five_move_scrambles_unique_states.txt", moves);
    */
    //find_solution_with_extra_moves("B' L' U' R2 B2 D2 B2 R2 U' F2 D2 R2 B2 L' U D2 R U' F L2 D Fw2 R' L2 U' B2 L2 Fw2 D' Rw2 D R' D' B' L' Fw' Rw2 U' L' B2 Rw Uw L' Rw' F2 Uw' Rw ", "all_five_move_scrambles_unique_states.txt", moves);
    //find_solution_with_extra_moves("R2 U' L F2 R L' F2 B2 L F2 D2 B R' L D' R' U' R Uw2 R Rw2 F' Uw2 L2 Uw2 L Rw2 Fw2 R F' Uw R U' Rw2 B2 Rw' F2 D' Fw' L Uw2 R' Rw ", "all_five_move_scrambles_unique_states.txt", moves);
    //find_solution_with_extra_moves("B2 R2 D2 B D2 B' U2 F' D2 R2 F D F B2 R D2 B R2 F D' Fw2 U Uw2 L Rw2 U2 L U2 D' B2 D Uw2 F' R' B Fw Rw' U' B' D2 Uw2 B2 Fw R' ", "all_five_move_scrambles_unique_states.txt", moves);
    //find_solution_with_extra_moves("B' L' F L U' F2 L' U L2 U B2 U' F2 B2 L2 U2 R2 D' R2 L' D' Fw2 R' F2 R Rw2 Uw2 B' R B' D2 Rw2 U' L2 Uw' R U' Uw Rw B2 Rw Uw' F' Rw U' ", "all_five_move_scrambles_unique_states.txt", moves);
    //find_solution_with_extra_moves("U2 F' R2 U2 L2 F2 L2 B U2 L2 U2 B' R U2 D' B D L F B Uw2 B2 Rw2 B' Fw2 U Fw2 U' F B' U2 D Rw F2 Rw Fw2 R2 L Fw2 Uw Rw Uw2 F Fw2 U ", "all_five_move_scrambles_unique_states.txt", moves);
    //find_solution_with_extra_moves("U2 B2 R' F2 D2 L' B2 R U2 F2 B U L2 U F' D2 F2 L D2 Fw2 U' Rw2 U Uw2 F2 R' U' Fw2 D' F' B2 Rw2 U' R' Rw' Uw Rw2 Uw B Rw' U F ", "all_five_move_scrambles_unique_states.txt", moves);
    //find_solution_with_extra_moves("D2 B2 D B2 D2 B2 L2 U2 B2 L' B' L2 F2 R B U L U' R' Rw2 U' B' Rw2 Uw2 F L2 U B2 Uw2 Rw2 F2 D' Rw D2 F2 R B' L Rw' Fw' R' Uw2 Rw B Uw' ", "all_five_move_scrambles_unique_states.txt", moves);
    //find_solution_with_extra_moves("F2 D B L2 F R U2 B' U' D2 F D2 F' R2 B R2 F2 R2 D2 Uw2 L2 F' L2 F' Uw2 Fw2 D' B' Uw2 R2 F' L2 Rw' B2 L F' Fw L' U Uw2 Rw' Fw' R2 U2 ", "all_five_move_scrambles_unique_states.txt", moves);
    //find_solution_with_extra_moves("L2 U2 B2 L2 F' B2 D2 B2 U2 L2 U L2 B2 L' U2 D' L U D' Fw2 L' D2 F2 Rw2 Fw2 R F Fw2 D2 Uw2 Rw2 U D2 Uw' R B Fw Rw' F Fw R Uw' Rw2 B ", "all_five_move_scrambles_unique_states.txt", moves);
    //find_solution_with_extra_moves("F D R' L2 F2 B D' F L' F2 U2 D2 L U2 R' F2 R2 B2 R B2 Uw2 Rw2 U F Rw2 U L2 Fw2 Rw2 F' D B L' Rw2 Uw2 Rw' D2 Uw2 Rw' Uw Fw Rw2 D2 Uw2 R Rw ", "all_five_move_scrambles_unique_states.txt", moves);
    //find_solution_with_extra_moves("R' F U R' B' R B D2 L B R2 L2 F' D2 B2 R2 D2 B U2 F' R2 Uw2 R2 B' D2 Fw2 Rw2 B' R D2 L2 B2 R Uw' F2 L' Uw B' R B Fw Uw B' Fw Rw' D2 ", "all_five_move_scrambles_unique_states.txt", moves);

    cout << "\n🔢 Total solutions (d0) found: " << solution_counter << endl;
    cout << "\n🔢 Total solutions (d1) found: " << solution_counter1 << endl;
    cout << "\n🔢 Total solutions (d2) found: " << solution_counter2 << endl;
    cout << "\n🔢 Total solutions (d3) found: " << solution_counter3 << endl;


    // Stop the timer
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    cout << "Execution time: " << duration.count() << " ms" << endl;
    return 0;
}
