#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct CardInfo {
    int id;
    vector<int> numbers;
    vector<int> winning;
};

vector<int> parseNumbers(const string &numbers) {
    vector<int> result;
    stringstream ss(numbers);

    int num;
    while (ss >> num)
        result.push_back(num);
    return result;
}

CardInfo parseCardInfo(const string &line) {
    CardInfo cardInfo;

    size_t colonPos = line.find(':');
    cardInfo.id = stoi(line.substr(5, colonPos));

    size_t orPos = line.find("|", colonPos);

    cardInfo.numbers = parseNumbers(line.substr(colonPos + 2, orPos - colonPos - 3));
    cardInfo.winning = parseNumbers(line.substr(orPos + 2));

    return cardInfo;
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
    vector<tuple<CardInfo, int, int>> cardResults;
    vector<CardInfo> cards;
    while (getline(inputFile, line)) {
        cards.push_back(parseCardInfo(line));
    }

    // Part 1
    transform(cards.begin(), cards.end(), back_inserter(cardResults), [](CardInfo &cardInfo) {
        sort(cardInfo.numbers.begin(), cardInfo.numbers.end());
        sort(cardInfo.winning.begin(), cardInfo.winning.end());
        vector<int> intersection;
        set_intersection(cardInfo.numbers.begin(), cardInfo.numbers.end(), cardInfo.winning.begin(), cardInfo.winning.end(),
                         back_inserter(intersection));
        return (make_tuple(cardInfo, intersection.size(), 1));
    });

    // Part 2
    for (auto it = cardResults.begin(); it != cardResults.end(); ++it) {
        auto &[card, points, cnt] = *it;
        auto it2 = it + 1;
        for (int i = 0; i < points && it2 != cardResults.end(); ++i, ++it2) {
            auto &[card2, points2, cnt2] = *it2;
            if (card.id != card2.id) {
                cnt2 += cnt;
            }
        }
    }

    // Print results
    int total1 = 0, total2 = 0;
    for (auto &[cardInfo, points, cnt] : cardResults) {
        int cpoints = pow(2, points - 1);
        cout << "Card " << cardInfo.id << ": " << cpoints << " - " << cnt << endl;
        total1 += cpoints;
        total2 += cnt;
    }
    cout << "Total1: " << total1 << endl;
    cout << "Total2: " << total2 << endl;
}
