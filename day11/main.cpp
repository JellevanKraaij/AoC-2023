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

void expandSpace(Space &space) {
    // Horizontal
    for (int i = 0; i < space.size(); i++) {
        bool hasGalaxy = false;
        for (SpaceCell &cell : space[i]) {
            if (cell == GALAXY) {
                hasGalaxy = true;
                break;
            }
        }
        if (!hasGalaxy) {
            space.insert(space.begin() + i, SpaceRow(space[i].size(), EMPTY));
            i++;
        }
    }
    // Vertical
    for (int i = 0; i < space[0].size(); i++) {
        bool hasGalaxy = false;
        for (int j = 0; j < space.size(); j++) {
            if (space[j][i] == GALAXY) {
                hasGalaxy = true;
                break;
            }
        }
        if (!hasGalaxy) {
            for (auto &row : space) {
                row.insert(row.begin() + i, EMPTY);
            }
            i++;
        }
    }
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

    expandSpace(space);

    vector<pair<int, int>> galaxies;

    for (int i = 0; i < space.size(); i++) {
        for (int j = 0; j < space[i].size(); j++) {
            if (space[i][j] == GALAXY) {
                galaxies.push_back({i, j});
            }
        }
    }

    int total = 0;
    int iteration = 0;
    for (auto it = galaxies.begin(); it != galaxies.end(); it++) {
        auto &[x1, y1] = *it;
        for (auto it2 = it + 1; it2 != galaxies.end(); it2++) {
            auto &[x2, y2] = *it2;
            total += (max(x1, x2) - min(x1, x2)) + (max(y1, y2) - min(y1, y2));
            iteration++;
        }
    }
    cout << "Total: " << total << endl;
    cout << "GalaxyCount: " << iteration << endl;
}