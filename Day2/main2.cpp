#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
#include <string>
#include <array>
#include <regex>
using namespace std;

// Day 2 Goal:
// Figure out the lowest amount of each cube color
// given the possible pulls of each game.
//
// Multiply those amounts together, the power,
// and return the sum.

const string DAY2_INPUT = "input.txt";
const int RED = 0;
const int GREEN = 1;
const int BLUE = 2;

// variables
ifstream file;

// functions
bool openFile(string path);
void closeFile();

// Functions
int getGamePower(const string gameInput);
array< int, 3 > &getGamePullAmounts(string pullInput, array< int, 3 > &out);

auto main(int argc, char *argv[]) -> int {
    if (openFile(DAY2_INPUT)) {
        string gameLine;
        int powerSum = 0;
        smatch m;
        regex rId ("Game (\\d+):");
        while (getline(file, gameLine)) {
            if (regex_search(gameLine, m, rId)) {
                int id = stoi(m[1]);
                gameLine = m.suffix();
                int power = getGamePower(gameLine);
                powerSum += power;
                cout << id << ": " << power << endl;
            }
        }
        cout << "Power sum: " << powerSum << endl;
    }
    return 0;
}

bool openFile(string path) {
    file = ifstream(path);
    return (file.is_open());
}

void closeFile() {
    file.close();
}

int getGamePower(const string gameInput) {
    // highest pulls for red (0) green (1) and blue (2)
    int highest[3] {0, 0, 0};

    // use find to separate out pull sets
    size_t end = gameInput.find_first_of(';');
    size_t last = 0;
    while (last != gameInput.size()) {
        string pullInput = gameInput.substr(last, end - last);

        array< int, 3 > latest {0, 0, 0};
        latest = getGamePullAmounts(pullInput, latest);

        // compare latest to lowest
        for (int i = 0; i < 3; i++) {
            if (latest[i] > highest[i])
                highest[i] = latest[i];
        }

        last = end;
        end = gameInput.find_first_of(';', end + 1);
        if (end == string::npos) {
            end = gameInput.size();
        }
    }

    cout << "Lowest: red - " << highest[RED] << ", green - " << highest[GREEN] << ", blue - " << highest[BLUE] << endl;
    // return multiplied power

    return highest[RED] * highest[GREEN] * highest[BLUE];
}

array< int, 3 > &getGamePullAmounts(string pullInput, array< int, 3 > &out) {
    cout << "Pull Input: " << pullInput << endl;

    // use regex to separate out pull data
    regex r ("(\\d+) (\\w+)[,|;]*");
    smatch m;
    while(regex_search(pullInput, m, r)) {
        int amt = stoi(m[1]);
        string color = m[2];

        if (color == "red") {
            out[RED] = amt;
        }
        else if (color == "green") {
            out[GREEN] = amt;
        }
        else if (color == "blue") {
            out[BLUE] = amt;
        }
        pullInput = m.suffix();
    }
    cout << "Pulls: red - " << out[RED] << ", green - " << out[GREEN] << ", blue - " << out[BLUE] << endl << endl;
    return out;
}
