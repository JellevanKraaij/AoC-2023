#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
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

    vector<long> seeds1 = numbers;
    vector<long> result2;

    for (auto it = numbers.begin(); it != numbers.end(); it += 2) {
        long seedStart = *it;
        long seedLength = *(it + 1);

        cout << "Seed: " << seedStart << " - " << seedLength << endl;
        std::vector<long> result_tmp;
        for (long i = 0; i < seedLength; i++) {
            long res = seedStart + i;
            for (ConvertRange range : convertRanges) {
                res = range.convert(res);
            }
            result_tmp.push_back(res);
        }
        long min = ranges::min(result_tmp);
        cout << "range min: " << min << endl;
        result2.push_back(min);
    }

    cout << "Seeds1: " << seeds1.size() << endl;

    vector<long> result1;
    for_each(seeds1.begin(), seeds1.end(), [&result1, convertRanges](long seed) {
        for (ConvertRange range : convertRanges)
            seed = range.convert(seed);
        result1.push_back(seed);
    });

    cout << "Part 1: " << ranges::min(result1) << endl;
    cout << "Part 2: " << ranges::min(result2) << endl;
}
