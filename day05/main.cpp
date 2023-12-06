#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<long> parseNumbers(const string &numbers) {
    vector<long> result;
    stringstream ss(numbers);

    long num;
    while (ss >> num) {
        result.push_back(num);
    }
    return result;
}

class ConvertRange {
   public:
    ConvertRange() = default;

    void addRange(long sourceStart, long destinationStart, long length) { ranges.push_back(Range(sourceStart, destinationStart, length)); }
    long convert(long source) {
        for (auto &range : ranges) {
            if (range.inRange(source))
                return range.convert(source);
        }
        return source;
    }

   public:
    class Range {
       public:
        Range(long sourceStart, long destinationStart, long length)
            : sourceStart(sourceStart), sourceEnd(sourceStart + length), conversion(destinationStart - sourceStart) {}
        long convert(long source) {
            if (inRange(source))
                return source + conversion;
            return source;
        }
        bool inRange(long source) { return source >= sourceStart && source < sourceEnd; }

       public:
        long sourceStart;
        long sourceEnd;
        long conversion;
    };
    vector<Range> ranges;
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

    vector<long> numbers;
    vector<ConvertRange> convertRanges;

    string line;
    getline(inputFile, line);
    numbers = parseNumbers(line.substr(6));
    getline(inputFile, line);
    ConvertRange convertRange;
    while (getline(inputFile, line)) {
        if (line.empty()) {
            convertRanges.push_back(convertRange);
            convertRange = ConvertRange();
            continue;
        }
        if (isdigit(line[0])) {
            vector<long> numbers = parseNumbers(line);
            convertRange.addRange(numbers[1], numbers[0], numbers[2]);
        }
    }
    convertRanges.push_back(convertRange);

    long smallest1 = numeric_limits<long>::max();
    for_each(numbers.begin(), numbers.end(), [&smallest1, convertRanges](long seed) {
        for (ConvertRange range : convertRanges)
            seed = range.convert(seed);
        if (seed < smallest1)
            smallest1 = seed;
    });

    long smallest2 = numeric_limits<long>::max();
    for (auto it = numbers.begin(); it != numbers.end(); it += 2) {
        long seedStart = *it;
        long seedLength = *(it + 1);

        cout << "Seed: " << seedStart << " - " << seedLength << endl;
        for (long i = 0; i < seedLength; i++) {
            long res = seedStart + i;
            for (ConvertRange range : convertRanges) {
                res = range.convert(res);
            }
            if (res < smallest2)
                smallest2 = res;
        }
    }

    cout << "Part 1: " << smallest1 << endl;
    cout << "Part 2: " << smallest2 << endl;
}
