#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <regex>
#include <vector>


using namespace std;

const string INPUT = "input.txt";
ifstream file;

struct raceInfo {
    int time;
    int bestDistance;
} sampleRace{7, 9}, sampleRace2{15, 40}, sampleRace3{30, 200};

long getTravelDistance(long timeHeld, long totalTime);
vector<long> getTravelDistancesForRace(long totalTime);
long getWinPossibilities(const vector<long> &distances, long bestDistance);

vector<int> parseIntsLine(string input);
long parseBadlyKernedNum(string input);

auto main(int argc, char *argv[]) -> int {
    file = ifstream(INPUT);
    if (file.is_open()) {
        string line;
        getline(file, line);
        // parse times
        long raceTime = parseBadlyKernedNum(line);
        // parse distances
        getline(file, line);
        long raceDist = parseBadlyKernedNum(line);

        vector<long> distances = getTravelDistancesForRace(raceTime);
        long wins = getWinPossibilities(distances, raceDist);

        cout << "Ways to win this one race: " << wins << endl;
        file.close();
    }

    return 0;
}

long getTravelDistance(long timeHeld, long totalTime) {
    return timeHeld * (totalTime - timeHeld);
}

vector<long> getTravelDistancesForRace(long totalTime) {
    vector<long> travelDistances;
    for (long i = 0; i < totalTime; i++) {
        travelDistances.push_back(getTravelDistance(i, totalTime));
    }
    return travelDistances;
}

long getWinPossibilities(const vector<long> &distances, long bestDistance) {
    long wins = 0;
    for (long x: distances) {
        if (x > bestDistance) {
            wins++;
        }
    }

    return wins;
}

vector<int> parseIntsLine(string input) {
    regex r("\\s+(\\d+)");
    smatch m;

    vector<int> times(0);
    while(regex_search(input, m, r)) {
        times.push_back(stoi(m[1]));
        input = m.suffix();
    }
    return times;
}

// I  l ove  ba d  kern in g
long parseBadlyKernedNum(string input) {
    regex r("\\s+(\\d+)");
    smatch m;

    string numStr = "";
    while(regex_search(input, m, r)) {
        numStr.append(m[1]);
        input = m.suffix();
    }
    cout << numStr << endl;
    return stol(numStr);
}

void testSampleRaces() {
    vector<long> distances = getTravelDistancesForRace(sampleRace.time);
    int winPossibles = getWinPossibilities(distances, sampleRace.bestDistance);

    cout << "Race 1: " << winPossibles << endl;

    vector<long> distances2 = getTravelDistancesForRace(sampleRace2.time);
    int winPossibles2 = getWinPossibilities(distances2, sampleRace2.bestDistance);

    cout << "Race 2: " << winPossibles2 << endl;

    vector<long> distances3 = getTravelDistancesForRace(sampleRace3.time);
    int winPossibles3 = getWinPossibilities(distances3, sampleRace3.bestDistance);

    cout << "Race 3: " << winPossibles3 << endl;
}
