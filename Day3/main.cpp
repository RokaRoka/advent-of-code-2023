#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
using namespace std;

// Day 3 Goal:
// Add up all part numbers to work our which part num is missing.
// - Provided a visual representation of the engine, of nums and symbols
// - Any number adjacent to a symbol (diagonal too) is a part number and should be added
// - Periods are not a symbol

// Idea:
// Before
// - Get all input data in a two dimensional array (or a string array, basically)
// - Have function interface for getting chars from 2d arr
//
// Solution
// - Parse each line with regex
// - For each number found, look at the numbers nearby

const string INPUT = "input.txt";
int maxColumn = 0;

ifstream file;
vector<string> engineScheme;

bool openFile(string path);
void closeFile();
bool isAdjacentSymbol(int col, int rowBegin, int rowEnd);
void testCase();
void printSymbolCheck(int row, int colBegin, int colEnd);

auto main(int argc, char *argv[]) -> int {
    // test case
    //testCase();

    if (openFile(INPUT)) {
        engineScheme = vector<string>();
        string inputLine;
        while (getline(file, inputLine)) {
            engineScheme.push_back(inputLine);
        }
        maxColumn = engineScheme[0].size();

        // find sum of part nums
        int partsSum = 0;
        // parse through each line with regex looking for numbers
        regex r("\\d+");
        for (int i = 0; i < engineScheme.size(); i++) {
            string line(engineScheme[i]);
            int offset = 0;
            smatch m;
            while (regex_search(line.cbegin() + offset, line.cend(), m, r)) {
                string numStr = m[0];
                int num = stoi(numStr);
                cout << "Found num: " << numStr << endl;
                if (isAdjacentSymbol(i, offset + m.position(), offset + m.position() + numStr.size())) {
                    partsSum += num;
                }

                offset += m.position() + numStr.size();
            }
        }

        cout << "Parts Sum is " << partsSum << endl;
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

bool isAdjacentSymbol(int row, int colBegin, int colEnd) {
    printSymbolCheck(row, colBegin, colEnd);
    // cout << "Test for detecting adjacent symbol: " << endl;
    // cout << "r: " << row << endl;
    // cout << "Col begin-end: " << colBegin << "-" << colEnd << endl;
    regex r ("[^\\d\\.]");

    // check row before curr row
    if (row > 0) {
        // Use min and max so we check only in the bounds of the scheme data
        for (int i = max(colBegin - 1, 0); i < min(colEnd + 1, maxColumn); i++) {
            string toCheck(engineScheme[row-1], i, 1) ;
            if (regex_match(toCheck, r)) {
                return true;
            }
        }
    }

    // check col before colBegin and col after colEnd
    if (colBegin > 0) {
        string toCheck(engineScheme[row], colBegin - 1, 1) ;
        if (regex_match(toCheck, r)) {
            return true;
        }
    }
    if (colEnd < maxColumn) {
        string toCheck(engineScheme[row], colEnd, 1) ;
        if (regex_match(toCheck, r)) {
            return true;
        }
    }


    if (row < engineScheme.size() - 1) {
        // check row after
        for (int i = max(colBegin - 1, 0); i < min(colEnd + 1, maxColumn); i++) {
            string toCheck(engineScheme[row+1], i, 1) ;
            if (regex_match(toCheck, r)) {
                return true;
            }
        }
        cout << endl;
    }

    // cout << "No symbol :(" << endl;
    // cout << endl;

    cout << "False!" << endl;
    cout << endl;
    return false;
}

void printSymbolCheck(int row, int colBegin, int colEnd) {
    cout << "Symbol check:" << endl;
    cout << "Row: " << row << ", Cols" << colBegin << " - " << colEnd << endl;

    // check row before curr row
    if (row > 0) {
        // Use min and max so we check only in the bounds of the scheme data
        for (int i = max(colBegin - 1, 0); i < min(colEnd + 1, maxColumn); i++) {
            string toCheck(engineScheme[row-1], i, 1);
            cout << toCheck;
        }
        cout << endl;
    }

    // check col before colBegin and col after colEnd
    if (colBegin > 0) {
        string toCheck(engineScheme[row], colBegin - 1, 1);
        cout << toCheck;
    }
    cout << string(engineScheme[row], colBegin, colEnd - colBegin);
    if (colEnd < maxColumn) {
        string toCheck(engineScheme[row], colEnd, 1);
        cout << toCheck;
    }
    cout << endl;

    if (row < engineScheme.size() - 1) {
        // check row after
        for (int i = max(colBegin - 1, 0); i < min(colEnd + 1, maxColumn); i++) {
            string toCheck(engineScheme[row+1], i, 1) ;
            cout << toCheck;
        }
        cout << endl;
    }

    cout << endl;
}

void testCase() {
    // load engine scheme into vector
    engineScheme = {
    "11...234..",
    "..*..#...1",
    "12345.....",
    };

    // find sum of part nums
    int partsSum = 0;
    // parse through each line with regex looking for numbers
    regex r("\\d+");
    smatch m;
    for (int i = 0; i < engineScheme.size(); i++) {
        string line(engineScheme[i]);
        cout << line << endl;
        while (regex_search(line, m, r)) {
            string numStr = m[0];
            int num = stoi(numStr);
            if (isAdjacentSymbol(i, m.position(), m.position() + numStr.size())) {
                partsSum += num;
            }

            line = m.suffix();
        }
    }

    cout << "Parts Sum is " << partsSum << endl;

    if (partsSum == int(11 + 234 + 12345)) {
        cout << "Yahoo!! Just as expected!" << endl;
    }
}
