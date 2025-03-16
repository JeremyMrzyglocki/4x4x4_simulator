#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void modify_file(const string& input_filename, const string& output_filename) {
    ifstream input_file(input_filename);
    ofstream output_file(output_filename);

    if (!input_file) {
        cerr << "❌ Error: Could not open " << input_filename << " for reading!" << endl;
        return;
    }
    if (!output_file) {
        cerr << "❌ Error: Could not open " << output_filename << " for writing!" << endl;
        return;
    }

    string line;
    while (getline(input_file, line)) {
        output_file << "find_solution_with_extra_moves(\"" << line << "\", \"all_five_move_scrambles_unique_states.txt\", moves);" << endl;
    }

    input_file.close();
    output_file.close();
    cout << "✅ File processed successfully. Output saved to: " << output_filename << endl;
}

int main() {
    string input_filename = "scrambles.txt";  // Change this to your input file
    string output_filename = "output.txt"; // Change this to your desired output file

    modify_file(input_filename, output_filename);

    return 0;
}
