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
#include <iomanip> 

#include <thread>
#include <mutex>
#include <vector>

using namespace std;
using namespace std::chrono; 

// Move application and rotation
string apply_move(const string& state, const string& move);

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

// functions for wings
string R_wings(const string&); string R2_wings(const string&); string R3_wings(const string&);
string L_wings(const string&); string L2_wings(const string&); string L3_wings(const string&);
string F_wings(const string&); string F2_wings(const string&); string F3_wings(const string&);
string B_wings(const string&); string B2_wings(const string&); string B3_wings(const string&);
string U_wings(const string&); string U2_wings(const string&); string U3_wings(const string&);
string D_wings(const string&); string D2_wings(const string&); string D3_wings(const string&);
string r_wings(const string&); string r2_wings(const string&); string r3_wings(const string&);
string l_wings(const string&); string l2_wings(const string&); string l3_wings(const string&);
string f_wings(const string&); string f2_wings(const string&); string f3_wings(const string&);
string b_wings(const string&); string b2_wings(const string&); string b3_wings(const string&);
string u_wings(const string&); string u2_wings(const string&); string u3_wings(const string&);
string d_wings(const string&); string d2_wings(const string&); string d3_wings(const string&);


string x_rotation(const string&);
string x2_rotation(const string&);
string x3_rotation(const string&);
string y_rotation(const string&);
string y2_rotation(const string&);
string y3_rotation(const string&);
string z_rotation(const string&);
string z2_rotation(const string&);
string z3_rotation(const string&);

string x_y_rotation(const string&);
string x_y2_rotation(const string&);
string x_y3_rotation(const string&);
string x3_y_rotation(const string&);
string x3_y2_rotation(const string&);
string x3_y3_rotation(const string&);
string y_x_rotation(const string&);
string y_x2_rotation(const string&);
string y_x3_rotation(const string&);
string y3_x_rotation(const string&);
string y3_x2_rotation(const string&);
string y3_x3_rotation(const string&);
string z_y2_rotation(const string&);
string y2_z_rotation(const string&);


// Mirror functions
string m_mirror(const string&);
string s_mirror(const string&);
string e_mirror(const string&);



// Letter normalization
string fixLetterOrder(const string&);

// Letter swap
string letter_swap(string, char, char);

ofstream log_file("log.txt", ios::out); // Log file for all printed outputs

const vector<string> MOVES = {
    "R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
    "U", "U2", "U'", "D", "D2", "D'", "Rw", "Rw2", "Rw'", "Lw", "Lw2", "Lw'",
    "Fw", "Fw2", "Fw'", "Bw", "Bw2", "Bw'", "Uw", "Uw2", "Uw'", "Dw", "Dw2", "Dw'"
};

const vector<string> MOVES_reduced = {
    "R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
    "U", "U2", "U'", "D", "D2", "D'", "Rw", "Rw2", "Rw'",
    "Fw", "Fw2", "Fw'", "Uw", "Uw2", "Uw'",
};

const vector<string> MOVES_post_HTRC = {
    "R", "R2", "R'", "L", "L2", "L'", "F", "F2", "F'", "B", "B2", "B'",
    "U", "U2", "U'", "D", "D2", "D'", "Rw2", "Fw2", "Uw2"
};

const vector<string> CUBE_ORIENTATIONS = {
    "", // identity (no rotation)
    "x", "x2", "x'",
    "y", "y2", "y'",
    "z", "z2", "z'",
    "x y", "x y2", "x y'",
    "x' y", "x' y2", "x' y'",
    "y x", "y x2", "y x'",
    "y' x", "y' x2", "y' x'",
    "z y2", "y2 z"
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
    if (move == "x y") return x_y_rotation(state);
    if (move == "x y2") return x_y2_rotation(state);
    if (move == "x y'") return x_y3_rotation(state);
    if (move == "x' y") return x3_y_rotation(state);
    if (move == "x' y2") return x3_y2_rotation(state);
    if (move == "x' y'") return x3_y3_rotation(state);
    if (move == "y x") return y_x_rotation(state);
    if (move == "y x2") return y_x2_rotation(state);
    if (move == "y x'") return y_x3_rotation(state);
    if (move == "y' x") return y3_x_rotation(state);
    if (move == "y' x2") return y3_x2_rotation(state);
    if (move == "y' x'") return y3_x3_rotation(state);
    if (move == "z y2") return z_y2_rotation(state);
    if (move == "y2 z") return y2_z_rotation(state);
    
    return state;  // Return unchanged state if move is not recognized
}

string apply_move_wings(const string& state, const string& move) {
    if (move == "R") return R_wings(state);
    if (move == "R2") return R2_wings(state);
    if (move == "R'") return R3_wings(state);
    if (move == "L") return L_wings(state);
    if (move == "L2") return L2_wings(state);
    if (move == "L'") return L3_wings(state);
    if (move == "F") return F_wings(state);
    if (move == "F2") return F2_wings(state);
    if (move == "F'") return F3_wings(state);
    if (move == "B") return B_wings(state);
    if (move == "B2") return B2_wings(state);
    if (move == "B'") return B3_wings(state);
    if (move == "U") return U_wings(state);
    if (move == "U2") return U2_wings(state);
    if (move == "U'") return U3_wings(state);
    if (move == "Uw") return u_wings(state);
    if (move == "Uw2") return u2_wings(state);
    if (move == "Uw'") return u3_wings(state);
    if (move == "D") return D_wings(state);
    if (move == "D2") return D2_wings(state);
    if (move == "D'") return D3_wings(state);
    if (move == "Dw") return d_wings(state);
    if (move == "Dw2") return d2_wings(state);
    if (move == "Dw'") return d3_wings(state);
    if (move == "Rw") return r_wings(state);
    if (move == "Rw2") return r2_wings(state);
    if (move == "Rw'") return r3_wings(state);
    if (move == "Lw") return l_wings(state);
    if (move == "Lw2") return l2_wings(state);
    if (move == "Lw'") return l3_wings(state);
    if (move == "Fw") return f_wings(state);
    if (move == "Fw2") return f2_wings(state);
    if (move == "Fw'") return f3_wings(state);
    if (move == "Bw") return b_wings(state);
    if (move == "Bw2") return b2_wings(state);
    if (move == "Bw'") return b3_wings(state);

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

string translate_scramble_to_moves_precise(const string& scramble) {
    stringstream ss(scramble);
    string move;
    string current_state = "aaaabbbbccccddddeeeeffff";  // HTR-solved cube state

    while (ss >> move) {
        current_state = apply_move(current_state, move);
    }
    return current_state;
}

string translate_scramble_to_moves_wings(const string& scramble) {
    stringstream ss(scramble);
    string move;
    string current_state = "aabbccddeejjnnrruuvvwwzz";  // edge-paired-states

    while (ss >> move) {
        current_state = apply_move_wings(current_state, move);
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
    cycle(combination_new[0], combination_new[1], combination_new[2], combination_new[3]);      // Cycle 0 -> 1 -> 2 -> 3 -> 0
    cycle(combination_new[4], combination_new[16], combination_new[12], combination_new[8]);    // Cycle 4 -> 16 -> 12 -> 8 -> 4
    cycle(combination_new[5], combination_new[17], combination_new[13], combination_new[9]);    // Cycle 5 -> 17 -> 13 -> 9 -> 5
    cycle(combination_new[6], combination_new[18], combination_new[14], combination_new[10]);   // Cycle 6 -> 18 -> 14 -> 10 -> 6
    cycle(combination_new[7], combination_new[19], combination_new[15], combination_new[11]);   // Cycle 7 -> 19 -> 15 -> 11 -> 7
    cycle(combination_new[20], combination_new[23], combination_new[22], combination_new[21]);  // Cycle 20 -> 23 -> 22 -> 21 -> 20
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

string x_y_rotation(const string& combination) { return y_rotation(x_rotation(combination));}
string x_y2_rotation(const string& combination) { return y2_rotation(x_rotation(combination));}
string x_y3_rotation(const string& combination) { return y3_rotation(x_rotation(combination));}
string x3_y_rotation(const string& combination) { return y_rotation(x3_rotation(combination));}
string x3_y2_rotation(const string& combination) { return y2_rotation(x3_rotation(combination));}
string x3_y3_rotation(const string& combination) { return y3_rotation(x3_rotation(combination));}
string y_x_rotation(const string& combination) { return x_rotation(y_rotation(combination));}
string y_x2_rotation(const string& combination) { return x2_rotation(y_rotation(combination));}
string y_x3_rotation(const string& combination) { return x3_rotation(y_rotation(combination));}
string y3_x_rotation(const string& combination) { return x_rotation(y3_rotation(combination));}
string y3_x2_rotation(const string& combination) { return x2_rotation(y3_rotation(combination));}
string y3_x3_rotation(const string& combination) { return x3_rotation(y3_rotation(combination));}
string z_y2_rotation(const string& combination) {return y2_rotation(z_rotation(combination));}
string y2_z_rotation(const string& combination) { return z_rotation(y2_rotation(combination));}

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

string R2_wings(const string& combination) {string combination_new = R_wings(combination);  return R_wings(combination_new);}
string R3_wings(const string& combination) {return R_wings(R2_wings(combination));}


string R_wings(const string& combination) {
    string combination_new = combination; 
    cycle(combination_new[3], combination_new[14], combination_new[19], combination_new[11]);
    cycle(combination_new[2], combination_new[15], combination_new[18], combination_new[10]);
    return combination_new;
}

//note that uncapitalized r is not a slice move, but equivalent to Rw in some literature. This applies to all uncapiatalized moves

string r(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[12], combination_new[13], combination_new[14], combination_new[15]);
    cycle(combination_new[1], combination_new[19], combination_new[21], combination_new[9]);
    cycle(combination_new[2], combination_new[16], combination_new[22], combination_new[10]);
    return combination_new;
}

string r_wings(const string& combination) {
    string combination_new = R_wings(combination);
    cycle(combination_new[4], combination_new[1], combination_new[20], combination_new[17]);
    return combination_new;
}


string r2(const string& combination) {string combination_new = r(combination); return r(combination_new);}
string r3(const string& combination) {return r(r2(combination));}

string r2_wings(const string& combination) {string combination_new = r_wings(combination); return r_wings(combination_new);}
string r3_wings(const string& combination) {return r_wings(r2_wings(combination));}


string L(const string& combination) {
    string combination_new = combination;     // Cycle 4 -> 5 -> 6 -> 7 -> 4
    cycle(combination_new[4], combination_new[5], combination_new[6], combination_new[7]);
    return combination_new;
}

string L_wings(const string& combination) {
    string combination_new = combination;    
    cycle(combination_new[7], combination_new[8], combination_new[23], combination_new[13]);
    cycle(combination_new[6], combination_new[9], combination_new[22], combination_new[12]);
    return combination_new;
}

string L2(const string& combination) {string combination_new = L(combination);  return L(combination_new);          }
string L3(const string& combination) {return L(L2(combination));}

string L2_wings(const string& combination) {string combination_new = L_wings(combination);  return L_wings(combination_new);}
string L3_wings(const string& combination) {return L_wings(L2_wings(combination));}


string l(const string& combination) {
    string combination_new = r(combination);
    return x3_rotation(combination_new);           
}

string l_wings(const string& combination) {
    string combination_new = L_wings(combination);
    cycle(combination_new[0], combination_new[5], combination_new[16], combination_new[21]);
    return combination_new;
}

string l2(const string& combination) {
    string combination_new = r2(combination);
    return x2_rotation(combination_new);           
}

string l3(const string& combination) {
    string combination_new = r3(combination);
    return x_rotation(combination_new);           
}

string l2_wings(const string& combination) {string combination_new = l_wings(combination);  return l_wings(combination_new);}
string l3_wings(const string& combination) {return l_wings(l2_wings(combination));}


string F(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[8], combination_new[9], combination_new[10], combination_new[11]);    // Cycle 8 -> 9 -> 10 -> 11 -> 8
    return combination_new;
}

string F_wings(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[5], combination_new[10], combination_new[17], combination_new[9]);   
    cycle(combination_new[4], combination_new[11], combination_new[16], combination_new[8]);
    return combination_new;
}

