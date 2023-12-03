#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

inline bool isSymbol(char c) {
    return (!isdigit(c) && c != '.');
}

enum Type { empty, symbol, number };
using Element = pair<Type, int>;
using Row = vector<Element>;
using Map = vector<Row>;

optional<vector<Element>> checkSurrounding(Map &map, int i, int j, function<bool(Element)> check) {
    static const array<pair<int, int>, 8> directions = {{{-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}}};
    vector<Element> result;
    for (auto [id, jd] : directions) {
        if (i + id < 0 || i + id >= map.size() || j + jd < 0 || j + jd >= map[i].size())
            continue;
        if (check(map[i + id][j + jd]))
            result.push_back(map[i + id][j + jd]);
    }
    if (!result.empty())
        return result;
    return {};
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
    Map map;
    while (getline(inputFile, line)) {
        Row row;
        for (size_t i = 0; i < line.size(); ++i) {
            if (isdigit(line[i])) {
                int number = stoi(line.substr(i));
                for (; isdigit(line[i]); ++i)
                    row.push_back({Type::number, number});
                --i;
            } else if (isSymbol(line[i]))
                row.push_back({Type::symbol, line[i]});
            else
                row.push_back({Type::empty, 0});
        }
        map.push_back(row);
    }

    int total1 = 0;
    int total2 = 0;
    for (size_t i = 0; i < map.size(); ++i) {
        bool hasSymbol = false;
        int number = 0;
        for (size_t j = 0; j < map[i].size(); ++j) {
            if (map[i][j].first == Type::number) {
                number = map[i][j].second;
                if (checkSurrounding(map, i, j, [](Element e) { return (e.first == Type::symbol); }))
                    hasSymbol = true;
            }
            if ((j == map.size() - 1 || map[i][j].first != Type::number) && hasSymbol) {
                total1 += number;
                hasSymbol = false;
            }

            if (map[i][j].first == Type::symbol && map[i][j].second == '*') {
                if (auto res = checkSurrounding(map, i, j, [](Element e) { return (e.first == Type::number); })) {
                    vector<int> unique;
                    for (auto e : res.value()) {
                        if (find(unique.begin(), unique.end(), e.second) == unique.end())
                            unique.push_back(e.second);
                    }
                    if (unique.size() == 2)
                        total2 += unique[0] * unique[1];
                }
            }
        }
    }
    cout << "Total1: " << total1 << endl;
    cout << "Total2: " << total2 << endl;
}
