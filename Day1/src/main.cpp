#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <regex>
using namespace std;

const string DAY1_INPUT = "input.txt";

// vars
ifstream file;

// functions
bool openFile(string path);
void closeFile();
void getCalibrationsSum();
int getCalibrationValue(string line);
string unspellDigit(string digit);

auto main(int argc, char *argv[]) -> int {
    if (openFile(DAY1_INPUT)) {
        getCalibrationsSum();
        closeFile();
    }
    return 0;
}

// Goal: find the calibration value from the first + last value of each line on the doc
// To do this:
// - open/close file
// - read file to sum all calibration values
// - read line of file to process line and return two-digit

bool openFile(string path) {
    file = ifstream(path);
    return (file.is_open());
}

void closeFile() {
    file.close();
}

void getCalibrationsSum() {
    string line = "";
    int sum = 0;
    //getline(file, line);
    //cout << line << endl;
    //sum = getCalibrationValue(line);
    while (!getline(file, line).eof()) {
        // cout << "Line: " << line << endl;
        int value = getCalibrationValue(line);
        sum += value;
        cout << "Value: " << value << endl;
    }
    cout << "Sum is " << sum << endl;
}

int getCalibrationValue(string line) {
    int value = 0;
    string sValue = "";
    // regex definitions
    smatch m;
    regex e("(?:[1-9])|(?:one)|(?:two)|(?:three)|(?:four)|(?:five)|(?:six)|(?:seven)|(?:eight)|(nine)");

    // search line
    while (regex_search(line.cbegin(), line.cend(), m, e, regex_constants::match_any | regex_constants::match_default)) {
        //for (auto x:m) {
            auto x = m[0];
            cout << x << " ";
            string toAdd = x;
            if (toAdd.size() > 1)
                toAdd = unspellDigit(toAdd);
            sValue.append(toAdd);
            line = line.substr(m.position() + 1);
        //}
        cout << endl;
    }
       

    // cast value into int
    if (sValue.size() > 0) {
        sValue = string({sValue.front(), sValue.back()});
    }
    value = stoi(sValue);
    return value;
}

string unspellDigit(string digit) {
    if (digit == "one") return "1";
    else if (digit == "two") return "2";
    else if (digit == "three") return "3";
    else if (digit == "four") return "4";
    else if (digit == "five") return "5";
    else if (digit == "six") return "6";
    else if (digit == "seven") return "7";
    else if (digit == "eight") return "8";
    else if (digit == "nine") return "9";
    else return "0";
}