string F2(const string& combination) {
    string combination_new = F(combination);
    return F(combination_new);               
}

string F3(const string& combination) { 
    return F(F2(combination));
}

string F2_wings(const string& combination) {string combination_new = F_wings(combination); return F_wings(combination_new);}
string F3_wings(const string& combination) {return F_wings(F2_wings(combination));}

string f(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[2], combination_new[15], combination_new[20], combination_new[5]);    // Cycle 2 -> 15 -> 20 -> 5 -> 2
    cycle(combination_new[3], combination_new[12], combination_new[21], combination_new[6]);    // Cycle 3 -> 12 -> 21 -> 6 -> 3  
    cycle(combination_new[8], combination_new[9], combination_new[10], combination_new[11]);    // Cycle 8 -> 9 -> 10 -> 11 -> 8
    return combination_new;
}

string f_wings(const string& combination) {
    string combination_new = F_wings(combination);
    cycle(combination_new[6], combination_new[3], combination_new[18], combination_new[23]);
    return combination_new;
}
        
string f2(const string& combination) {string combination_new = f(combination); return f(combination_new);}
string f3(const string& combination) {return f(f2(combination));}
string f2_wings(const string& combination) {string combination_new = f_wings(combination); return f_wings(combination_new);}
string f3_wings(const string& combination) {return f_wings(f2_wings(combination));}

string B(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[16], combination_new[17], combination_new[18], combination_new[19]);     // Cycle 16 -> 17 -> 18 -> 19 -> 16
    return combination_new;
}

string B_wings(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[1], combination_new[12], combination_new[21], combination_new[15]);
    cycle(combination_new[0], combination_new[13], combination_new[20], combination_new[14]);
    return combination_new;
}

string B2(const string& combination) {
    string combination_new = B(combination); 
    return B(combination_new);               
}

string B3(const string& combination) { 
    return B(B2(combination));
}

string B2_wings(const string& combination) {string combination_new = B_wings(combination); return B_wings(combination_new);}
string B3_wings(const string& combination) {return B_wings(B2_wings(combination));}

string b(const string& combination) {
    string combination_new = f(combination);
    return z3_rotation(combination_new);           
}

string b_wings(const string& combination) {
    string combination_new = B_wings(combination);
    cycle(combination_new[2], combination_new[7], combination_new[22], combination_new[19]);
    return combination_new;
}

string b2(const string& combination) {
    string combination_new = f2(combination);
    return z2_rotation(combination_new);           
}

string b3(const string& combination) {
    string combination_new = f3(combination);
    return z_rotation(combination_new);           
}

string b2_wings(const string& combination) {string combination_new = b_wings(combination); return b_wings(combination_new);}
string b3_wings(const string& combination) {return b_wings(b2_wings(combination));}

string U(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[0], combination_new[1], combination_new[2], combination_new[3]);     // Cycle 0 -> 1 -> 2 -> 3 -> 0
    return combination_new;
}

string U_wings(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[0], combination_new[2], combination_new[4], combination_new[6]);     
    cycle(combination_new[1], combination_new[3], combination_new[5], combination_new[7]);  
    return combination_new;
}


string U2(const string& combination) {
    string combination_new = U(combination); 
    return U(combination_new);               
}

string U3(const string& combination) {
    return U(U2(combination));
}

string U2_wings(const string& combination) {string combination_new = U_wings(combination); return U_wings(combination_new);}
string U3_wings(const string& combination) {return U_wings(U2_wings(combination));}

string u(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[0], combination_new[1], combination_new[2], combination_new[3]);     // Cycle 0 -> 1 -> 2 -> 3 -> 0
    cycle(combination_new[4], combination_new[16], combination_new[12], combination_new[8]);    // Cycle 4 -> 16 -> 12 -> 8 -> 4
    cycle(combination_new[5], combination_new[17], combination_new[13], combination_new[9]);    // Cycle 5 -> 17 -> 13 -> 9 -> 5
    return combination_new;
}


string u_wings(const string& combination) {
    string combination_new = U_wings(combination); 
    cycle(combination_new[8], combination_new[12], combination_new[14], combination_new[10]);
    return combination_new;
}


string u2(const string& combination) {
    string combination_new = u(combination);  // First transformation
    return u(combination_new);               // Apply u again and return
}

string u3(const string& combination) {
    return u(u2(combination));
}

string u2_wings(const string& combination) {string combination_new = u_wings(combination); return u_wings(combination_new);}
string u3_wings(const string& combination) {return u_wings(u2_wings(combination));}

string D(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[20], combination_new[21], combination_new[22], combination_new[23]);     // Cycle 20 -> 21 -> 22 -> 23 -> 20
    return combination_new;
}

string D_wings(const string& combination) {
    string combination_new = combination;
    cycle(combination_new[17], combination_new[19], combination_new[21], combination_new[23]);   
    cycle(combination_new[16], combination_new[18], combination_new[20], combination_new[22]);
    return combination_new;
}

string D2(const string& combination) {
    string combination_new = D(combination);  // First transformation
    return D(combination_new);               // Apply D again and return
}

string D3(const string& combination) { // Perform D3 (D applied three times)
    return D(D2(combination));
}

string D2_wings(const string& combination) {string combination_new = D_wings(combination); return D_wings(combination_new);}
string D3_wings(const string& combination) {return D_wings(D2_wings(combination));}

string d(const string& combination) {
    string combination_new = u(combination);
    return y3_rotation(combination_new);           
}

string d_wings(const string& combination) {
    string combination_new = D_wings(combination);
    cycle(combination_new[9], combination_new[11], combination_new[15], combination_new[13]);
    return combination_new;
}

string d2(const string& combination) {
    string combination_new = u2(combination);
    return y2_rotation(combination_new);           
}

string d3(const string& combination) {
    string combination_new = u3(combination);
    return y_rotation(combination_new);           
}

string d2_wings(const string& combination) {string combination_new = d_wings(combination); return d_wings(combination_new);}
string d3_wings(const string& combination) {return d_wings(d2_wings(combination));}



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

void explorer() {
    const int MAX_DEPTH = 10;
    vector<vector<string>> depths(MAX_DEPTH + 1);
    vector<unordered_set<string>> depth_sets(MAX_DEPTH + 1);

    string initial_state = "aaaabbbbccccbbbbccccaaaa";
    depths[0].push_back(initial_state);
    depth_sets[0].insert(initial_state);

    for (int d = 0; d < MAX_DEPTH; ++d) {
        //log_message("\n\nApplying all possible moves to the depth " + to_string(d) + " states:");
        for (const string& state : depths[d]) {
            //log_message("  ⬇ Applying moves to depth-" + to_string(d) + " state: " + state);

            for (const string& move : MOVES_reduced) {
                string new_state = apply_move(state, move);

                vector<string> rotated_states;
                rotated_states.push_back(fixLetterOrder(new_state)); // include case of no rotation

                for (const string& rotation : CUBE_ORIENTATIONS) {
                    string rotated_state = fixLetterOrder(apply_move(new_state, rotation));
                    rotated_states.push_back(rotated_state);
                }
                string canonical_state = *min_element(rotated_states.begin(), rotated_states.end());

                bool seen = false;
                for (int prev_d = 0; prev_d <= d + 1 && prev_d <= MAX_DEPTH; ++prev_d) {
                    if (depth_sets[prev_d].find(canonical_state) != depth_sets[prev_d].end()) {
                        seen = true;
                        break;
                    }
                }
                if (!seen && d + 1 <= MAX_DEPTH) {
                    depth_sets[d + 1].insert(canonical_state);
                    depths[d + 1].push_back(canonical_state);
                }

                stringstream output_line;
                //output_line << "    ├─ Move: " << std::setw(4) << std::left << move << " => State: " << new_state << " | Canonical: " << canonical_state;
                //log_message(output_line.str());
            }
        }
    }
        // 📝 Write each depth to a separate file
        for (int d = 0; d <= MAX_DEPTH; ++d) {
            string filename = "depth_" + to_string(d) + ".txt";
            ofstream outfile(filename);
            for (const auto& state : depths[d]) {
                outfile << state << '\n';
            }
            outfile.close();
            log_message("📁 Saved " + to_string(depths[d].size()) + " states to " + filename);
        }
    

    log_message("\n==================== STATE COUNTS ====================");
    for (int d = 0; d <= MAX_DEPTH; ++d) {
        log_message("Depth " + to_string(d) + ": " + to_string(depths[d].size()) + " states");
    }
}


