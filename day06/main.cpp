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

long calculateWinCount(long time, long record) {
    long winCount = 0;
    for (long j = 0; j <= time; j++) {
        long speed = j;
        long remainingTime = time - j;
        long distance = speed * remainingTime;
        if (distance > record)
            winCount++;
    }
    return winCount;
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

    string lineTime;
    string lineDistance;

    getline(inputFile, lineTime);
    getline(inputFile, lineDistance);

    vector<long> times = parseNumbers(lineTime.substr(10));
    vector<long> recordDistance = parseNumbers(lineDistance.substr(10));

    long result = 1;
    for (size_t i = 0; i < times.size(); i++) {
        result *= calculateWinCount(times[i], recordDistance[i]);
    }
    cout << "Result1: " << result << endl;

    string lineTime2 = lineTime.substr(10);
    string lineDistance2 = lineDistance.substr(10);

    erase_if(lineTime2, [](char c) { return isspace(c); });
    erase_if(lineDistance2, [](char c) { return isspace(c); });
    long time2 = stol(lineTime2);
    long record2 = stol(lineDistance2);

    cout << "Result2: " << calculateWinCount(time2, record2) << endl;
}
