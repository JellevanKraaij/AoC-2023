#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

enum Type { EMPTY = '.', GALAXY = '#' };

using SpaceCell = Type;
using SpaceRow = vector<SpaceCell>;
using Space = vector<SpaceRow>;

vector<int> calcEmptyRows(const Space &space) {
    vector<int> emptyRows;
    for (int i = 0; i < space.size(); i++) {
        bool hasGalaxy = false;
        for (const SpaceCell &cell : space[i]) {
            if (cell == GALAXY) {
                hasGalaxy = true;
                break;
            }
        }
        if (!hasGalaxy)
            emptyRows.push_back(i);
    }
    return emptyRows;
}

vector<int> calcEmptyColumns(const Space &space) {
    vector<int> emptyColumns;
    for (int i = 0; i < space[0].size(); i++) {
        bool hasGalaxy = false;
        for (int j = 0; j < space.size(); j++) {
            if (space[j][i] == GALAXY) {
                hasGalaxy = true;
                break;
            }
        }
        if (!hasGalaxy)
            emptyColumns.push_back(i);
    }
    return emptyColumns;
}

vector<pair<int, int>> findGalaxies(const Space &space) {
    vector<pair<int, int>> galaxies;
    for (int i = 0; i < space.size(); i++) {
        for (int j = 0; j < space[i].size(); j++) {
            if (space[i][j] == GALAXY) {
                galaxies.push_back({i, j});
            }
        }
    }
    return galaxies;
}

int passesEmptyRoC(const vector<int> &empty, int start, int end) {
    int count = 0;
    for (int i = start; i < end; i++) {
        if (find(empty.begin(), empty.end(), i) != empty.end())
            count++;
    }
    return count;
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
    Space space;
    int galaxyCount = 0;
    while (getline(inputFile, line)) {
        SpaceRow row;
        for (char c : line)
            row.push_back(c == '#' ? GALAXY : EMPTY);
        space.push_back(row);
    }

    vector<int> emptyRows = calcEmptyRows(space);
    vector<int> emptyColumns = calcEmptyColumns(space);

    vector<pair<int, int>> galaxies = findGalaxies(space);

    long total = 0;
    long total2 = 0;
    int galaxyCombinations = 0;
    for (auto it = galaxies.begin(); it != galaxies.end(); it++) {
        auto &[x1, y1] = *it;
        for (auto it2 = it + 1; it2 != galaxies.end(); it2++) {
            auto &[x2, y2] = *it2;

            int totalWithoutExpansion = (max(x1, x2) - min(x1, x2)) + (max(y1, y2) - min(y1, y2));

            int emptyRowsCnt = passesEmptyRoC(emptyRows, min(x1, x2), max(x1, x2));
            int emptyColumnsCnt = passesEmptyRoC(emptyColumns, min(y1, y2), max(y1, y2));

            total += totalWithoutExpansion + emptyRowsCnt + emptyColumnsCnt;
            total2 += totalWithoutExpansion + (emptyRowsCnt + emptyColumnsCnt) * 999999;
            galaxyCombinations++;
        }
    }
    cout << "Total: " << total << endl;
    cout << "Total2: " << total2 << endl;
    cout << "GalaxyCount: " << galaxyCombinations << endl;
}