pair<string, string> get_canonical_form_with_rotation(const string& state) {
    string canonical = fixLetterOrder(state);
    string best_rotation = "";  // empty means identity (no rotation)

    for (const string& rotation : CUBE_ORIENTATIONS) {
        string rotated_state = apply_move(state, rotation);
        string fixed = fixLetterOrder(rotated_state);

        if (fixed < canonical) {
            canonical = fixed;
            best_rotation = rotation;
        }
    }

    return {canonical, best_rotation};
}


void generate_next_depth_from_file( // for the case that you have a file with states and want to generate the next depth
    const string& previous_depth_file,
    const string& current_depth_file,
    const string& next_depth_file
) {
    unordered_set<string> known_states;
    vector<string> current_states;

    // Load previous depth states
    ifstream infile_prev(previous_depth_file);
    if (!infile_prev) {
        log_message("❌ Failed to open " + previous_depth_file);
        return;
    }
    string line;
    while (getline(infile_prev, line)) {
        known_states.insert(line);
    }
    infile_prev.close();

    // Load current depth states
    ifstream infile_current(current_depth_file);
    if (!infile_current) {
        log_message("❌ Failed to open " + current_depth_file);
        return;
    }
    while (getline(infile_current, line)) {
        current_states.push_back(line);
        known_states.insert(line); // also avoid revisiting current states
    }
    infile_current.close();

    unordered_set<string> next_depth_set;
    vector<string> next_depth_states;

    int processed_count = 0;
    for (const string& state : current_states) {
        ++processed_count;
        if (processed_count % 10000 == 0) {
            log_message("⏳ Processed " + to_string(processed_count) + " / " + to_string(current_states.size()) + " states...");
        }
    
        
        for (const string& move : MOVES_reduced) {
            string new_state = apply_move(state, move);

            vector<string> rotated_states;
            rotated_states.push_back(fixLetterOrder(new_state)); // no rotation

            for (const string& rotation : CUBE_ORIENTATIONS) {
                string rotated_state = fixLetterOrder(apply_move(new_state, rotation));
                rotated_states.push_back(rotated_state);
            }

            string canonical_state = *min_element(rotated_states.begin(), rotated_states.end());

            if (known_states.find(canonical_state) == known_states.end()) {
                known_states.insert(canonical_state); // track to avoid duplicates
                next_depth_set.insert(canonical_state);
                next_depth_states.push_back(canonical_state);
            }
        }
    }
    log_message("✅ Processed " + to_string(processed_count) + " states from " + current_depth_file);


    ofstream outfile(next_depth_file);
    if (!outfile) {
        log_message("❌ Failed to open " + next_depth_file);
        return;
    }
    for (const auto& state : next_depth_states) {
        outfile << state << '\n';
    }
    outfile.close();

    log_message("📁 Saved " + to_string(next_depth_states.size()) + " states to " + next_depth_file);
}

int find_depth_of_state(const string& canonical_state) {
    for (int d = 0; d <= 11; ++d) {
        string filename = "depth_" + to_string(d) + ".txt";
        ifstream infile(filename);
        if (!infile.is_open()) continue;

        string line;
        while (getline(infile, line)) {
            if (line == canonical_state) {
                return d;
            }
        }
    }
    return -1; // Not found
}

void solver_txt(const string& scramble) {
    log_message("solving ... " + scramble);

    string raw_state = translate_scramble_to_moves(scramble);
    auto [canonical, rotation_used] = get_canonical_form_with_rotation(raw_state);
    log_message("Starting canonical state: " + canonical + " (rotation: " + rotation_used + ")");

    int current_depth = find_depth_of_state(canonical);
    if (current_depth == -1) {
        log_message("❌ Starting state not found in depth files.");
        return;
    }

    log_message("Starting at depth: " + to_string(current_depth));

    string current_state = canonical;
    vector<string> solution_path = {canonical};

    while (current_depth > 0) {
        bool found = false;
        for (const string& move : MOVES) {
            string moved = apply_move(current_state, move);
            auto [canon_next, rot] = get_canonical_form_with_rotation(moved);
            int depth_next = find_depth_of_state(canon_next);
            if (depth_next != -1 && depth_next < current_depth) {
                log_message("  ➤ Move: " + move + ", Rotation: " + rot + ", Depth: " + to_string(depth_next));
                current_state = canon_next;
                solution_path.push_back(canon_next);
                current_depth = depth_next;
                found = true;
                break;
            }
        }
        if (!found) {
            log_message("❌ Could not find a move to lower depth.");
            return;
        }
    }

    log_message("✅ Full solving path:");
    for (const string& step : solution_path) {
        log_message("  ➤ " + step);
    }
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

// a function to test a multinomial rank of a specific state:
void test_multinomial_rank(const string& state) {
    map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};
    __uint128_t rank = multinomial_rank(state, letter_counts);
    cout << "Multinomial rank of " << state << ": " << uint128_to_string(rank) << endl;
}





void generate_blank_bin_file(const string& filename, size_t num_entries = 2100000000) {
    ofstream outfile(filename, ios::binary | ios::trunc);
    const size_t CHUNK_SIZE = 1000000; // write in 1 million-byte chunks
    vector<uint8_t> ones_chunk(CHUNK_SIZE, 0xFF); // Each byte = 0b11111111
    size_t total_bytes = num_entries / 2; // 4 bits = 0.5 byte per entry
    size_t full_chunks = total_bytes / CHUNK_SIZE;

    cout << "🔧 Generating binary file of size " << total_bytes << " bytes (" << num_entries << " entries at 4 bits each)..." << endl;

    for (size_t i = 0; i < full_chunks; ++i) {
        outfile.write(reinterpret_cast<const char*>(ones_chunk.data()), CHUNK_SIZE);
    }
    outfile.close();
    cout << "✅ Blank binary file generated: " << filename << endl;
}


void bin_file_preview(const string& filename, size_t num_entries_to_show = 64) {
    ifstream infile(filename, ios::binary);
    if (!infile.is_open()) {
        cerr << "❌ Could not open file: " << filename << endl;
        return;
    }

    size_t total_bytes = (num_entries_to_show + 1) / 2; // 2 entries per byte
    vector<uint8_t> buffer(total_bytes);

    infile.read(reinterpret_cast<char*>(buffer.data()), total_bytes);
    infile.close();

    cout << "\n🔍 Previewing first " << num_entries_to_show << " 4-bit flags from: " << filename << endl;
    cout << "═══════════════════════════════════════════════════════════════" << endl;
    cout << std::hex << std::uppercase;

    for (size_t i = 0; i < num_entries_to_show; ++i) {
        uint8_t byte = buffer[i / 2];
        uint8_t flag = (i % 2 == 0) ? (byte >> 4) : (byte & 0x0F);

        cout << " " << setw(1) << (int)flag;
        if ((i + 1) % 16 == 0) cout << endl; // 16 flags per row
    }

    cout << "\n═══════════════════════════════════════════════════════════════" << endl;
    cout << std::dec; // Reset to decimal output
}



void flag_multinomial_ranks_in_4bit_bin_file(
    const string& input_txt,
    const string& binary_filename,
    uint8_t depth_flag  // must be ≤ 15 (4-bit)
) {
    ifstream infile(input_txt);
    if (!infile.is_open()) {
        cerr << "❌ Could not open input file: " << input_txt << endl;
        return;
    }

    fstream binfile(binary_filename, ios::in | ios::out | ios::binary);
    if (!binfile.is_open()) {
        cerr << "❌ Could not open binary file: " << binary_filename << endl;
        return;
    }

    const map<char, int> letter_counts = {{'a', 7}, {'b', 8}, {'c', 8}};
    string line;
    size_t updated = 0;
    size_t line_num = 0;

    while (getline(infile, line)) {
        ++line_num;

        if (line.length() != 24 || line[0] != 'a') {
            cerr << "⚠️ Skipping invalid line " << line_num << ": " << line << endl;
            continue;
        }

        string trimmed = line.substr(1);
        __uint128_t rank = multinomial_rank(trimmed, letter_counts);
        size_t index = static_cast<size_t>(rank);
        size_t byte_offset = index / 2;
        bool is_high_nibble = (index % 2 == 0);

        // Read the byte containing our 4-bit slot
        binfile.seekg(byte_offset);
        uint8_t byte;
        binfile.read(reinterpret_cast<char*>(&byte), 1);

        if (binfile.eof()) {
            cerr << "⚠️ Index out of bounds: " << index << endl;
            break;
        }

        uint8_t current_flag = is_high_nibble ? (byte >> 4) & 0xF : byte & 0xF;

        if (current_flag == 15) {
            // Set the new flag
            if (is_high_nibble) {
                byte = (depth_flag << 4) | (byte & 0x0F);
            } else {
                byte = (byte & 0xF0) | (depth_flag & 0x0F);
            }

            // Write it back
            binfile.seekp(byte_offset);
            binfile.write(reinterpret_cast<const char*>(&byte), 1);
            ++updated;
        }
    }

    infile.close();
    binfile.close();

    cout << "✅ Flagged " << updated << " entries in " << binary_filename
         << " for depth " << static_cast<int>(depth_flag) << endl;
}


