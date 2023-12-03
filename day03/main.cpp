#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

array<pair<int, int>, 8> directions = {{{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}}};

inline bool isSymbol(char c) {
    return (!isdigit(c) && c != '.');
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
    vector<vector<char>> map;
    while (getline(inputFile, line)) {
        vector<char> row;
        for (auto &c : line) {
            row.push_back(c);
        }
        map.push_back(row);
    }
    int total = 0;
    for (size_t i = 0; i < map.size(); ++i) {
        stringstream ss;
        bool symbol = false;
        for (size_t j = 0; j < map[i].size() + 1; ++j) {
            if (j < map[i].size() && isdigit(map[i][j])) {
                ss << map[i][j];
                for (auto [id, jd] : directions) {
                    if (i + id < 0 || i + id >= map.size() || j + jd < 0 || j + jd >= map[i].size())
                        continue;
                    if (isSymbol(map[i + id][j + jd]))
                        symbol = true;
                }
            } else {
                if (ss.str().empty())
                    continue;
                int value;
                ss >> value;
                ss = stringstream();
                cout << value;
                if (symbol) {
                    total += value;
                    symbol = false;
                    cout << "X";
                }
                cout << " ";
            }
        }
        cout << endl;
    }
    cout << "Total: " << total << endl;
}
