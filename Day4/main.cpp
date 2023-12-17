#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
#include <string>
#include <regex>
#include <unordered_set>
#include <vector>
using namespace std;

// template stuff
const string INPUT = "input.txt";
int inputColumns = 0;
ifstream file;

bool openFile(string path);
void closeFile();

// day 4
// processing input
unordered_set<int> getWinningNumSet(string input);
vector<int> getYourNums(string input);

// for calcing sum
int getCardScore(unordered_set<int> winningNumSet, vector<int> yourNums);
int getTotalScore(ifstream &file);

// for getting more cards
int getMatches(unordered_set<int> winningNumSet, vector<int> yourNums);
int getTotalScratchCards(ifstream &file);

// for testing
void testInputParsing();
void testCardScore();

auto main(int argc, char *argv[]) -> int {
    if (openFile(INPUT)) {
        //testInputParsing();
        //int totalScore = getTotalScore(file);
        //cout << "Part 1: Total Score is " << totalScore << endl;
        int totalCards = getTotalScratchCards(file);
        cout << "Part 2: Total Cards is " << totalCards << endl;
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

unordered_set<int> getWinningNumSet(string input) {
    // input looks like "41 48 83 86 17"
    regex r("\\s(\\d+)");
    smatch m;
    unordered_set<int> set(0);
    while(regex_search(input, m, r)) {
        int num = stoi(m[1]);
        // cout << num << " ";
        set.insert(num);
        input = m.suffix();
    }
    // cout << endl;

    return set;
}

vector<int> getYourNums(string input) {
    // input looks like "41 48 83 86 17"
    regex r("\\s(\\d+)");
    smatch m;
    vector<int> nums(0);
    while(regex_search(input, m, r)) {
        int num = stoi(m[1]);
        // cout << num << " ";
        nums.push_back(num);
        input = m.suffix();
    }
    // cout << endl;

    return nums;
}

void testInputParsing() {
    vector<string> input = {
        "Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53",
        "Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19"
    };

    regex r("Card \\d:([\\s|\\d]+) \\|([\\s|\\d]+)");
    smatch m;
    // card 1
    if (regex_match(input[0], m, r)) {
        // test win numbers
        unordered_set<int> winningNums = getWinningNumSet(m[1]);
        cout << "Win numbers test... " << endl;
        if (winningNums.count(41) && winningNums.count(17)) {
            cout << "Success!!" << endl;
        }
        else if (winningNums.count(100)) {
            cout << "Failure... 100 is not supposed to be there" << endl;
        }
        else {
            cout << "Failure... could not find 41 or 17" << endl;
        }
        // test get numbers
        vector<int> yourNums = getYourNums(m[2]);
        
        cout << "Your numbers test... " << endl;
        if (yourNums[0] == 83 && yourNums[7] == 53) {
            cout << "Success!!" << endl;
        }
        else if (yourNums[1] != 83) {
            cout << "Failure... 83 is not supposed to be the second element" << endl;
        }
        else {
            cout << "Failure... first or last element is incorrect" << endl;
        }
    }
}

int getCardScore(unordered_set<int> winningNumSet, vector<int> yourNums) {
    int score = 0;
    for (int x: yourNums) {
        if (winningNumSet.count(x)) {
            if (score == 0) {
                score = 1;
            }
            else {
                score *= 2;
            }
        }
    }

    return score;
}

int getMatches(unordered_set<int> winningNumSet, vector<int> yourNums) {
    int count = 0;
    for (int x: yourNums) {
        count += winningNumSet.count(x);
    }
    return count;
}

int getTotalScore(ifstream &file) {
    int totalScore = 0;
    if (file.is_open()) {
        string line;
        while(getline(file, line)) {
           cout << line << endl;
            regex r("Card\\s+\\d+:([\\s|\\d]+) \\|([\\s|\\d]+)");
            smatch m;
            // card
            if (regex_match(line, m, r)) {
                cout << "m!" << endl;
                // get winning numbers
                unordered_set<int> winningNums = getWinningNumSet(m[1]);
                // get your nums
                vector<int> yourNums = getYourNums(m[2]);
                // score
                totalScore += getCardScore(winningNums, yourNums);
            }
        }
    }
    return totalScore;
}

int getTotalScratchCards(ifstream &file) {
    int totalCards = 0;
    if (file.is_open()) {
        vector<int> cardCopies(1, 1);
        int i = 0;
        string line;
        while(getline(file, line)) {
            cout << "Card " << i+1 << " - Copies: " << cardCopies[i] << endl;
            regex r("Card\\s+\\d+:([\\s|\\d]+) \\|([\\s|\\d]+)");
            smatch m;
            // card
            if (regex_match(line, m, r)) {
                cout << "m!" << endl;
                unordered_set<int> winningNums = getWinningNumSet(m[1]);
                vector<int> yourNums = getYourNums(m[2]);
                int matches = getMatches(winningNums, yourNums);

                // run for each copy of CURRENT card
                for (int j = 0; j < cardCopies[i]; j++) {
                    cout << "Copy " << j + 1 << " out of " << cardCopies[i] << endl;
                    // create copies of FUTURE CARDS
                    for (int k = i + 1; k < i + 1 + matches; k++) {
                        //cout << "Create copy for card" << k << endl;
                        if (k >= cardCopies.size()) {
                            cardCopies.push_back(1);
                        }
                        cardCopies[k]++;
                    }
                    // count each card copy we run through
                    totalCards += 1;
                }
            }
            i++;
            if (i >= cardCopies.size()) {
                cardCopies.push_back(1);
            }

        }
    }
    return totalCards;
}