unordered_map<string, unordered_map<char, char>> rotation_maps = {
    {"x",   {{'U','F'}, {'F','D'}, {'D','B'}, {'B','U'}, {'L','L'}, {'R','R'}, {'y','z'}, {'x','x'}, {'z','y'}}},
    {"x'",  {{'U','B'}, {'B','D'}, {'D','F'}, {'F','U'}, {'L','L'}, {'R','R'}, {'y','z'}, {'x','x'}, {'z','y'}}},
    {"x2",  {{'U','D'}, {'D','U'}, {'F','B'}, {'B','F'}, {'L','L'}, {'R','R'}, {'y','z'}, {'x','x'}, {'z','y'}}},
    {"y",   {{'F','R'}, {'R','B'}, {'B','L'}, {'L','F'}, {'U','U'}, {'D','D'}, {'y','y'}, {'x','z'}, {'z','x'}}},
    {"y'",  {{'F','L'}, {'L','B'}, {'B','R'}, {'R','F'}, {'U','U'}, {'D','D'}, {'y','y'}, {'x','z'}, {'z','x'}}},
    {"y2",  {{'F','B'}, {'B','F'}, {'L','R'}, {'R','L'}, {'U','U'}, {'D','D'}, {'y','y'}, {'x','z'}, {'z','x'}}},
    {"z",   {{'U','L'}, {'L','D'}, {'D','R'}, {'R','U'}, {'F','F'}, {'B','B'}, {'y','x'}, {'x','y'}, {'z','z'}}},
    {"z'",  {{'U','R'}, {'R','D'}, {'D','L'}, {'L','U'}, {'F','F'}, {'B','B'}, {'y','x'}, {'x','y'}, {'z','z'}}},
    {"z2",  {{'U','D'}, {'D','U'}, {'L','R'}, {'R','L'}, {'F','F'}, {'B','B'}, {'y','x'}, {'x','y'}, {'z','z'}}}
};

bool is_modifier(char c) {
    return c == '\'' || c == '2';
}

bool is_rotation_token(const string& token) {
    return rotation_maps.count(token) > 0;
}

// Split the scramble string into tokens (e.g., "R", "L'", "z2")
vector<string> tokenize_scramble(const string& scramble) {
    istringstream ss(scramble);
    vector<string> tokens;
    string token;
    while (ss >> token) tokens.push_back(token);
    return tokens;
}

// Apply a single rotation to a move face
char apply_single_rotation(char face, const string& rotation) {
    if (rotation_maps.count(rotation) && rotation_maps.at(rotation).count(face))
        return rotation_maps.at(rotation).at(face);
    return face;
}

// Apply a rotation to all moves AFTER that rotation in-place
void apply_rotation_to_suffix(vector<string>& tokens, size_t rotation_index, const string& rotation) {
    for (size_t i = rotation_index + 1; i < tokens.size(); ++i) {
        string& move = tokens[i];
        if (is_rotation_token(move)) continue;

        char face = move[0];
        string modifier = move.substr(1);
        char rotated = apply_single_rotation(face, rotation);
        tokens[i] = string(1, rotated) + modifier;
    }
}

string cleanup(const string& scramble) {
    vector<string> scramble_as_array = tokenize_scramble(scramble);

    // Process from back to front
    for (int i = static_cast<int>(scramble_as_array.size()) - 1; i >= 0; --i) {
        const string& token = scramble_as_array[i];
        if (is_rotation_token(token)) {
            apply_rotation_to_suffix(scramble_as_array, i, token);
        }
    }

    // Construct cleaned scramble
    stringstream cleaned;
    for (const string& t : scramble_as_array) {
        if (!is_rotation_token(t)) {
            cleaned << t << " ";
        }
    }

    return cleaned.str();
}

string HTRC_subset(const string& scramble) {
    stringstream ss(scramble);
    string move;
    string state = "aaaabbbbccccddddeeeeffff";

    while (ss >> move) {
        state = apply_move(state, move);
    }

    string first = "x", second = "x", third = "x"; // default to placeholder

    // First face: indices 0–3 vs 20–23
    if (state.substr(0, 4) == string(4, state[0])) {
        first = "a";
    } else if (((state[0] == state[1] && state[2] == state[3] && state[0] != state[2]) ||
                (state[0] == state[3] && state[1] == state[2] && state[0] != state[1])) &&
               ((state[20] == state[21] && state[22] == state[23] && state[20] != state[22]) ||
                (state[20] == state[23] && state[21] == state[22] && state[20] != state[21]))) {
        first = "b";
    } else if ((state[0] != state[1] && state[1] != state[2] && state[2] != state[3] && state[3] != state[0]) &&
               (state[20] != state[21] && state[21] != state[22] && state[22] != state[23] && state[23] != state[20])) {
        first = "c";
    } else if (((state[0] == state[1] && state[2] == state[3] && state[0] != state[2]) ||
                (state[0] == state[3] && state[1] == state[2] && state[0] != state[1])) &&
               (state[20] != state[21] && state[21] != state[22] && state[22] != state[23] && state[23] != state[20])) {
        first = "d";
    } else if ((state[0] != state[1] && state[1] != state[2] && state[2] != state[3] && state[3] != state[0]) &&
               ((state[20] == state[21] && state[22] == state[23] && state[20] != state[22]) ||
                (state[20] == state[23] && state[21] == state[22] && state[20] != state[21]))) {
        first = "d";
    } else if ((state[0] == state[1] && state[1] == state[2] && state[2] != state[3] && state[3] != state[0]) ||
               (state[0] != state[1] && state[1] == state[2] && state[2] == state[3] && state[3] != state[0]) ||
               (state[0] != state[1] && state[1] != state[2] && state[2] == state[3] && state[3] == state[0]) ||
               (state[0] == state[1] && state[1] != state[2] && state[2] != state[3] && state[3] == state[0])) {
        first = "e";
    }

    // Second face: indices 4–7 vs 12–15
    if (state.substr(4, 4) == string(4, state[4])) {
        second = "a";
    } else if (((state[4] == state[5] && state[6] == state[7] && state[4] != state[6]) ||
                (state[4] == state[7] && state[5] == state[6] && state[4] != state[5])) &&
               ((state[12] == state[13] && state[14] == state[15] && state[12] != state[14]) ||
                (state[12] == state[15] && state[13] == state[14] && state[12] != state[13]))) {
        second = "b";
    } else if ((state[4] != state[5] && state[5] != state[6] && state[6] != state[7] && state[7] != state[4]) &&
               (state[12] != state[13] && state[13] != state[14] && state[14] != state[15] && state[15] != state[12])) {
        second = "c";
    } else if (((state[4] == state[5] && state[6] == state[7] && state[4] != state[6]) ||
                (state[4] == state[7] && state[5] == state[6] && state[4] != state[5])) &&
               (state[12] != state[13] && state[13] != state[14] && state[14] != state[15] && state[15] != state[12])) {
        second = "d";
    } else if ((state[4] != state[5] && state[5] != state[6] && state[6] != state[7] && state[7] != state[4]) &&
               ((state[12] == state[13] && state[14] == state[15] && state[12] != state[14]) ||
                (state[12] == state[15] && state[13] == state[14] && state[12] != state[13]))) {
        second = "d";
    } else if ((state[4] == state[5] && state[5] == state[6] && state[6] != state[7] && state[7] != state[4]) ||
               (state[4] != state[5] && state[5] == state[6] && state[6] == state[7] && state[7] != state[4]) ||
               (state[4] != state[5] && state[5] != state[6] && state[6] == state[7] && state[7] == state[4]) ||
               (state[4] == state[5] && state[5] != state[6] && state[6] != state[7] && state[7] == state[4])) {
        second = "e";
    }

    // Third face: indices 8–11 vs 16–19
    if (state.substr(8, 4) == string(4, state[8])) {
        third = "a";
    }

    else if (((state[8] == state[9] && state[10] == state[11] && state[8] != state[10]) ||
                (state[8] == state[11] && state[9] == state[10] && state[8] != state[9])) &&
               ((state[16] == state[17] && state[18] == state[19] && state[16] != state[18]) ||
                (state[16] == state[19] && state[17] == state[18] && state[16] != state[17]))) {
        third = "b";
    } else if ((state[8] != state[9] && state[9] != state[10] && state[10] != state[11] && state[11] != state[8]) &&
               (state[16] != state[17] && state[17] != state[18] && state[18] != state[19] && state[19] != state[16])) {
        third = "c";
    } else if (((state[8] == state[9] && state[10] == state[11] && state[8] != state[10]) ||
                (state[8] == state[11] && state[9] == state[10] && state[8] != state[9])) &&
               (state[16] != state[17] && state[17] != state[18] && state[18] != state[19] && state[19] != state[16])) {
        third = "d";

    } else if ((state[8] != state[9] && state[9] != state[10] && state[10] != state[11] && state[11] != state[8]) &&
                ((state[16] == state[17] && state[18] == state[19] && state[16] != state[18]) ||
                (state[16] == state[19] && state[17] == state[18] && state[16] != state[17]))) {
        third = "d";

    } else if ((state[8] == state[9] && state[9] == state[10] && state[10] != state[11] && state[11] != state[8]) ||
               (state[8] != state[9] && state[9] == state[10] && state[10] == state[11] && state[11] != state[8]) ||
               (state[8] != state[9] && state[9] != state[10] && state[10] == state[11] && state[11] == state[8]) ||
               (state[8] == state[9] && state[9] != state[10] && state[10] != state[11] && state[11] == state[8])) {
        third = "e";
    }

    string subset = first + second + third;
    sort(subset.begin(), subset.end());

    //cout << "Final scrambled state: " << state << endl;
    //cout << "HTRC-subset: " << subset << endl;

    return subset;
}



