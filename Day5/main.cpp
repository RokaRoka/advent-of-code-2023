#include <iostream>
#include <fstream>
#include <iterator>
#include <ostream>
#include <string>
#include <regex>
#include <unordered_map>
#include <vector>
using namespace std;

// template stuff
const string INPUT = "input.txt";
int inputColumns = 0;
ifstream file;

bool openFile(string path);
void closeFile();

// day 5
struct maprange_t {
    long destStart;
    long srcStart;
    long length;
};

class RangeMap {
private:
        vector<maprange_t> _ranges;
public:
        RangeMap();
        RangeMap(vector<maprange_t> ranges);
        long get(long source);
        void setRanges(vector<maprange_t> ranges);
};

// contains maps for "seed-to-soil", "soil-to-fertilizer", etc
unordered_map<string, RangeMap> almanacMaps;

// contains keys that give the detination of any given source
const unordered_map<string, string> destinationMapLookup {
    {"seed", "soil"},
    {"soil", "fertilizer"},
    {"fertilizer", "water"},
    {"water", "light"},
    {"light", "temperature"},
    {"temperature", "humidity"},
    {"humidity", "location"}
};

// adds a map to the almanac maps
void parseMap(string name, string rangesInput);
long convertSrcToDest(string dest, string src, long srcId);

void testMapParsing();
void testSeedToWater();

auto main(int argc, char *argv[]) -> int {
    if (openFile(INPUT)) {
        string inputLine;

        // separate seed ids and map
        regex r("seeds:(.+)");
        smatch m;
        getline(file, inputLine);
        if (regex_match(inputLine, m, r)) {
            string numsStr = m[1];
            regex r2("\\s(\\d+)");
            smatch m2;
            vector<long> seeds (0);
            while (regex_search(numsStr, m2, r2)) {
                seeds.push_back(stol(m2[1]));
                numsStr = m2.suffix();
            }

            regex r3("(\\S+)\\smap:");
            regex r4 ("\\d+\\s\\d+\\s\\d+");
            string mapName = "";
            while (getline(file, inputLine)) {
                cout << inputLine << endl;
                // check if we hit a map name
                smatch m3;
                if (regex_match(inputLine, m3, r3)) {
                    string allMapInput = "";
                    smatch m4;
                    string mapInput;
                    getline(file, mapInput);
                    while (regex_match(mapInput, m4, r4)) {
                        allMapInput.append(m4[0]);
                        allMapInput.append("\n");
                        getline(file, mapInput);
                    }
                    cout << allMapInput << endl;
                    parseMap(m3[1], allMapInput);
                }
            }

            // convert each seed to a location
            long lowest = 0;
            for (long seed: seeds) {
                long loc = convertSrcToDest("location", "seed", seed);
                if (loc < lowest || lowest == 0) {
                    lowest = loc;
                }
            }
            cout << "Lowest location is " << lowest << endl;
        }
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

RangeMap::RangeMap() {
    _ranges = vector<maprange_t>(0);
}

RangeMap::RangeMap(vector<maprange_t> ranges) {
    _ranges = vector<maprange_t>(ranges);
}

long RangeMap::get(long source) {
    for (maprange_t range:_ranges) {
        if (source >= range.srcStart && source < range.srcStart + range.length) {
            //cout << "Found!" << endl;
            return (source - range.srcStart) + range.destStart;
        }
    }
    return source;
}

void parseMap(const string name, string rangesInput) {
    regex r ("(\\d+)\\s(\\d+)\\s(\\d+)");
    smatch m;
    vector<maprange_t> mapRanges(0);
    while( regex_search(rangesInput, m, r) ) {
        // idxs:
        // 1 - destination
        // 2 - source
        // 3 - range length

        mapRanges.push_back({stol(m[1]), stol(m[2]), stol(m[3])});
        rangesInput = m.suffix();
    }

    almanacMaps.emplace(name, mapRanges);
}

// assumes we are searching forward towards our dest
long convertSrcToDest(const string dest, const string src, long srcId) {
    //cout << "Dest: " << dest << " Src: " << src << " SrcId: " << srcId << endl;
    if (dest == src)
        return srcId;

    // get the correct map
    string next = destinationMapLookup.at(src);
    RangeMap map = almanacMaps[src + "-to-" + next];
    return convertSrcToDest(dest, next, map.get(srcId));
}

void testMapParsing() {
    string name = "seed-to-soil";
    string rangeInput = "\
    440744287 1133551978 536306564\
    52 50 48";

    parseMap(name, rangeInput);
    cout << "Almanac map keys: " << almanacMaps.begin()->first << endl;
    cout << "Get should return 440744287. Returns: " << almanacMaps["seed-to-soil"].get(1133551978) << endl;
}

void testSeedToWater() {
    string name = "seed-to-soil";
    string rangeInput = "\
    50 98 2\
    52 50 48";
    parseMap(name, rangeInput);

    string name2 = "soil-to-fertilizer";
    string rangeInput2 = "\
    0 15 37\
    37 52 2\
    39 0 15";
    parseMap(name2, rangeInput2);

    string name3 = "fertilizer-to-water";
    string rangeInput3 = "\
    49 53 8\
    0 11 42\
    42 0 7\
    57 7 4";
    parseMap(name3, rangeInput3);

    long seed = 79;
    cout << "Searching for seed 14 to water id" << endl;
    long waterId = convertSrcToDest("water", "seed", 14);
    cout << "Answer is 49. Found: " << waterId << endl;
}
