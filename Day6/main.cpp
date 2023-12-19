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

int getTravelDistance(int timeHeld, int totalTime);
vector<int> getTravelDistancesForRace(int totalTime);
int getWinPossibilities(const vector<int> &distances, int bestDistance);

vector<int> parseIntsLine(string input);

auto main(int argc, char *argv[]) -> int {
    file = ifstream(INPUT);
    if (file.is_open()) {
        string line;
        getline(file, line);
        // parse times
        vector<int> raceTimes = parseIntsLine(line);
        // parse distances
        getline(file, line);
        vector<int> raceDists = parseIntsLine(line);

        int waysToWinMultiplied = 1;
        for (int i = 0; i < raceTimes.size(); i++) {
            vector<int> distances = getTravelDistancesForRace(raceTimes[i]);
            waysToWinMultiplied *= getWinPossibilities(distances, raceDists[i]);
        }

        cout << "Margin of error: " << waysToWinMultiplied << endl;
        file.close();
    }

    return 0;
}

int getTravelDistance(int timeHeld, int totalTime) {
    return timeHeld * (totalTime - timeHeld);
}

vector<int> getTravelDistancesForRace(int totalTime) {
    vector<int> travelDistances;
    for (int i = 0; i < totalTime; i++) {
        travelDistances.push_back(getTravelDistance(i, totalTime));
    }
    return travelDistances;
}

int getWinPossibilities(const vector<int> &distances, int bestDistance) {
    int wins = 0;
    for (int x: distances) {
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

void testSampleRaces() {
    vector<int> distances = getTravelDistancesForRace(sampleRace.time);
    int winPossibles = getWinPossibilities(distances, sampleRace.bestDistance);

    cout << "Race 1: " << winPossibles << endl;

    vector<int> distances2 = getTravelDistancesForRace(sampleRace2.time);
    int winPossibles2 = getWinPossibilities(distances2, sampleRace2.bestDistance);

    cout << "Race 2: " << winPossibles2 << endl;

    vector<int> distances3 = getTravelDistancesForRace(sampleRace3.time);
    int winPossibles3 = getWinPossibilities(distances3, sampleRace3.bestDistance);

    cout << "Race 3: " << winPossibles3 << endl;
}