void solver_bin(const string& scramble, const string& bin_filename = "HTR_C.bin") {
    log_message("🧩 Solving scramble using binary file: " + scramble);

    string scrambled_state = translate_scramble_to_moves(scramble);
    auto [canonical, rotation_used] = get_canonical_form_with_rotation(scrambled_state);
    string trimmed = canonical.substr(1);  // Remove leading 'a'

    __uint128_t rank = multinomial_rank(trimmed, {{'a', 7}, {'b', 8}, {'c', 8}});
    size_t index = static_cast<size_t>(rank);
    size_t byte_offset = index / 2;
    bool is_high_nibble = (index % 2 == 0);

    ifstream binfile(bin_filename, ios::binary);
    if (!binfile.is_open()) {
        log_message("❌ Failed to open binary file.");
        return;
    }

    binfile.seekg(byte_offset);
    uint8_t byte;
    binfile.read(reinterpret_cast<char*>(&byte), 1);
    binfile.close();

    uint8_t current_flag = is_high_nibble ? (byte >> 4) & 0xF : byte & 0xF;
    int current_depth = static_cast<int>(current_flag);

    log_message("📍 Starting canonical state: " + canonical + " (depth = " + to_string(current_depth) + ", rotation: " + rotation_used + ")");

    if (current_depth == 15) {
        log_message("❌ State not found (flag = 15). Possibly unsolved or not yet flagged.");
        return;
    }

    // Tracking the solution path
    vector<string> path = {canonical};
    vector<string> rotations = {rotation_used};
    vector<string> moves;
    int start_depth = current_depth;

    while (current_depth > 0) {
        bool moved_down = false;

        for (const string& move : MOVES_reduced) {
            string moved = apply_move(path.back(), move);
            auto [canon_next, rot] = get_canonical_form_with_rotation(moved);
            string trimmed_next = canon_next.substr(1);
            __uint128_t rank_next = multinomial_rank(trimmed_next, {{'a', 7}, {'b', 8}, {'c', 8}});
            size_t index_next = static_cast<size_t>(rank_next);
            size_t offset_next = index_next / 2;
            bool is_high_next = (index_next % 2 == 0);

            ifstream binfile2(bin_filename, ios::binary);
            binfile2.seekg(offset_next);
            uint8_t byte_next;
            binfile2.read(reinterpret_cast<char*>(&byte_next), 1);
            binfile2.close();

            uint8_t flag_next = is_high_next ? (byte_next >> 4) & 0xF : byte_next & 0xF;

            if (flag_next < current_depth) {
                path.push_back(canon_next);
                rotations.push_back(rot);
                moves.push_back(move);
                current_depth = flag_next;
                moved_down = true;
                break;
            }
        }

        if (!moved_down) {
            log_message("❌ No move found that lowers depth further.");
            return;
        }
    }

    // Neatly aligned path log
    log_message("\n✅ Full solving path:");
    log_message("  " + string(100, '='));

    for (size_t i = 0; i < path.size() - 1; ++i) {
        ostringstream line;
        line << "  ➤ "
             << left << setw(6) << moves[i]
             << "→ Canonical: " << path[i + 1]
             << " | Rotation: " << setw(8) << left << rotations[i + 1]
             << " | Depth: " << (start_depth - static_cast<int>(i + 1));
        log_message(line.str());
    }

    log_message("  " + string(100, '='));

    // Final one-line solution summary
    stringstream solution_line;
    if (!rotations[0].empty()) {
        solution_line << rotations[0] << " ";
    }

    for (size_t i = 0; i < moves.size(); ++i) {
        solution_line << moves[i];
        if (!rotations[i + 1].empty()) {
            solution_line << " " << rotations[i + 1] << "";
        }
        solution_line << " ";
    }

    //log_message("\n Final solution: " + solution_line.str() + "(" + to_string(moves.size()) + " moves)");
    log_message("\n Final solution: " + cleanup(solution_line.str()) + "(" + to_string(moves.size()) + " moves)");
}


int count_wing_pairs(const string& state) {
    int pairs = 0;
    for (size_t i = 0; i + 1 < state.size(); i += 2) {
        if (state[i] == state[i + 1]) {
            pairs++;
        }
    }
    return pairs;
}

int count_wide_turns(const string& scramble) {
    stringstream ss(scramble);
    string move;
    int count = 0;

    while (ss >> move) {
        if (move.length() >= 2 && move[1] == 'w') {
            // Check if it's NOT a wide double turn (e.g., Rw2)
            if (move.length() == 2 || (move.length() == 3 && move[2] != '2')) {
                count++;
            }
        }
    }

    return count;
}

string count_cycles(const string& input) {
    if (input.length() != 24) return "Invalid input length";

    // Step 1: Split into 2-letter pairs
    vector<pair<char, char>> pairs;
    for (int i = 0; i < 24; i += 2) {
        char a = input[i];
        char b = input[i + 1];
        if (a != b) {
            pairs.emplace_back(a, b);
        }
    }

    vector<bool> used(pairs.size(), false);
    vector<int> cycle_lengths;

    while (true) {
        // Find the first unused pair
        int start_idx = -1;
        for (int i = 0; i < pairs.size(); ++i) {
            if (!used[i]) {
                start_idx = i;
                break;
            }
        }
        if (start_idx == -1) break;  // No more unused pairs

        char target = pairs[start_idx].first;
        char hand = pairs[start_idx].second;
        used[start_idx] = true;
        int length = 1;

        while (true) {
            bool found = false;
            for (int i = 0; i < pairs.size(); ++i) {
                if (used[i]) continue;
                auto& [a, b] = pairs[i];
                if (a == hand || b == hand) {
                    // Replace the opposite with hand, pick up the other
                    hand = (a == hand) ? b : a;
                    used[i] = true;
                    ++length;
                    found = true;
                    break;
                }
            }
            if (!found || hand == target) break;
        }

        cycle_lengths.push_back(length);
    }

    // Sort cycle lengths descending and format result
    sort(cycle_lengths.rbegin(), cycle_lengths.rend());
    string result;
    for (int len : cycle_lengths) {
        result += to_string(len) + "e";
    }

    return result;
}

string apply_scramble_to_wings(const string& scramble) {
    string current_state = "aabbccddeejjnnrruuvvwwzz";
    stringstream ss(scramble);
    string move;


    //log_message("🔁 Applying scramble: " + scramble);
    //log_message("🔹 Start state: " + current_state);

    int step = 1;
    while (ss >> move) {
        string before = current_state;
        current_state = apply_move_wings(current_state, move);
        //log_message("Step " + to_string(step - 1) + ": Move '" + move + "'\n" + "        Before: " + before + "\n" + "        After : " + current_state);
    }

    //log_message("🔹 Final state: " + current_state);
    return current_state;
}




// Non-recursive all-path solver with tree-debug prints
void solver_bin_all_paths_generalized(
    const string& scramble,
    const string& bin_filename,
    int min_pairs_filter,
    int additional_depths_allowed = 0,
    function<void(const string&)> handle_result = nullptr
) {
    log_message("Exploring all solving paths (+0 to +" + to_string(additional_depths_allowed) + ") for scramble: " + scramble);
    ofstream outfile("phase1.txt", ios::app);
    ofstream detailed("phase1_detailed.txt", ios::app);

    string scrambled_state = translate_scramble_to_moves(scramble);
    auto [canonical_start, rotation_start] = get_canonical_form_with_rotation(scrambled_state);
    string trimmed_start = canonical_start.substr(1);
    __uint128_t rank_start = multinomial_rank(trimmed_start, {{'a', 7}, {'b', 8}, {'c', 8}});
    size_t index_start = static_cast<size_t>(rank_start);
    size_t byte_offset_start = index_start / 2;
    bool is_high_start = (index_start % 2 == 0);

    ifstream binfile(bin_filename, ios::binary);
    if (!binfile.is_open()) {
        log_message("❌ Failed to open binary file.");
        return;
    }
    binfile.seekg(byte_offset_start);
    uint8_t byte_start;
    binfile.read(reinterpret_cast<char*>(&byte_start), 1);
    binfile.close();

    uint8_t flag_start = is_high_start ? (byte_start >> 4) & 0xF : byte_start & 0xF;
    int start_depth = static_cast<int>(flag_start);

    if (start_depth == 15) {
        log_message("❌ Starting state not found (flag = 15).");
        return;
    }

    struct Node {
        string state;
        int depth;
        string move;
        string rotation;
        vector<string> moves;
        vector<string> rotations;
        int non_decreasing_used;
    };

    auto read_depth = [&](const string& state) -> int {
        string trimmed = state.substr(1);
        __uint128_t rank = multinomial_rank(trimmed, {{'a', 7}, {'b', 8}, {'c', 8}});
        size_t idx = static_cast<size_t>(rank);
        size_t offset = idx / 2;
        bool is_high = (idx % 2 == 0);
        ifstream file(bin_filename, ios::binary);
        file.seekg(offset);
        uint8_t b;
        file.read(reinterpret_cast<char*>(&b), 1);
        file.close();
        return is_high ? (b >> 4) & 0xF : b & 0xF;
    };

    unordered_set<string> printed_paths;
    vector<Node> stack;
    stack.push_back({canonical_start, start_depth, "", rotation_start, {}, {rotation_start}, 0});

    while (!stack.empty()) {
        Node current = stack.back();
        stack.pop_back();

        if (current.depth == 0) {
            stringstream summary;
            if (!current.rotations.empty() && !current.rotations[0].empty()) {
                summary << current.rotations[0] << " ";
            }
            for (size_t i = 0; i < current.moves.size(); ++i) {
                summary << current.moves[i];
                if (i + 1 < current.rotations.size() && !current.rotations[i + 1].empty()) {
                    summary << " " << current.rotations[i + 1];
                }
                summary << " ";
            }

            string cleaned = cleanup(summary.str());


            if ((count_wide_turns(cleaned) % 2) == (count_wide_turns(scramble) % 2)){

                if (!printed_paths.count(cleaned)) {
                    printed_paths.insert(cleaned);
                    int pairs = count_wing_pairs(apply_scramble_to_wings(scramble + " " + cleaned));

                    if (pairs >= min_pairs_filter) {
                        string htrc_subset = HTRC_subset(scramble + " " + cleaned);
                        log_message(scramble + " " + cleaned + " | Moves: " + to_string(current.moves.size()) + " | Pairs: " + to_string(pairs) + " | HTRC: " + htrc_subset);
                        outfile << scramble + " " + cleaned << endl;
                        detailed << scramble << " " << cleaned << " | Moves: " << current.moves.size() << " | Pairs: " << pairs << " | HTRC: " << htrc_subset << endl;
                        if (handle_result) {
                            handle_result(cleaned);
                        }
                    }
                }
            }


            continue;
        }

        for (const string& move : MOVES_reduced) {
            string moved_state = apply_move(current.state, move);
            auto [next_canon, next_rot] = get_canonical_form_with_rotation(moved_state);
            int next_depth = read_depth(next_canon);

            bool allow_same_depth = (next_depth == current.depth && current.non_decreasing_used < additional_depths_allowed);
            bool allow_lower_depth = (next_depth < current.depth);

            if (allow_lower_depth || allow_same_depth) {
                vector<string> next_moves = current.moves;
                vector<string> next_rots = current.rotations;
                next_moves.push_back(move);
                next_rots.push_back(next_rot);
                int next_non_decreasing = current.non_decreasing_used + (next_depth == current.depth ? 1 : 0);
                stack.push_back({next_canon, next_depth, move, next_rot, next_moves, next_rots, next_non_decreasing});
            }
        }
    }

    outfile.close();
    detailed.close();
}
































