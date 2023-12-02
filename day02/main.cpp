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

    string colors = line.substr(pos + 2);
    stringstream ss(colors);
    string chunk;
    int i = 0;
    while (getline(ss, chunk, ';')) {
        stringstream ss2(chunk);
        string chunk2;
        while (getline(ss2, chunk2, ',')) {
            stringstream ss3(chunk2);
            int colorValue;
            string colorName;
            ss3 >> colorValue >> colorName;
            game.colors.push_back({{colorName, colorValue}});
        }
        i++;
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
    int total = 0;
    while (getline(inputFile, line)) {
        bool valid = true;
        gameLine game = parseLine(line);
        for (auto &color : game.colors) {
            for (auto &c : color) {
                if (gameBag[c.first] < c.second) {
                    valid = false;
                }
            }
        }
        if (valid) {
            cout << "Game " << game.gameId << " is valid" << endl;
            total += game.gameId;
        }
    }
    cout << "Total: " << total << endl;
}
