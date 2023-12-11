#include <iostream>
#include <fstream>
#include <iterator>
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
int gearRatioSum = 0;

bool openFile(string path);
void closeFile();
bool isAdjacentSymbol(int col, int rowBegin, int rowEnd);
void testCase(); // test getting inner number
void testCase2(); // test getting a gear

// modifies and adds to gear sum
void evaluateGear(int row, int col);
// gets the full number, even if it starts from a center digit
int getInnerNumber(int row, int col);

auto main(int argc, char *argv[]) -> int {
    // test case
    //testCase2();

    cout << "cmon man" << endl;
    if (openFile(INPUT)) {
        cout << "bruh" << endl;
        engineScheme = vector<string>();
        string inputLine;
        while (getline(file, inputLine)) {
            engineScheme.push_back(inputLine);
        }
        maxColumn = engineScheme[0].size();

        // parse through each line with regex looking for gears
        regex r("\\*");
        for (int i = 0; i < engineScheme.size(); i++) {
            string line(engineScheme[i]);
            int offset = 0;
            smatch m;
            while (regex_search(line.cbegin() + offset, line.cend(), m, r)) {
                evaluateGear(i, offset + m.position());
                offset += (m.position() + m.length());
            }
        }

        cout << "Gear Sum is " << gearRatioSum << endl;
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

void testCase() {
    // load engine scheme into vector
    engineScheme = {
    "11...2347.",
    "..*..#...1",
    "12.45.....",
    };

    // should return 2347
    cout << getInnerNumber(0, 6) << endl;
    // should return 1
    cout << getInnerNumber(1, 9) << endl;
    // should return 12
    cout << getInnerNumber(2, 1) << endl;
    // should return 45
    cout << getInnerNumber(2, 3) << endl;
}

// modifies state, adds to gear sum
void evaluateGear(int row, int col) {
    vector<int> adjacentNumbers (0);
    // top row
    regex r ("\\d+");

    for (int i = max(row - 1, 0); i < min(row + 2, (int)engineScheme.size()); i++) { 
        string rowLine = engineScheme[i];
        int colLeft = max(col - 1, 0);
        int colRight = min(col + 2, maxColumn);
        //cout << "col " << colLeft << " - " << colRight << endl;
        cout << rowLine.substr(colLeft, colRight - colLeft) << endl;
    }

    for (int i = max(row - 1, 0); i < min(row + 2, (int)engineScheme.size()); i++) { 
        string rowLine = engineScheme[i];
        int colLeft = max(col - 1, 0);
        int colRight = min(col + 2, maxColumn);
        cout << "row: " << i << endl;
        smatch m;
        auto begin = rowLine.cbegin() + colLeft;
        auto end = rowLine.cbegin() + colRight;
        while (regex_search(begin, end, m, r)) {
            //get the full part num
            cout << m[0] << endl;
            int fullPart = getInnerNumber(i, colLeft + m.position());
            
            adjacentNumbers.push_back(fullPart);
            if (adjacentNumbers.size() > 2)
                return;
            
            begin += (m.position() + m.length());
            if (begin > end)
                begin = end;
        }
    }

    if (adjacentNumbers.size() == 2) {
        cout << "Adding: " << adjacentNumbers[0] * adjacentNumbers[1] << endl;
        gearRatioSum += adjacentNumbers[0] * adjacentNumbers[1];
    }
}


// gets a full number, from any position from within it
int getInnerNumber(int row, int col) {
    //cout << "Row " << row << " col " << col << " num as position: " << engineScheme[row][col];
    //cout << endl;
    regex r ("\\d+"); // for finding the full number
    regex r2 ("\\D"); // for finding a number

    // do a backwards search
    string line = engineScheme[row];
    for (int i = col; i >= 0; i--) {
        // if we hit a non digit, we have the start of our number
        smatch m;
        string oneChar = line.substr(i, 1);
        if (regex_match(oneChar, m, r2) || i == 0) {
            // found our number beginning
            smatch m2;
            regex_search(line.cbegin() + i, line.cend(), m2, r);
            // found our number
            int num = stoi(m2[0]);
            cout << "Found inner number: " << num << endl;
            return num;
        }       
    }
   
    // return 0 if something goes wrong
    return 0;
}

void testCase2() {
    engineScheme = {
    "11...2347.",
    "*.*..#..*1",
    "12.45.....",
    };
    maxColumn = engineScheme[0].size();

    // SHOULD be a gear
    evaluateGear(1, 0);
    cout << endl;
    
    //should NOT be a gear
    evaluateGear(1, 2);
    cout << endl;

    //should be a gear
    evaluateGear(1, 8);
    cout << endl;

    //gear ratio should be 2347
    cout << "Gear Ratio Sum " << gearRatioSum << endl;
}