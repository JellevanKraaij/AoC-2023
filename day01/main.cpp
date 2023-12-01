#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>

using namespace std;

const regex reg("(one|two|three|four|five|six|seven|eight|nine|[1-9])");
const std::map<std::string, int> convert = {{"one", 1},   {"two", 2},   {"three", 3}, {"four", 4}, {"five", 5}, {"six", 6},
                                            {"seven", 7}, {"eight", 8}, {"nine", 9},  {"1", 1},    {"2", 2},    {"3", 3},
                                            {"4", 4},     {"5", 5},     {"6", 6},     {"7", 7},    {"8", 8},    {"9", 9}};

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
    int sum_ex01 = 0;
    int sum_ex02 = 0;
    while (getline(inputFile, line)) {
        {
            auto first = find_if(line.begin(), line.end(), [](const unsigned char &c) { return isdigit(c); });
            auto last = find_if(line.rbegin(), line.rend(), [](const unsigned char &c) { return isdigit(c); });
            if (first != line.end() && last != line.rend()) {
                sum_ex01 += (convert.at(string(1, first[0])) * 10) + convert.at(string(1, last[0]));
            }
        }
        {
            smatch match;
            string first, last;

            string::const_iterator searchStart(line.cbegin());
            while (regex_search(searchStart, line.cend(), match, reg)) {
                if (first.empty()) {
                    first = match[0];
                }
                searchStart = match.prefix().second + 1;
                last = match[0];
            }
            if (first.empty() || last.empty()) {
                continue;
            }
            sum_ex02 += (convert.at(first) * 10) + convert.at(last);
        }
    }
    cout << "Sum ex01: " << sum_ex01 << endl;
    cout << "Sum ex02: " << sum_ex02 << endl;
}
