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
void saveBackup(const set<string>& validCombinations, int backupIndex);


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



void saveBackup(const set<string>& validCombinations, int backupIndex) {
    string filename = "backup_" + to_string(backupIndex) + ".txt";
    ofstream backupFile(filename);
    for (const string& combination : validCombinations) {
        backupFile << combination << endl;
    }
    backupFile.close();
    cout << "📂 Backup saved: " << filename << " (" << validCombinations.size() << " combinations)" << endl;
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


// Perform a "x-rotation" on the combination
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

string M_mirror_solver(const string& move_sequence) {
    unordered_map<string, string> mirror_map = {
        {"R", "L'"}, {"R'", "L"}, {"R2", "L2"},
        {"L", "R'"}, {"L'", "R"}, {"L2", "R2"},
        {"U", "U'"}, {"U'", "U"}, {"U2", "U2"},
        {"D", "D'"}, {"D'", "D"}, {"D2", "D2"},
        {"F", "F'"}, {"F'", "F"}, {"F2", "F2"},
        {"B", "B'"}, {"B'", "B"}, {"B2", "B2"},
        {"Uw", "Uw'"}, {"Uw'", "Uw"}, {"Uw2", "Uw2"},
        {"Dw", "Dw'"}, {"Dw'", "Dw"}, {"Dw2", "Dw2"},
        {"Rw", "Lw'"}, {"Rw'", "Lw"}, {"Rw2", "Lw2"},
        {"Lw", "Rw'"}, {"Lw'", "Rw"}, {"Lw2", "Rw2"},
        {"Fw", "Fw'"}, {"Fw'", "Fw"}, {"Fw2", "Fw2"},
        {"Bw", "Bw'"}, {"Bw'", "Bw"}, {"Bw2", "Bw2"}
    };

    stringstream ss(move_sequence);
    string move, mirrored_sequence = "";
    
    while (ss >> move) {
        if (mirror_map.find(move) != mirror_map.end()) {
            mirrored_sequence += mirror_map[move] + " ";
        } else {
            mirrored_sequence += move + " ";  // If no mapping exists, keep the move unchanged
        }
    }

    return mirrored_sequence.substr(0, mirrored_sequence.size() - 1); // Remove trailing space
}

string S_mirror_solver(const string& move_sequence) {
    unordered_map<string, string> mirror_map = {
        {"R", "R'"}, {"R'", "R"}, {"F2", "B2"},
        {"F", "B'"}, {"F'", "B"}, {"L2", "L2"},
        {"L", "L'"}, {"L'", "L"}, {"R2", "R2"},
        {"D", "D'"}, {"D'", "D"}, {"D2", "D2"},
        {"U", "U'"}, {"U'", "U"}, {"U2", "U2"},
        {"B", "F'"}, {"B'", "F"}, {"B2", "F2"},
        {"Uw", "Uw'"}, {"Uw'", "Uw"}, {"Uw2", "Uw2"},
        {"Dw", "Dw'"}, {"Dw'", "Dw"}, {"Dw2", "Dw2"},
        {"Rw", "Rw'"}, {"Rw'", "Rw"}, {"Rw2", "Rw2"},
        {"Lw", "Lw'"}, {"Lw'", "Lw"}, {"Lw2", "Lw2"},
        {"Fw", "Bw'"}, {"Fw'", "Bw"}, {"Fw2", "Bw2"},
        {"Bw", "Fw'"}, {"Bw'", "Fw"}, {"Bw2", "Fw2"}

    };

    stringstream ss(move_sequence);
    string move, mirrored_sequence = "";
    
    while (ss >> move) {
        if (mirror_map.find(move) != mirror_map.end()) {
            mirrored_sequence += mirror_map[move] + " ";
        } else {
            mirrored_sequence += move + " ";  // If no mapping exists, keep the move unchanged
        }
    }

    return mirrored_sequence.substr(0, mirrored_sequence.size() - 1); // Remove trailing space
}
/*
string E_mirror_solver(const string& move_sequence) {
    unordered_map<string, string> mirror_map = {
        {"R", "L'"}, {"R'", "L"}, {"R2", "L2"},
        {"L", "R'"}, {"L'", "R"}, {"L2", "R2"},
        {"U", "U'"}, {"U'", "U"}, {"U2", "U2"},
        {"D", "D'"}, {"D'", "D"}, {"D2", "D2"},
        {"F", "F'"}, {"F'", "F"}, {"F2", "F2"},
        {"B", "B'"}, {"B'", "B"}, {"B2", "B2"},
        {"Uw", "Uw'"}, {"Uw'", "Uw"}, {"Uw2", "Uw2"},
        {"Dw", "Dw'"}, {"Dw'", "Dw"}, {"Dw2", "Dw2"},
        {"Rw", "Lw'"}, {"Rw'", "Lw"}, {"Rw2", "Lw2"},
        {"Lw", "Rw'"}, {"Lw'", "Rw"}, {"Lw2", "Rw2"}
    };

    stringstream ss(move_sequence);
    string move, mirrored_sequence = "";
    
    while (ss >> move) {
        if (mirror_map.find(move) != mirror_map.end()) {
            mirrored_sequence += mirror_map[move] + " ";
        } else {
            mirrored_sequence += move + " ";  // If no mapping exists, keep the move unchanged
        }
    }

    return mirrored_sequence.substr(0, mirrored_sequence.size() - 1); // Remove trailing space
}*/

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
    string output = y_rotation(combination_new); 
    return output;           
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

void generate_five_move_scrambles(const vector<string>& moves, const string& initial_state, const string& filename) {
    ofstream outputFile(filename);
    if (!outputFile) {
        cerr << "Error: Could not open file " << filename << " for writing." << endl;
        return;
    }

    map<string, vector<string>> move_groups = get_move_groups();

    for (const string& move1 : moves) {
        for (const string& move2 : moves) {
            for (const string& move3 : moves) {
                for (const string& move4 : moves) {
                    for (const string& move5 : moves) {
                        // Skip if move2 is in the same group as move1
                        if (find(move_groups[move1].begin(), move_groups[move1].end(), move2) != move_groups[move1].end()) {
                            continue;
                        }
                        // Skip if move3 is in the same group as move2
                        if (find(move_groups[move2].begin(), move_groups[move2].end(), move3) != move_groups[move2].end()) {
                            continue;
                        }
                        // Skip if move4 is in the same group as move3
                        if (find(move_groups[move3].begin(), move_groups[move3].end(), move4) != move_groups[move3].end()) {
                            continue;
                        }
                        if (find(move_groups[move4].begin(), move_groups[move4].end(), move5) != move_groups[move4].end()) {
                            continue;
                        }

                        string scramble = move1 + " " + move2 + " " + move3 + " " + move4 + " " + move5;
                        string cube_state = translate_scramble_to_moves(scramble);
                        outputFile << scramble << " " << cube_state << endl;
                    }
                }
            }
        }
    }

    outputFile.close();
    cout << "Scramble results saved to " << filename << endl;
}


void generate_five_move_scrambles_unique_states(const vector<string>& moves, const string& initial_state, const string& filename) {
    ofstream outputFile(filename);
    if (!outputFile) {
        cerr << "Error: Could not open file " << filename << " for writing." << endl;
        return;
    }

    map<string, vector<string>> move_groups = get_move_groups();
    unordered_map<string, string> seen_states; // Tracks unique normalized cube states

    // Counters for eliminations
    int eliminated_letter_order = 0;
    int eliminated_y_rotation = 0;
    int eliminated_x_rotation = 0;
    int eliminated_z_rotation = 0;
    int eliminated_double_rot = 0;

    for (const string& move1 : moves) {
        // Branch skipping (speed optimization by 66%):
        // Skip if the first move is a double move (R2, L2, F2, etc.)
        if (move1 == "R2" || move1 == "L2" || move1 == "F2" || move1 == "B2" || 
            move1 == "U2" || move1 == "D2" || move1 == "Rw2" || move1 == "Lw2" || 
            move1 == "Fw2" || move1 == "Bw2" || move1 == "Uw2" || move1 == "Dw2") {
            continue;
        }
        // Skip if the first move is an outer move (R, L, F, B, U, D)
        if (move1 == "R" || move1 == "R'" || move1 == "L" || move1 == "L'" || 
            move1 == "F" || move1 == "F'" || move1 == "B" || move1 == "B'" || 
            move1 == "U" || move1 == "U'" || move1 == "D" || move1 == "D'") {
            continue;
        }
        for (const string& move2 : moves) {
            if (find(move_groups[move1].begin(), move_groups[move1].end(), move2) != move_groups[move1].end()) {
                continue;
            }
            for (const string& move3 : moves) {
                if (find(move_groups[move2].begin(), move_groups[move2].end(), move3) != move_groups[move2].end()) {
                    continue;
                }
                for (const string& move4 : moves) {
                    if (find(move_groups[move3].begin(), move_groups[move3].end(), move4) != move_groups[move3].end()) {
                        continue;
                    }

                    for (const string& move5 : moves) {
                        if (find(move_groups[move4].begin(), move_groups[move4].end(), move5) != move_groups[move4].end()) {
                            continue;
                        }



                        string scramble = move1 + " " + move2 + " " + move3 + " " + move4 + " " + move5;
                        string cube_state = translate_scramble_to_moves(scramble);

                        // Normalize using fixLetterOrder
                        string normalized_state = fixLetterOrder(cube_state);

                        // Apply y-rotations
                        string y1_state = fixLetterOrder(y_rotation(cube_state));
                        string y2_state = fixLetterOrder(y2_rotation(cube_state));
                        string y3_state = fixLetterOrder(y3_rotation(cube_state));

                        // Apply x-rotations
                        string x1_state = fixLetterOrder(x_rotation(cube_state));
                        string x2_state = fixLetterOrder(x2_rotation(cube_state));
                        string x3_state = fixLetterOrder(x3_rotation(cube_state));

                        // Apply z-rotations
                        string z1_state = fixLetterOrder(z_rotation(cube_state));
                        string z2_state = fixLetterOrder(z2_rotation(cube_state));
                        string z3_state = fixLetterOrder(z3_rotation(cube_state));

                        // Apply double rotations
                        string double_rot_state = fixLetterOrder(x3_rotation(z_rotation(cube_state)));
                        string double_rot_state2 = fixLetterOrder(x3_rotation(z2_rotation(cube_state)));
                        string double_rot_state3 = fixLetterOrder(x3_rotation(z3_rotation(cube_state)));
                        string double_rot_state4 = fixLetterOrder(y2_rotation(z_rotation(cube_state)));
                        string double_rot_state5 = fixLetterOrder(y2_rotation(z2_rotation(cube_state)));
                        string double_rot_state6 = fixLetterOrder(y2_rotation(z3_rotation(cube_state)));
                        string double_rot_state7 = fixLetterOrder(y_rotation(z_rotation(cube_state)));
                        string double_rot_state8 = fixLetterOrder(y_rotation(z2_rotation(cube_state)));
                        string double_rot_state9 = fixLetterOrder(y_rotation(z3_rotation(cube_state)));
                        string double_rot_state10 = fixLetterOrder(y3_rotation(z_rotation(cube_state)));
                        string double_rot_state11 = fixLetterOrder(y3_rotation(z2_rotation(cube_state)));
                        string double_rot_state12 = fixLetterOrder(y3_rotation(z3_rotation(cube_state)));
                        string double_rot_state13 = fixLetterOrder(x_rotation(z_rotation(cube_state)));
                        string double_rot_state14 = fixLetterOrder(x_rotation(z2_rotation(cube_state)));
                        string double_rot_state15 = fixLetterOrder(x_rotation(z3_rotation(cube_state)));

                        // Apply mirror-symmetries
                        string m_state = fixLetterOrder(m_mirror(cube_state));
                        string s_state = fixLetterOrder(s_mirror(cube_state));
                        string e_state = fixLetterOrder(e_mirror(cube_state));

                        // apply double mirrors
                        string ms_state = fixLetterOrder(m_mirror(s_mirror(cube_state)));
                        string sm_state = fixLetterOrder(s_mirror(m_mirror(cube_state)));
                        string em_state = fixLetterOrder(e_mirror(m_mirror(cube_state)));
                        string me_state = fixLetterOrder(m_mirror(e_mirror(cube_state)));
                        string se_state = fixLetterOrder(s_mirror(e_mirror(cube_state)));
                        string es_state = fixLetterOrder(e_mirror(s_mirror(cube_state)));


                        // Check if any of these states already exist
                        if (seen_states.find(normalized_state) != seen_states.end()) {
                            eliminated_letter_order++;
                            cout << "Duplicate (Letter Order): " << scramble << " -> " << seen_states[normalized_state] << endl;
                            continue;
                        }
                        if (seen_states.find(y1_state) != seen_states.end() ||
                            seen_states.find(y2_state) != seen_states.end() ||
                            seen_states.find(y3_state) != seen_states.end()) {
                            eliminated_y_rotation++;
                            cout << "Duplicate (Y-Rotation): " << scramble << " -> " << seen_states[y1_state] << endl;
                            continue;
                        }
                        if (seen_states.find(x1_state) != seen_states.end() ||
                            seen_states.find(x2_state) != seen_states.end() ||
                            seen_states.find(x3_state) != seen_states.end()) {
                            eliminated_x_rotation++;
                            cout << "Duplicate (X-Rotation): " << scramble << " -> " << seen_states[x1_state] << endl;
                            continue;
                        }
                        if (seen_states.find(z1_state) != seen_states.end() ||
                            seen_states.find(z2_state) != seen_states.end() ||
                            seen_states.find(z3_state) != seen_states.end()) {
                            eliminated_z_rotation++;
                            cout << "Duplicate (Z-Rotation): " << scramble << " -> " << seen_states[z1_state] << endl;
                            continue;
                        }
                        if (seen_states.find(double_rot_state) != seen_states.end() ||
                            seen_states.find(double_rot_state2) != seen_states.end() ||
                            seen_states.find(double_rot_state3) != seen_states.end() ||
                            seen_states.find(double_rot_state4) != seen_states.end() ||
                            seen_states.find(double_rot_state5) != seen_states.end() ||
                            seen_states.find(double_rot_state6) != seen_states.end() ||
                            seen_states.find(double_rot_state7) != seen_states.end() ||
                            seen_states.find(double_rot_state8) != seen_states.end() ||
                            seen_states.find(double_rot_state9) != seen_states.end() ||
                            seen_states.find(double_rot_state10) != seen_states.end() ||
                            seen_states.find(double_rot_state11) != seen_states.end() ||
                            seen_states.find(double_rot_state12) != seen_states.end() ||
                            seen_states.find(double_rot_state13) != seen_states.end() ||
                            seen_states.find(double_rot_state14) != seen_states.end() ||
                            seen_states.find(double_rot_state15) != seen_states.end()) {
                            eliminated_double_rot++;
                            cout << "Duplicate (Double Rotation): " << scramble << " -> " << seen_states[double_rot_state] << endl;
                            continue;
                        }

                        if (seen_states.find(m_state) != seen_states.end()) {
                            eliminated_double_rot++;
                            cout << "Duplicate (M-Mirror): " << scramble << " -> " << seen_states[m_state] << endl;
                            continue;
                        }

                        if (seen_states.find(s_state) != seen_states.end()) {
                            eliminated_double_rot++;
                            cout << "Duplicate (S-Mirror): " << scramble << " -> " << seen_states[s_state] << endl;
                            continue;
                        }

                        if (seen_states.find(e_state) != seen_states.end()) {
                            eliminated_double_rot++;
                            cout << "Duplicate (E-Mirror): " << scramble << " -> " << seen_states[e_state] << endl;
                            continue;
                        }

                        if (seen_states.find(ms_state) != seen_states.end()) {
                            eliminated_double_rot++;
                            cout << "Duplicate (MS-Mirror): " << scramble << " -> " << seen_states[ms_state] << endl;
                            continue;
                        }

                        if (seen_states.find(sm_state) != seen_states.end()) {
                            eliminated_double_rot++;
                            cout << "Duplicate (SM-Mirror): " << scramble << " -> " << seen_states[sm_state] << endl;
                            continue;
                        }

                        if (seen_states.find(me_state) != seen_states.end()) {
                            eliminated_double_rot++;
                            cout << "Duplicate (ME-Mirror): " << scramble << " -> " << seen_states[me_state] << endl;
                            continue;
                        }

                        if (seen_states.find(em_state) != seen_states.end()) {
                            eliminated_double_rot++;
                            cout << "Duplicate (EM-Mirror): " << scramble << " -> " << seen_states[em_state] << endl;
                            continue;
                        }

                        if (seen_states.find(se_state) != seen_states.end()) {
                            eliminated_double_rot++;
                            cout << "Duplicate (SE-Mirror): " << scramble << " -> " << seen_states[se_state] << endl;
                            continue;
                        }

                        if (seen_states.find(es_state) != seen_states.end()) {
                            eliminated_double_rot++;
                            cout << "Duplicate (ES-Mirror): " << scramble << " -> " << seen_states[es_state] << endl;
                            continue;
                        }



                        // Save the new state and its scramble
                        seen_states[normalized_state] = scramble;
                        outputFile << scramble << " " << normalized_state << endl;
                    }
                }
            }
        }
    }

    outputFile.close();

    // Print final counts
    cout << "Scramble results saved to " << filename << endl;
    cout << "Eliminated due to Letter Order Normalization: " << eliminated_letter_order << endl;
    cout << "Eliminated due to Y-Rotations: " << eliminated_y_rotation << endl;
    cout << "Eliminated due to X-Rotations: " << eliminated_x_rotation << endl;
    cout << "Eliminated due to Z-Rotations: " << eliminated_z_rotation << endl;
    cout << "Eliminated due to Double Rotations: " << eliminated_double_rot << endl;
    cout << "Eliminated due to M-Mirror: " << eliminated_double_rot << endl;
    cout << "Eliminated due to S-Mirror: " << eliminated_double_rot << endl;
    cout << "Eliminated due to E-Mirror: " << eliminated_double_rot << endl;
    cout << "Eliminated due to MS-Mirror: " << eliminated_double_rot << endl;
    cout << "Eliminated due to SM-Mirror: " << eliminated_double_rot << endl;
    cout << "Eliminated due to ME-Mirror: " << eliminated_double_rot << endl;
    cout << "Eliminated due to EM-Mirror: " << eliminated_double_rot << endl;
    cout << "Eliminated due to SE-Mirror: " << eliminated_double_rot << endl;
    cout << "Eliminated due to ES-Mirror: " << eliminated_double_rot << endl;

}



// Main function
int main() {
    auto start_time = high_resolution_clock::now();

    int length = 24;
    int counter = 0;

    set<string> validCombinations;

    // Generate combinations
    unordered_set<string> forbiddenSequences;
    forbiddenSequences.insert("aaca");
    forbiddenSequences.insert("aacb");
    forbiddenSequences.insert("aacc");
    forbiddenSequences.insert("aaac"); // this list is not exhaustive, because in the generation-process, most forbidden 4-letter sequences are already avoided because of only allowing "a" as first and "a" and "b" as second letter
    //generateCombinations(length, validCombinations, counter, forbiddenSequences);

    // Write valid combinations to file
    //ofstream outputFile("prune.txt");

    //for (const string& combination : validCombinations) {
    //    outputFile << combination << endl;
    //}

    //outputFile.close();



    /////
    //cout << "Total Valid Combinations: " << counter << endl;
    //cout << endl << endl;
    //cout << "Type in your scramble" << endl;
    //string scramble;
    //getline(cin, scramble);
    //cout << "New Cubestate" << endl << translate_scramble_to_moves(scramble) << endl;
    /////

    vector<string> moves = {
        "R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'", 
        "U", "U2", "U'", "D", "D2", "D'", 
        "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'", 
        "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", 
        "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"
    };
    //string initial_state = "012345678901234567890123";  // Solved cube state
    string initial_state = "aaaabbbbccccbbbbccccaaaa";  // HTR-solved cube state
    // Lets go through  Fw Rw2 Fw' Dw' Lw'
    //cout << "Initial state: " << initial_state << endl;
    //cout << d3(initial_state) << endl;
    //cout << f(initial_state) << endl;
    //cout << r2(f(initial_state)) << endl;
    //cout << f3(r2(f(initial_state))) << endl;
    //cout << d3(f3(r2(f(initial_state)))) << endl;
    //cout << l3(d3(f3(r2(f(initial_state))))) << endl;
    //cout << "Final state: " << fixLetterOrder(l3(d3(f3(r2(f(initial_state)))))) << endl;
//
    generate_five_move_scrambles_unique_states(moves, initial_state, "all_five_move_scrambles_unique_states_buffer.txt");

    // Stop the timer
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    cout << "Execution time: " << duration.count() << " ms" << endl;
    return 0;
}
