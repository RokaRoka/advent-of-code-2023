#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
#include <string>
#include <vcruntime.h>
#include <vector>
#include <regex>
using namespace std;

// Day 2 Goal:
// Figure out how many games of colored-cube pulls are possible, 
// given the actual amount of cubes.

const string DAY2_INPUT = "input.txt";
const int RED_CUBE_AMOUNT = 12;
const int GREEN_CUBE_AMOUNT = 13;
const int BLUE_CUBE_AMOUNT = 14;

// variables
ifstream file;

// functions
bool openFile(string path);
void closeFile();

// Functions
bool checkGamePossible(string gameInput);
bool checkGamePull(string pullInput);

auto main(int argc, char *argv[]) -> int {
    // test case
    // string testGame = "Game 1: 3 green, 12 red, 15 blue;";
    // cout << testGame << endl;
    // smatch m;
    // regex rId ("Game (\\d)+:");
    // if (regex_search(testGame, m, rId)) {
    //     testGame = m.suffix();
    //     string id = m[1];
    //     cout << id << ": " << checkGamePossible(testGame) << endl;    
    // }
    if (openFile(DAY2_INPUT)) {
        string gameLine;
        int validGamesSum = 0;
        smatch m;
        regex rId ("Game (\\d+):");
        while (getline(file, gameLine)) {
            if (regex_search(gameLine, m, rId)) {
                int id = stoi(m[1]);
                gameLine = m.suffix();
                bool valid = checkGamePossible(gameLine);
                if (valid) {
                    validGamesSum += id;
                }
                cout << id << ": " << valid << endl;  
            }
        }
        cout << "ID sum: " << validGamesSum << endl;
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

bool checkGamePossible(const string gameInput) {
    bool gameValid = true;

    // use find to separate out pull sets
    size_t end = gameInput.find_first_of(';');
    size_t last = 0;
    while (gameValid && last != gameInput.size()) {
        string pullInput = gameInput.substr(last, last - end);

        gameValid = checkGamePull(pullInput);
       
        last = end;
        end = gameInput.find_first_of(';', end + 1);
        if (end == string::npos) {
            end = gameInput.size();
        }
    }

    return gameValid;
}

bool checkGamePull(string pullInput) {
    cout << pullInput << endl;
     // use regex to separate out pull data
    regex r ("(\\d+) (\\w+)[,|;]*");
    smatch m;
    while(regex_search(pullInput, m, r)) {
        int amt = stoi(m[1]);
        string color = m[2];

        if (color == "red") {
            if (amt > RED_CUBE_AMOUNT) return false;
        }
        else if (color == "green") {
            if (amt > GREEN_CUBE_AMOUNT) return false;
        }
        else if (color == "blue") {
            if (amt > BLUE_CUBE_AMOUNT) return false;
        }
        pullInput = m.suffix();
    }
    return true;
}