string apply_scramble(const string& start_state, const string& scramble) {
    string state = start_state;
    stringstream ss(scramble);
    string move;
    while (ss >> move) {
        state = apply_move(state, move);
    }
    return state;
}

bool two_edge_two_edge_swap_detector(const string& scramble) {
    string initial_state = "aaaabbbbccccddddeeeeffff";
    string scrambled_state = translate_scramble_to_moves_precise(scramble);

    vector<pair<int, pair<char, char>>> mismatches;

    for (size_t i = 0; i < scrambled_state.length(); i += 2) {
        if (i + 1 >= scrambled_state.length()) break;

        char first = scrambled_state[i];
        char second = scrambled_state[i + 1];

        if (first != second) {
            mismatches.push_back({static_cast<int>(i), {first, second}});
        }
    }

    cout << "🔍 Scrambled state: " << scrambled_state << endl;
    cout << "📊 2-edge swap mismatches:" << endl;

    for (const auto& m : mismatches) {
        cout << "Mismatch at indices " << m.first << " and " << m.first + 1
             << ": " << m.second.first << " ≠ " << m.second.second << endl;
    }

    if (mismatches.size() != 2) {
        cout << "❌ Not exactly two mismatch pairs — total: " << mismatches.size() << endl;
        return false;
    }

    // Extract mismatch characters
    char a = mismatches[0].second.first;
    char b = mismatches[0].second.second;
    char c = mismatches[1].second.first;
    char d = mismatches[1].second.second;

    bool valid_swap = (a == d && b == c) || (a == c && b == d);

    if (valid_swap) {
        cout << "✅ Valid 2-edge 2-edge swap detected." << endl;
    } else {
        cout << "❌ Mismatches do not form two swaps." << endl;
    }

    return valid_swap;
}

bool is_two_edge_two_edge_swap(const string& scramble) {
    string initial_state = "aabbccddeeffgghhiijjkkll";
    string scrambled_state = translate_scramble_to_moves_wings(scramble);

    vector<pair<char, char>> mismatches;
    for (size_t i = 0; i < scrambled_state.size(); i += 2) {
        if (i + 1 >= scrambled_state.size()) break;
        if (scrambled_state[i] != scrambled_state[i + 1]) {
            mismatches.emplace_back(scrambled_state[i], scrambled_state[i + 1]);
        }
    }

    if (mismatches.size() != 4) return false;

    // Try to group the mismatches into two matching unordered pairs (identical or inverse)
    vector<bool> used(4, false);
    for (int i = 0; i < 4; ++i) {
        if (used[i]) continue;
        for (int j = i + 1; j < 4; ++j) {
            if (used[j]) continue;
            auto a = mismatches[i];
            auto b = mismatches[j];
            if ((a.first == b.second && a.second == b.first) || 
                (a.first == b.first && a.second == b.second)) {
                used[i] = used[j] = true;
                break;
            }
        }
    }

    // Debug output
    cout << "state: " << scrambled_state << endl;
    cout << "mismatches: ";
    for (const auto& m : mismatches) {
        cout << "(" << m.first << ", " << m.second << ") ";
    }
    cout << endl;
    cout << "used: ";
    for (const auto& u : used) {
        cout << (u ? "1" : "0") << " ";
    }
    cout << endl;

    return all_of(used.begin(), used.end(), [](bool b) { return b; });
}


void brute_force_scramble_explorer(
    const string& scramble,
    int max_depth = 5,
    bool filter_pairs = false,
    int min_pairs = 6,
    bool filter_subset = false,
    const unordered_set<string>& allowed_subsets = {}
) {
    log_message("🔎 Brute-forcing all paths from scramble: " + scramble);
    string base_state = translate_scramble_to_moves(scramble);
    string base_wing_state = translate_scramble_to_moves_wings(scramble);

    struct Node {
        string move_sequence;
        string last_face_group;
        string state;
        string wing_state;
        int depth;
    };

    queue<Node> q;
    q.push({"", "", base_state, base_wing_state, 0});

    auto extract_face_group = [](const string& move) -> string {
        if (move.length() >= 2 && move[1] == 'w') return move.substr(0, 2);
        return move.substr(0, 1);
    };

    while (!q.empty()) {
    Node current = q.front();
    q.pop();

    if (current.depth > max_depth) continue;

    string cleaned_sequence = cleanup(current.move_sequence);
    string full_sequence = scramble + " " + cleaned_sequence;

    string centers_after = apply_scramble("aaaabbbbccccddddeeeeffff", full_sequence);
    string subset = HTRC_subset(full_sequence);

    // Pairs
    int pairs = count_wing_pairs(current.wing_state);
    bool passes_pair_filter = !filter_pairs || pairs >= min_pairs;

    // Subset filter
    bool passes_subset_filter = !filter_subset || allowed_subsets.count(subset);

    if (passes_pair_filter && passes_subset_filter) {
        stringstream output;
        output << left << setw(35) << cleaned_sequence
               << "(depth=" << current.depth
               << ", pairs=" << pairs
               << ", subset=" << subset << ")";
        log_message(output.str());

        // Optional debug info:
        // log_message("🔹 Centers after: " + centers_after);
    }

    if (current.depth < max_depth) {
        for (const string& move : MOVES_post_HTRC) {
            string face_group = extract_face_group(move);
            if (face_group == current.last_face_group) continue;

            string new_state = apply_move(current.state, move);
            string new_wing = apply_move_wings(current.wing_state, move);
            q.push({current.move_sequence + " " + move, face_group, new_state, new_wing, current.depth + 1});
        }
    }
}
}



void ultimate_oneslice_solver_phase1(const string& scramble, const string& bin_filename = "HTR_C.bin") {
    string timestamp = to_string(time(nullptr));
    string phase1_filename = "phase1.txt";
    ofstream outfile(phase1_filename);

    if (!outfile.is_open()) {
        log_message("❌ Failed to open phase 1 output file.");
        return;
    }

    log_message("🚀 Phase 1: " + scramble);

    auto save_result = [&](const string& moves) {
        int pairs = count_wing_pairs(apply_scramble_to_wings(scramble + " " + moves));
        if (pairs >= 3) {
            outfile << scramble << " " << moves << endl;
            log_message("✅ Phase 1 valid: " + moves + " (pairs = " + to_string(pairs) + ")");
        }
    };

    // using new function:
    solver_bin_all_paths_generalized(scramble, bin_filename, 3, 1, save_result);


    outfile.close();
    log_message("📄 Phase 1 results saved to: " + phase1_filename);
}






void ultimate_oneslice_solver_phase2(const string& phase1_filename = "phase1.txt") {
    ifstream infile(phase1_filename);
    if (!infile.is_open()) {
        log_message("❌ Failed to open phase1 file: " + phase1_filename);
        return;
    }

    ofstream outfile("phase2.txt");
    ofstream detailed_out("phase2_detailed.txt");

    if (!outfile.is_open() || !detailed_out.is_open()) {
        log_message("❌ Failed to open output file(s): phase2.txt or phase2_detailed.txt");
        return;
    }

    unordered_set<string> allowed_subsets = {
        "aaa", "aab", "aac", "aae", "aee",
        "abc", "abb", "abe", "acc", "ace",
        "bbc", "bbb", "bbe"
    };

    const vector<string>& ALL_MOVES = MOVES_post_HTRC;

    string line;
    int branch_index = 0;

    while (getline(infile, line)) {
        if (line.empty()) continue;
        ++branch_index;

        log_message("🌿 Phase 2: Exploring branch #" + to_string(branch_index) + ": " + line);

        string base_state = translate_scramble_to_moves(line);
        string base_wing_state = translate_scramble_to_moves_wings(line);

        struct Node {
            string move_sequence;
            string last_face_group;
            string state;
            string wing_state;
            int depth;
        };

        queue<Node> q;
        q.push({"", "", base_state, base_wing_state, 0});

        auto extract_face_group = [](const string& move) -> string {
            if (move.length() >= 2 && move[1] == 'w') return move.substr(0, 2);
            return move.substr(0, 1);
        };

        while (!q.empty()) {
            Node current = q.front();
            q.pop();

            if (current.depth > 4) continue;

            string cleaned_sequence = cleanup(current.move_sequence);
            string full_sequence = line + " " + cleaned_sequence;
            int pairs = count_wing_pairs(current.wing_state);
            string subset = HTRC_subset(full_sequence);

            if (pairs >= 5 && allowed_subsets.count(subset)) {
                int move_count = 0;
                for (const string& token : tokenize_scramble(cleaned_sequence)) {
                    if (!is_rotation_token(token)) ++move_count;
                }

                outfile << full_sequence << "\n";
                detailed_out << full_sequence
                             << " | " << pairs
                             << " | " << subset
                             << " | " << move_count << "\n";

                log_message("✅ Phase 2 valid: " + cleaned_sequence +
                            " (pairs = " + to_string(pairs) +
                            ", subset = " + subset +
                            ", moves = " + to_string(move_count) + ")");
            }

            if (current.depth < 4) {
                for (const string& move : ALL_MOVES) {
                    string face_group = extract_face_group(move);
                    if (face_group == current.last_face_group) continue;

                    string new_state = apply_move(current.state, move);
                    string new_wing = apply_move_wings(current.wing_state, move);
                    q.push({current.move_sequence + " " + move, face_group, new_state, new_wing, current.depth + 1});
                }
            }
        }
    }

    infile.close();
    outfile.close();
    detailed_out.close();

    log_message("📄 Phase 2 results saved to: phase2.txt and phase2_detailed.txt");
}





