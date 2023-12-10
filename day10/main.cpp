#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Vec2.hpp"

using namespace std;

enum class Tile {
    GROUND = '.',
    VERTICAL = '|',
    HORIZONTAL = '-',
    BEND_NE = 'L',
    BEND_NW = 'J',
    BEND_SW = '7',
    BEND_SE = 'F',
    START = 'S',
};

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
    vector<vector<Tile>> map;
    while (getline(inputFile, line)) {
        vector<Tile> row;
        transform(line.begin(), line.end(), back_inserter(row), [](char c) { return static_cast<Tile>(c); });
        map.push_back(row);
    }

    Vec2 startLocation;
    for (auto it = map.begin(); it != map.end(); ++it) {
        auto res = find(it->begin(), it->end(), Tile::START);
        if (res != it->end()) {
            startLocation[0] = distance(map.begin(), it);
            startLocation[1] = distance(it->begin(), res);
            cout << "Found start at " << startLocation[0] << ", " << startLocation[1] << endl;
            break;
        }
    }

    static const array<pair<Vec2, vector<Tile>>, 4> directions = {{{Vec2{0, 1}, {Tile::HORIZONTAL, Tile::BEND_SW, Tile::BEND_NW}},
                                                                   {Vec2{0, -1}, {Tile::HORIZONTAL, Tile::BEND_SE, Tile::BEND_NE}},
                                                                   {Vec2{1, 0}, {Tile::VERTICAL, Tile::BEND_NW, Tile::BEND_NE}},
                                                                   {Vec2{-1, 0}, {Tile::VERTICAL, Tile::BEND_SW, Tile::BEND_SE}}}};
    Vec2 startDirection;
    for (auto &direction : directions) {
        Vec2 testLocation = startLocation + direction.first;
        if (testLocation[0] < 0 || testLocation[0] >= map.size() || testLocation[1] < 0 || testLocation[1] >= map[testLocation[0]].size()) {
            continue;
        }
        if (find(direction.second.begin(), direction.second.end(), map[testLocation[0]][testLocation[1]]) != direction.second.end()) {
            startDirection = direction.first;
            break;
        }
    }

    static const unordered_map<Tile, pair<Vec2, Vec2>> tileDirections = {{Tile::BEND_NE, {{-1, 0}, {0, 1}}},    {Tile::BEND_NW, {{0, -1}, {-1, 0}}},
                                                                         {Tile::BEND_SW, {{1, 0}, {0, -1}}},    {Tile::BEND_SE, {{0, 1}, {1, 0}}},
                                                                         {Tile::HORIZONTAL, {{0, 1}, {0, -1}}}, {Tile::VERTICAL, {{1, 0}, {-1, 0}}}};
    Vec2 currentLocation = startLocation + startDirection;
    Vec2 currentDirection = startDirection;
    int steps = 0;

    while (true) {
        steps++;
        if (map[currentLocation[0]][currentLocation[1]] == Tile::START) {
            break;
        }
        pair<Vec2, Vec2> direction = tileDirections.at(map[currentLocation[0]][currentLocation[1]]);
        if (currentDirection == inverse(direction.first)) {
            currentDirection = direction.second;
        } else {
            currentDirection = direction.first;
        }
        currentLocation = currentLocation + currentDirection;
    }
    cout << "Steps: " << steps << endl;
    cout << "Result: " << steps / 2 << endl;
}