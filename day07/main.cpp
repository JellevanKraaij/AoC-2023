#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

const map<char, int> convertCard1 = {{'A', 14}, {'K', 13}, {'Q', 12}, {'J', 11}, {'T', 10}, {'9', 9}, {'8', 8},
                                     {'7', 7},  {'6', 6},  {'5', 5},  {'4', 4},  {'3', 3},  {'2', 2}};
const map<char, int> convertCard2 = {{'A', 14}, {'K', 13}, {'Q', 12}, {'J', 1}, {'T', 10}, {'9', 9}, {'8', 8},
                                     {'7', 7},  {'6', 6},  {'5', 5},  {'4', 4}, {'3', 3},  {'2', 2}};
vector<int> convertHand(const string &str, const map<char, int> &convert) {
    vector<int> result;
    for (auto &c : str) {
        result.push_back(convert.at(c));
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

    if (count.contains(1)) {
        int jokerCount = count[1];
        count.erase(count.find(1));
        int largest = max_element(count.begin(), count.end(), [](auto &a, auto &b) { return a.second < b.second; })->first;
        count[largest] += jokerCount;
    }

    int largest = max_element(count.begin(), count.end(), [](auto &a, auto &b) { return a.second < b.second; })->second;
    static const map<pair<int, int>, HandType> handType = {
        {{1, 5}, FIVE_OF_A_KIND}, {{2, 4}, FOUR_OF_A_KIND}, {{2, 3}, FULL_HOUSE}, {{3, 3}, THREE_OF_A_KIND},
        {{3, 2}, TWO_PAIRS},      {{4, 2}, ONE_PAIR},       {{5, 1}, HIGH_CARD},
    };
    return handType.find({count.size(), largest})->second;
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
    vector<pair<vector<int>, int>> hands1;
    vector<pair<vector<int>, int>> hands2;

    while (getline(inputFile, line)) {
        stringstream ss(line);

        string cards;
        ss >> cards;

        int bid;
        ss >> bid;
        hands1.push_back(make_pair(convertHand(cards, convertCard1), bid));
        hands2.push_back(make_pair(convertHand(cards, convertCard2), bid));
    }

    for_each(hands1.begin(), hands1.end(), [](auto &hand) { hand.first.insert(hand.first.begin(), getHandType(hand.first)); });
    for_each(hands2.begin(), hands2.end(), [](auto &hand) { hand.first.insert(hand.first.begin(), getHandType(hand.first)); });

    sort(hands1.begin(), hands1.end(), [](auto &a, auto &b) { return a.first < b.first; });
    sort(hands2.begin(), hands2.end(), [](auto &a, auto &b) { return a.first < b.first; });

    long result1 = 0;
    long result2 = 0;
    for (size_t i = 0; i < min(hands1.size(), hands2.size()); i++) {
        result1 += hands1[i].second * (i + 1);
        result2 += hands2[i].second * (i + 1);
    }
    cout << "Result: " << result1 << endl;
    cout << "Result: " << result2 << endl;
}