void ultimate_oneslice_solver_phase3(const string& phase2_filename = "phase2.txt") {
    ifstream infile(phase2_filename);
    if (!infile.is_open()) {
        log_message("❌ Failed to open phase2 file: " + phase2_filename);
        return;
    }

    ofstream outfile("phase_3.txt");
    ofstream detailed_out("phase_3_detailed.txt");

    if (!outfile.is_open() || !detailed_out.is_open()) {
        log_message("❌ Failed to open output file(s): phase_3.txt or phase_3_detailed.txt");
        return;
    }

    const vector<string> OUTER_MOVES = {
        "R", "R'", "R2", "U", "U'", "U2", "D", "D'", "D2",
        "L", "L'", "L2", "F", "F'", "F2", "B", "B'", "B2"
    };
    const vector<string> FINAL_SLICE_MOVES = { "Uw2", "Rw2", "Fw2" };

    string line;
    int branch_index = 0;

    while (getline(infile, line)) {
        if (line.empty()) continue;
        ++branch_index;

        log_message("🌿 Phase 3: Branch #" + to_string(branch_index) + ": " + line);

        string base_state = translate_scramble_to_moves(line);
        string base_wing_state = translate_scramble_to_moves_wings(line);

        struct Node {
            string move_sequence;
            string last_face_group;
            string state;
            string wing_state;
            int depth;
        };

        queue<Node> q;
        q.push({"", "", base_state, base_wing_state, 0});

        auto extract_face_group = [](const string& move) -> string {
            return move.substr(0, 1);
        };

        while (!q.empty()) {
            Node current = q.front();
            q.pop();

            if (current.depth > 5) continue;

            for (const string& slice_move : FINAL_SLICE_MOVES) {
                string sequence = cleanup(current.move_sequence + " " + slice_move);
                string full_sequence = line + " " + sequence;

                string final_state = apply_move(current.state, slice_move);
                string final_wings = apply_move_wings(current.wing_state, slice_move);
                int pairs = count_wing_pairs(final_wings);
                string subset = HTRC_subset(full_sequence);

                if (pairs >= 8 && subset == "abb") {
                    int move_count = 0;
                    for (const string& token : tokenize_scramble(sequence)) {
                        if (!is_rotation_token(token)) ++move_count;
                    }

                    outfile << full_sequence << "\n";

                    bool is_2e2e = is_two_edge_two_edge_swap(full_sequence);

                    detailed_out << full_sequence
                                << " | " << pairs
                                << " | " << subset
                                << " | " << move_count
                                << " | 2e2e_swap=" << (is_2e2e ? "true" : "false") << "\n";

                    log_message("✅ Phase 3 valid: " + sequence +
                        " (pairs = " + to_string(pairs) +
                        ", subset = " + subset +
                        ", moves = " + to_string(move_count) +
                        ", 2e2e_swap = " + (is_2e2e ? "true" : "false") + ")");
                }
            }

            if (current.depth < 5) {
                for (const string& move : OUTER_MOVES) {
                    string face_group = extract_face_group(move);
                    if (face_group == current.last_face_group) continue;

                    string new_state = apply_move(current.state, move);
                    string new_wing = apply_move_wings(current.wing_state, move);
                    q.push({current.move_sequence + " " + move, face_group, new_state, new_wing, current.depth + 1});
                }
            }
        }
    }

    infile.close();
    outfile.close();
    detailed_out.close();

    log_message("📄 Phase 3 results saved to: phase_3.txt and phase_3_detailed.txt");
}



void brute_force_extend_phase1_results(const string& filename, int min_pairs, int brute_force_depth) {
    ifstream infile(filename);
    if (!infile.is_open()) {
        log_message("❌ Could not open " + filename + " for reading.");
        return;
    }

    string line;
    while (getline(infile, line)) {
        if (line.empty()) continue;

        log_message("\n🔎 Brute-forcing extensions for: " + line);

        string base_state = translate_scramble_to_moves(line);
        string base_wing_state = translate_scramble_to_moves_wings(line);

        struct Node {
            string move_sequence;
            string last_face_group;
            string state;
            string wing_state;
            int depth;
        };

        queue<Node> q;
        q.push({"", "", base_state, base_wing_state, 0});

        auto extract_face_group = [](const string& move) -> string {
            if (move.length() >= 2 && move[1] == 'w') return move.substr(0, 2);
            return move.substr(0, 1);
        };

        while (!q.empty()) {
            Node current = q.front();
            q.pop();

            if (current.depth > brute_force_depth) continue;

            string full_sequence = line + " " + cleanup(current.move_sequence);
            string final_wing_state = current.wing_state;
            int pairs = count_wing_pairs(final_wing_state);

            if (pairs >= min_pairs) {
                string subset = HTRC_subset(full_sequence);
                log_message("🧮 " + cleanup(current.move_sequence) + " | Total: " + full_sequence + " | P: " + to_string(pairs) + " | Sub: " + subset + " | W: " + final_wing_state);
            }

            if (current.depth < brute_force_depth) {
                for (const string& move : MOVES_post_HTRC) {
                    string face_group = extract_face_group(move);
                    if (face_group == current.last_face_group) continue;

                    string new_state = apply_move(current.state, move);
                    string new_wing = apply_move_wings(current.wing_state, move);
                    q.push({current.move_sequence + " " + move, face_group, new_state, new_wing, current.depth + 1});
                }
            }
        }
    }

    infile.close();
}







mutex log_mutex;
mutex file_mutex;  

void brute_force_worker(
    const vector<string>& entries,
    int min_pairs,
    int brute_depth,
    bool use_subset_filter,
    const unordered_set<string>& allowed_subsets
){
    ofstream outfile("phase2.txt", ios::app);  // open once per thread

    for (const string& line : entries) {
        if (line.empty()) continue;

        {
            lock_guard<mutex> guard(log_mutex);
            log_message("🧵 Thread processing: " + line);
        }

        string base_state = translate_scramble_to_moves(line);
        string base_wing_state = translate_scramble_to_moves_wings(line);

        struct Node {
            string move_sequence;
            string last_face_group;
            string state;
            string wing_state;
            int depth;
        };

        queue<Node> q;
        q.push({"", "", base_state, base_wing_state, 0});

        auto extract_face_group = [](const string& move) -> string {
            if (move.length() >= 2 && move[1] == 'w') return move.substr(0, 2);
            return move.substr(0, 1);
        };

        while (!q.empty()) {
            Node current = q.front();
            q.pop();

            if (current.depth > brute_depth) continue;

            string cleaned_sequence = cleanup(current.move_sequence);
            string full_sequence = line + " " + cleaned_sequence;
            int pairs = count_wing_pairs(current.wing_state);
            string subset = HTRC_subset(full_sequence);
            string cycle_pattern = count_cycles(current.wing_state);

            // ✅ Save immediately if 3c2e2e is found
            if (cycle_pattern == "3c2e2e") {
                {
                    lock_guard<mutex> guard(file_mutex);
                    outfile << full_sequence << "\n";
                }
                {
                    lock_guard<mutex> guard(log_mutex);
                    log_message("🌟 3c2e2e found: " + full_sequence);
                }
            }

            // ✅ Existing logic for pair/subset filtering
            if (pairs >= min_pairs && (!use_subset_filter || allowed_subsets.count(subset))) {
                {
                    lock_guard<mutex> guard(file_mutex);
                    outfile << full_sequence << "\n";
                }

                {
                    lock_guard<mutex> guard(log_mutex);
                    log_message("✅ " + full_sequence +
                        " | Pairs: " + to_string(pairs) +
                        " | Cycles: " + cycle_pattern +
                        " | Subset: " + subset +
                        " | Wings: " + current.wing_state);
                }
            }

            if (current.depth < brute_depth) {
                for (const string& move : MOVES_post_HTRC) {
                    string face_group = extract_face_group(move);
                    if (face_group == current.last_face_group) continue;

                    string new_state = apply_move(current.state, move);
                    string new_wing = apply_move_wings(current.wing_state, move);
                    q.push({current.move_sequence + " " + move, face_group, new_state, new_wing, current.depth + 1});
                }
            }
        }
    }

    outfile.close();
}



void parallel_brute_force_phase1(
    const string& filename,
    int min_pairs,
    int brute_depth,
    int num_threads,
    bool use_subset_filter,
    const unordered_set<string>& allowed_subsets
) {
    ifstream infile(filename);
    vector<string> entries;
    string line;
    while (getline(infile, line)) {
        if (!line.empty()) entries.push_back(line);
    }

    size_t total = entries.size();
    size_t chunk_size = (total + num_threads - 1) / num_threads;

    vector<thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        size_t start = i * chunk_size;
        size_t end = min(start + chunk_size, total);
        vector<string> subset(entries.begin() + start, entries.begin() + end);

        threads.emplace_back(
            brute_force_worker,
            subset,
            min_pairs,
            brute_depth,
            use_subset_filter,
            cref(allowed_subsets)  // pass by reference safely for threads
        );
    }

    for (auto& t : threads) t.join();

    log_message("✅ All threads complete.");
}



