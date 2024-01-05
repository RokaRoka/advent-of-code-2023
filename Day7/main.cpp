#include <cstdio>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

const int HAND_SIZE = 5;

enum Card {
TWO = 2,
THREE,
FOUR,
FIVE,
SIX,
SEVEN,
EIGHT,
NINE,
T,
J,
Q,
K,
A
};

enum HandType {
HIGH_CARD,
ONE_PAIR,
TWO_PAIR,
THREE_OF_A_KIND,
FULL_HOUSE,
FOUR_OF_A_KIND,
FIVE_OF_A_KIND
};

struct CardHand {
    Card cards[5];
    HandType handType;
    long bid;
} sampleCardHand[5] {
{ { THREE, TWO, T, THREE, K }, ONE_PAIR, 765 },

{ { TWO, TWO, TWO, THREE, K }, THREE_OF_A_KIND, 765 },

{ { THREE, TWO, THREE, THREE, TWO }, FULL_HOUSE, 765 },

{ { TWO, A, FOUR, THREE, K }, HIGH_CARD, 765 },

{ { TWO, SEVEN, SEVEN, THREE, K }, ONE_PAIR, 765 },
};

using GameHand = pair<CardHand, long>;

bool compareCardHand(const CardHand &a, const CardHand &b);
long getTotalWinnings(const vector<GameHand> &hands);
void insertionSortCardHand(vector<CardHand> &hands);
void testSortFiveHands();

auto main(int argc, char *argv[]) -> int {
    testSortFiveHands();
    return 0;
}

// to save time, we sort hands into card rank as we parse them. Might as well!

// test if a is bigger than b
bool compareCardHand(const CardHand &a, const CardHand &b) {
    // compare hand type
    if (a.handType > b.handType) {
        return false;
    }
    else if (b.handType > a.handType) {
        return false;
    }
    else {
        // compare card rank
        for (int i = 0; i < HAND_SIZE; i++) {
            if (a.cards[i] > b.cards[i]) {
                return true;
            }
            else if (b.cards[i] > a.cards[i]) {
                return false;
            }
        }
    }

    // If the hands are exactly the same, return a :shrug:
    return true;
}

long getTotalWinnings(const vector<GameHand> &hands) {
    long winnings = 0;
    // insertion Sort hand/bids based on hand score. We got too much data

    return winnings;
}


void insertionSortCardHand(vector<CardHand> &hands) {
    int i, j;
    CardHand key;
    for (i = 1; i < hands.size(); i++) {
        key = hands[i];
        j = i - 1;

        // Move elements of arr[0..i-1],
        // that are greater than key,
        // to one position ahead of their
        // current position
        while (j >= 0 && compareCardHand(hands[j], key)) {
            hands[j + 1] = hands[j];
            j = j - 1;
        }
        hands[j + 1] = key;
    }
}

void testSortFiveHands() {
    vector<CardHand> hands(0);
    for (int i = 0; i < 5; i++) {
        hands.push_back(sampleCardHand[i]);
    }
    insertionSortCardHand(hands);

    long i = 1;
    for (CardHand hand: hands) {
        printf("Rank: %li - Hand: %s\n", i, "");
        i++;
    }
}
