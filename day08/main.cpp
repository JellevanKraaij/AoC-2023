#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

long calculateCyclusLength(const unordered_map<string, pair<string, string>> &nodes, const string &startNode, const string &instructions,
                           const regex &endRegex) {
    string currentNode = startNode;
    long steps = 0;
    auto it = instructions.begin();

    while (true) {
        auto [left, right] = nodes.at(currentNode);
        if (*it == 'L')
            currentNode = left;
        else
            currentNode = right;
        steps++;
        it++;
        if (it == instructions.end())
            it = instructions.begin();
        if (regex_match(currentNode, endRegex))
            break;
    }
    return steps;
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

    string instructions;
    getline(inputFile, instructions);

    string line;
    unordered_map<string, pair<string, string>> nodes;
    while (getline(inputFile, line)) {
        if (line.empty())
            continue;
        nodes[line.substr(0, 3)] = make_pair(line.substr(7, 3), line.substr(12, 3));
    }

    cout << "Part1: " << calculateCyclusLength(nodes, "AAA", instructions, regex("ZZZ")) << endl;

    vector<string> startNodes;
    for_each(nodes.begin(), nodes.end(), [&startNodes](auto &node) {
        if (node.first.ends_with("A"))
            startNodes.push_back(node.first);
    });

    vector<long> cyclusLengths;
    transform(startNodes.begin(), startNodes.end(), back_inserter(cyclusLengths),
              [&nodes, &instructions](auto &startNode) { return calculateCyclusLength(nodes, startNode, instructions, regex("..Z")); });

    long lcmResult = cyclusLengths.front();
    for_each(cyclusLengths.begin() + 1, cyclusLengths.end(), [&lcmResult](auto &cyclusLength) { lcmResult = lcm(lcmResult, cyclusLength); });
    cout << "Part2: " << lcmResult << endl;
}