// Main function
int main() {
   auto start_time = high_resolution_clock::now();
   precompute_factorials();

    //string scramble = " "; // example scramble
    //test_scramble_on_wings(scramble);
    //scramble = "Fw"; // example scramble
    //test_scramble_on_wings(scramble);

    //solver_bin_all_paths("L2 R2 F Uw' L2 Rw B U Fw2 Rw R' D2 F2 R Fw' Uw2 Rw2 Uw2 D B2 R2 Fw2 Uw2 D2 Rw' Fw' D L' Fw' F' U' B Fw Rw D F B' Fw U2 D", "HTR_C.bin");
    //solver_bin_all_paths_plus_two("L2 R2 F Uw' L2 Rw B U Fw2 Rw R' D2 F2 R Fw' Uw2 Rw2 Uw2 D B2 R2 Fw2 Uw2 D2 Rw' Fw' D L' Fw' F' U' B Fw Rw D F B' Fw U2 D", "HTR_C.bin");
    
    //solver_bin_all_paths("L2 R2 F Uw' L2 Rw B U Fw2 Rw R' D2 F2 R Fw' Uw2 Rw2 Uw2 D B2 R2 Fw2 Uw2 D2 Rw' Fw' D L' Fw' F' U' B Fw Rw D F B' Fw U2 D", "HTR_C.bin", 3);
    //solver_bin_all_paths_plus_one("L2 R2 F Uw' L2 Rw B U Fw2 Rw R' D2 F2 R Fw' Uw2 Rw2 Uw2 D B2 R2 Fw2 Uw2 D2 Rw' Fw' D L' Fw' F' U' B Fw Rw D F B' Fw U2 D", "HTR_C.bin", 3);
    //brute_force_scramble_explorer("L2 R2 F Uw' L2 Rw B U Fw2 Rw R' D2 F2 R Fw' Uw2 Rw2 Uw2 D B2 R2 Fw2 Uw2 D2 Rw' Fw' D L' Fw' F' U' B Fw Rw D F B' Fw U2 D Fw2 Dw2 R U' Bw2 Dw2 Rw Bw F' Dw ", 5);
    //brute_force_scramble_explorer("L2 R2 F Uw' L2 Rw B U Fw2 Rw R' D2 F2 R Fw' Uw2 Rw2 Uw2 D B2 R2 Fw2 Uw2 D2 Rw' Fw' D L' Fw' F' U' B Fw Rw D F B' Fw U2 D Fw2 Dw2 R U' Bw2 Dw2 Rw Bw F' Dw", 5, true);  // Only show results with ≥ 8 pairs
    //brute_force_scramble_explorer("L2 R2 F Uw' L2 Rw B U Fw2 Rw R' D2 F2 R Fw' Uw2 Rw2 Uw2 D B2 R2 Fw2 Uw2 D2 Rw' Fw' D L' Fw' F' U' B Fw Rw D F B' Fw U2 D Fw2 Dw2 R U' Bw2 Dw2 Rw Bw F' Dw D2 B' U2 Lw2 Bw2", 5, true, 8, false, { "bee", "dee" });

    // U2 R' Uw2                          (depth=3, pairs=5, subset=abb) 
    //R2 B2 Fw2 R Fw2                    (depth=5, pairs=6, subset=aae)


   //ultimate_oneslice_solver_phase1("L2 R2 F Uw' L2 Rw B U Fw2 Rw R' D2 F2 R Fw' Uw2 Rw2 Uw2 D B2 R2 Fw2 Uw2 D2 Rw' Fw' D L' Fw' F' U' B Fw Rw D F B' Fw U2 D");
   //ultimate_oneslice_solver_phase2("phase1.txt");
   //ultimate_oneslice_solver_phase3("phase2.txt");
//
    // R' D B' L B2 L' U' B L2 F D2 F2 U2 F D2 F' R2 B L' F2 Rw2 F' Rw2 Uw2 R F Uw2 F' R L B L2 Fw2 Uw' F' U B2 Rw2 U Rw' U2 Rw' Uw2 Fw U2 Fw'
   //ultimate_oneslice_solver_phase1("L2 R2 F Uw' L2 Rw B U Fw2 Rw R' D2 F2 R Fw' Uw2 Rw2 Uw2 D B2 R2 Fw2 Uw2 D2 Rw' Fw' D L' Fw' F' U' B Fw Rw D F B' Fw U2 D");
   //ultimate_oneslice_solver_phase2("phase1.txt");
   //ultimate_oneslice_solver_phase3("phase2.txt");
   //solver_bin("L' F2 R' B2 R2 L' B2 U2 R' F2 B2 U L D R2 U L' F' B U Rw2 F B2 R2 Rw2 U Rw2 U2 B2 U F2 Rw2 F' Rw F2 D L B2 Rw Uw' Rw F' U B2 Uw");


   solver_bin_all_paths_generalized("Lw' Uw' R' Lw Uw2 B Lw Dw' Lw2 F Rw L' Bw Dw Lw2 Bw' R' U Dw R2 Bw Fw2 U' B2 Lw2 Dw' R Fw2 Rw2 Fw' Lw2 F' Dw2 Fw' Lw2 L' Rw B' Fw' F2 Bw Rw2 U R Rw2 Fw' F U B F' Fw Dw L2 Dw2 F' U Uw Lw' Rw2 Uw2", "HTR_C.bin", /*p_min=*/2, /*d=*/2);
   parallel_brute_force_phase1("phase1.txt", /*min_pairs=*/5, /*brute_depth=*/4, /*num_threads=*/8, /*use_subset_filter=*/true, /*allowed_subsets=*/{"aaa", "aab", "aac", "aae", "aee", "abc", "abb", "abe", "acc", "ace", "bbc", "bbb", "bbe"});

   // d=2, bd=5 : took 680 sec, 80 ergebnisse (dopplungen)
   // d=3, bd=4 war nicht schlecht für viele cases.

   // wing-cycle that work: 2e2e3e

   // mit den aktuellen Einstellungen (z.B. erlauben von 0 pairs in phase 1) bekommene pair=5 cases: 80 potentielle. 123 sec
   // mit den aktuellen Einstellungen (z.B. erlauben von 1 pairs in phase 1) bekommene pair=5 cases: 80 potentielle. 43 sec



   //cout << is_two_edge_two_edge_swap("L2 R2 F Uw' L2 Rw B U Fw2 Rw R' D2 F2 R Fw' Uw2 Rw2 Uw2 D B2 R2 Fw2 Uw2 D2 Rw' Fw' D L' Fw' F' U' B Fw Rw D F B' Fw U2 D R Bw2 R Bw R2 Bw B Uw' L2 Dw D2 Fw2 U F Rw2 L F R D' F2 Rw2 U' L D B' R B Uw2");

    //HTRC_subset("B L2 Uw2 L' F' Rw2 B' U' D Uw Rw2 F R2 Rw' Fw R' F' Uw' R' D' F U2 Dw L2 B' Bw2 Uw' Fw Rw");

    //uncomment to generate all states:
    //explorer();
    //generate_next_depth_from_file("depth_10.txt", "depth_11.txt", "depth_12.txt");
    //solver_txt("L' U' B2 U2 B2 L2 D R2 D L2 U2 L' D B L' F' D' R2 B2 U Uw2 F Fw2 Rw2 B D Uw2 F U B2 Rw2 Uw2 B2 Rw F U' Rw B' Rw2 Uw B2 Rw' B2 R Fw' R"); // Example scramble
    
    //binary_generator(0, 2100000000, "all_states.bin"); 

    //write_multinomial_ranks_to_file("depth_0.txt", "depth_0_ranks.txt");
    //test_multinomial_rank("bccccccccbbbbbbbaaaaaaa");
    //generate_blank_bin_file("HTR_C.bin", 2100000000);
    //bin_file_preview("HTR_C.bin", 64);
    //flag_multinomial_ranks_in_4bit_bin_file("depth_4.txt", "HTR_C.bin", 4);
    //flag_multinomial_ranks_in_4bit_bin_file("depth_5.txt", "HTR_C.bin", 5);
    //flag_multinomial_ranks_in_4bit_bin_file("depth_6.txt", "HTR_C.bin", 6);
    //flag_multinomial_ranks_in_4bit_bin_file("depth_7.txt", "HTR_C.bin", 7);
    //flag_multinomial_ranks_in_4bit_bin_file("depth_8.txt", "HTR_C.bin", 8);
    //flag_multinomial_ranks_in_4bit_bin_file("depth_9.txt", "HTR_C.bin", 9);
    //flag_multinomial_ranks_in_4bit_bin_file("depth_11.txt", "HTR_C.bin", 11);
    //solver_bin("L' U' B2 U2 B2 L2 D R2 D L2 U2 L' D B L' F' D' R2 B2 U Uw2 F Fw2 Rw2 B D Uw2 F U B2 Rw2 Uw2 B2 Rw F U' Rw B' Rw2 Uw B2 Rw' B2 R Fw' R", "HTR_C.bin");
    //solver_bin("L2 R2 F Uw' L2 Rw B U Fw2 Rw R' D2 F2 R Fw' Uw2 Rw2 Uw2 D B2 R2 Fw2 Uw2 D2 Rw' Fw' D L' Fw' F' U' B Fw Rw D F B' Fw U2 D");

    //solver_bin_all_paths("L2 R2 F Uw' L2 Rw B U Fw2 Rw R' D2 F2 R Fw' Uw2 Rw2 Uw2 D B2 R2 Fw2 Uw2 D2 Rw' Fw' D L' Fw' F' U' B Fw Rw D F B' Fw U2 D");
    //solver_bin("R' F L2 F2 D2 R D2 R2 U2 F2 D2 L' U2 L' U' F' D' F' L' B R2 Rw2 B' R F' B L2 Uw2 L' F' Rw2 B' U' D Uw Rw2 F R2 Rw' Fw R' F' Uw' R' D'", "HTR_C.bin");
    
    ///solver_bin_all_paths("L2 R2 F Uw' L2 Rw B U Fw2 Rw R' D2 F2 R Fw' Uw2 Rw2 Uw2 D B2 R2 Fw2 Uw2 D2 Rw' Fw' D L' Fw' F' U' B Fw Rw D F B' Fw U2 D", "HTR_C.bin");
    
    //solver_bin_all_paths_plus_one("L2 R2 F Uw' L2 Rw B U Fw2 Rw R' D2 F2 R Fw' Uw2 Rw2 Uw2 D B2 R2 Fw2 Uw2 D2 Rw' Fw' D L' Fw' F' U' B Fw Rw D F B' Fw U2 D", "HTR_C.bin");
    //solver_bin_all_paths("L' U' B2 U2 B2 L2 D R2 D L2 U2 L' D B L' F' D' R2 B2 U Uw2 F Fw2 Rw2 B D Uw2 F U B2 Rw2 Uw2 B2 Rw F U' Rw B' Rw2 Uw B2 Rw' B2 R Fw' R", "HTR_C.bin");

    //solver_bin_all_paths("Rw D2 Dw");
    //solver bin all paths is buggy



    auto end_time = high_resolution_clock::now(); // code for timer
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    log_message("Execution time: " + to_string(duration.count()) + " ms");
    log_file.close();
    return 0;
}


