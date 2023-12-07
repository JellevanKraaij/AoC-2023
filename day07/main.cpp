#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

vector<int> parseCards(string cards) {
    vector<int> result;

    static const map<char, int> convertCard = {{'A', 14}, {'K', 13}, {'Q', 12}, {'J', 11}, {'T', 10}};

    for (auto &c : cards) {
        if (isdigit(c))
            result.push_back(c - '0');
        else
            result.push_back(convertCard.at(c));
    }
    return result;
}

enum HandType {
    HIGH_CARD = 0,
    ONE_PAIR = 1,
    TWO_PAIRS = 2,
    THREE_OF_A_KIND = 3,
    FULL_HOUSE = 4,
    FOUR_OF_A_KIND = 5,
    FIVE_OF_A_KIND = 6,
};

HandType getHandType(const vector<int> &cards) {
    map<int, int> count;
    for (auto &c : cards) {
        count[c]++;
    }

    if (count.size() == 1)
        return FIVE_OF_A_KIND;
    if (count.size() == 2) {
        if (count.begin()->second == 1 || count.begin()->second == 4)
            return FOUR_OF_A_KIND;
        return FULL_HOUSE;
    }
    if (count.size() == 3) {
        if (count.begin()->second == 2 || count.rbegin()->second == 2)
            return TWO_PAIRS;
        return THREE_OF_A_KIND;
    }
    if (count.size() == 4)
        return ONE_PAIR;
    return HIGH_CARD;
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
    vector<pair<vector<int>, int>> hands;
    while (getline(inputFile, line)) {
        stringstream ss(line);

        string cards;
        ss >> cards;

        int bid;
        ss >> bid;
        hands.push_back(make_pair(parseCards(cards), bid));
    }
    sort(hands.begin(), hands.end(), [](auto &a, auto &b) { return a.first < b.first; });

    long result = 0;
    for (size_t i = 0; i < hands.size(); i++) {
        result += hands[i].second * (i + 1);
    }
    cout << "Result: " << result << endl;
}
