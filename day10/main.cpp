#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "Vec2.hpp"

using namespace std;

class Tile {
   public:
    enum TileType {
        GROUND = '.',
        VERTICAL = '|',
        HORIZONTAL = '-',
        BEND_NE = 'L',
        BEND_NW = 'J',
        BEND_SW = '7',
        BEND_SE = 'F',
        START = 'S',
    };
    TileType type;
    Tile(TileType type) : type(type) {}
    bool isPartOfPath = false;
    bool isBend() { return type == Tile::BEND_NE || type == Tile::BEND_NW || type == Tile::BEND_SW || type == Tile::BEND_SE; }
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
        transform(line.begin(), line.end(), back_inserter(row), [](char c) { return static_cast<Tile::TileType>(c); });
        map.push_back(row);
    }

    Vec2 startLocation;
    for (auto it = map.begin(); it != map.end(); ++it) {
        auto res = find_if(it->begin(), it->end(), [](Tile t) { return t.type == Tile::START; });
        if (res != it->end()) {
            startLocation[0] = distance(map.begin(), it);
            startLocation[1] = distance(it->begin(), res);
            cout << "Found start at " << startLocation[0] << ", " << startLocation[1] << endl;
            break;
        }
    }

    static const array<pair<Vec2, vector<Tile::TileType>>, 4> directions = {{{Vec2{0, 1}, {Tile::HORIZONTAL, Tile::BEND_SW, Tile::BEND_NW}},
                                                                             {Vec2{0, -1}, {Tile::HORIZONTAL, Tile::BEND_SE, Tile::BEND_NE}},
                                                                             {Vec2{1, 0}, {Tile::VERTICAL, Tile::BEND_NW, Tile::BEND_NE}},
                                                                             {Vec2{-1, 0}, {Tile::VERTICAL, Tile::BEND_SW, Tile::BEND_SE}}}};
    Vec2 startDirection;
    for (auto &direction : directions) {
        Vec2 testLocation = startLocation + direction.first;
        if (testLocation[0] < 0 || testLocation[0] >= map.size() || testLocation[1] < 0 || testLocation[1] >= map[testLocation[0]].size()) {
            continue;
        }
        if (find(direction.second.begin(), direction.second.end(), map[testLocation[0]][testLocation[1]].type) != direction.second.end()) {
            startDirection = direction.first;
            break;
        }
    }

    static const unordered_map<Tile::TileType, pair<Vec2, Vec2>> tileDirections = {
        {Tile::BEND_NE, {{-1, 0}, {0, 1}}}, {Tile::BEND_NW, {{0, -1}, {-1, 0}}},   {Tile::BEND_SW, {{1, 0}, {0, -1}}},
        {Tile::BEND_SE, {{0, 1}, {1, 0}}},  {Tile::HORIZONTAL, {{0, 1}, {0, -1}}}, {Tile::VERTICAL, {{1, 0}, {-1, 0}}}};
    Vec2 currentLocation = startLocation + startDirection;
    Vec2 currentDirection = startDirection;
    int steps = 0;

    while (true) {
        steps++;
        map[currentLocation[0]][currentLocation[1]].isPartOfPath = true;
        if (map[currentLocation[0]][currentLocation[1]].type == Tile::START) {
            break;
        }
        pair<Vec2, Vec2> direction = tileDirections.at(map[currentLocation[0]][currentLocation[1]].type);
        if (currentDirection == inverse(direction.first)) {
            currentDirection = direction.second;
        } else {
            currentDirection = direction.first;
        }
        currentLocation = currentLocation + currentDirection;
    }
    cout << "Steps: " << steps << endl;

    static const std::map<pair<Vec2, Vec2>, Tile::TileType> convertStart = {
        {{{0, 1}, {0, -1}}, Tile::HORIZONTAL}, {{{0, -1}, {0, 1}}, Tile::HORIZONTAL}, {{{1, 0}, {-1, 0}}, Tile::VERTICAL},
        {{{-1, 0}, {1, 0}}, Tile::VERTICAL},   {{{0, 1}, {1, 0}}, Tile::BEND_SW},     {{{1, 0}, {0, 1}}, Tile::BEND_SW},
        {{{0, -1}, {1, 0}}, Tile::BEND_SE},    {{{1, 0}, {0, -1}}, Tile::BEND_SE},    {{{0, -1}, {-1, 0}}, Tile::BEND_NE},
        {{{-1, 0}, {0, -1}}, Tile::BEND_NE},   {{{0, 1}, {-1, 0}}, Tile::BEND_NW},    {{{-1, 0}, {0, 1}}, Tile::BEND_NW},
    };
    map[startLocation[0]][startLocation[1]].type = convertStart.at(make_pair(startDirection, inverse(currentDirection)));

    cout << "Start value: " << static_cast<char>(map[startLocation[0]][startLocation[1]].type) << endl;

    enum Direction { UP, DOWN };

    unordered_map<Tile::TileType, Direction> bendDirections = {
        {Tile::BEND_NE, UP}, {Tile::BEND_NW, UP}, {Tile::BEND_SW, DOWN}, {Tile::BEND_SE, DOWN}};

    int total = 0;
    for (auto &row : map) {
        bool insidePath = false;
        bool encounteredBend = false;
        Direction lastDir;
        for (auto &tile : row) {
            if (tile.isPartOfPath) {
                if (tile.type == Tile::VERTICAL)
                    insidePath = !insidePath;
                else if (tile.isBend()) {
                    if (!encounteredBend) {
                        encounteredBend = true;
                        lastDir = bendDirections.at(tile.type);
                    } else {
                        if (lastDir != bendDirections.at(tile.type))
                            insidePath = !insidePath;
                        encounteredBend = false;
                    }
                }
            }

            if (insidePath && !tile.isPartOfPath) {
                cout << 'I';
                total++;
            } else {
                cout << static_cast<char>(tile.type);
            }
        }
        cout << endl;
    }

    cout << "Result 1: " << steps / 2 << endl;
    cout << "Result 2: " << total << endl;
}
