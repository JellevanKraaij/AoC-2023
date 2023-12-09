#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<int> parseNumbers(const string &numbers) {
    vector<int> result;
    stringstream ss(numbers);

    int num;
    while (ss >> num) {
        result.push_back(num);
    }
    return result;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " inputfile" << endl;
        return (EXIT_FAILURE);
    }

    ifstream inputFile(argv[1]);

    if (!inputFile) {
        cout << "Cannot open inputfile" << endl;
        return (EXIT_FAILURE);
    }

    string line;
    int total1 = 0;
    int total2 = 0;
    while (getline(inputFile, line)) {
        vector<vector<int>> table;
        table.push_back(parseNumbers(line));
        for (size_t i = 0; !all_of(table[i].begin(), table[i].end(), [](int n) { return n == 0; }); ++i) {
            vector<int> &row = table[i];
            vector<int> newRow;
            for (size_t j = 0; j < row.size() - 1; ++j) {
                newRow.push_back(row[j + 1] - row[j]);
            }
            table.push_back(newRow);
        }
        int num1 = 0;
        int num2 = 0;
        for (auto it = table.rbegin(); it != table.rend(); ++it) {
            num1 = it->back() + num1;
            num2 = it->front() - num2;
        }
        cout << "num1: " << num1 << endl;
        cout << "num2: " << num2 << endl;
        total1 += num1;
        total2 += num2;
    }
    cout << "total1: " << total1 << endl;
    cout << "total2: " << total2 << endl;
}