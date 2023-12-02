#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

map<string, int> gameBag = {{"red", 12}, {"green", 13}, {"blue", 14}};

struct gameLine {
    int gameId;
    vector<map<string, int>> colors;
};

gameLine parseLine(const string &line) {
    gameLine game;

    size_t pos = line.find(':');
    game.gameId = stoi(line.substr(5, pos - 5));

    stringstream ss(line.substr(pos + 2));
    string chunk;
    while (getline(ss, chunk, ';')) {
        stringstream ss(chunk);
        string chunk;
        while (getline(ss, chunk, ',')) {
            int colorValue;
            string colorName;
            stringstream(chunk) >> colorValue >> colorName;
            game.colors.push_back({{colorName, colorValue}});
        }
    }
    return game;
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
        bool valid = true;
        gameLine game = parseLine(line);
        map<string, int> maxColor;
        for (auto &color : game.colors) {
            for (auto &c : color) {
                if (gameBag[c.first] < c.second) {
                    valid = false;
                }
                if (maxColor[c.first] < c.second) {
                    maxColor[c.first] = c.second;
                }
            }
        }
        if (valid) {
            cout << "Game " << game.gameId << " is valid" << endl;
            total1 += game.gameId;
        }
        int power = 1;
        for (auto &c : maxColor) {
            power *= c.second;
        }
        total2 += power;
    }
    cout << "Total1: " << total1 << endl;
    cout << "Total2: " << total2 << endl;
}
