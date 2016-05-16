#include <string>
#include <utility>
#include <list>
#include <vector>
#include <tuple>
#include <iostream>

using namespace std;

class SubstringGenerator
{
public:
        SubstringGenerator() = delete;
        SubstringGenerator(const string &rhs);

        /* Generates lexicographically next substring of mainString
         *
         * parameters:
         * outSubstring - generated substring is returned here
         *
         * return: true - substring generated, false - no more substrings
         */
        bool nextSubstring(string &outSubstring);

private:
        typedef pair<unsigned int, unsigned int> SubstringData;
        typedef list<SubstringData> SubstringDataList;
        typedef tuple<unsigned int, decltype(((SubstringDataList*)nullptr)->begin()),
                        int> SubstringPosition;

        void sortSubstringList(SubstringDataList &list);
        int getDiffPos();
        void setInitialPos();

        const string &mainString;
        vector<SubstringDataList> substrings;
        SubstringPosition currentPos;
};

SubstringGenerator::SubstringGenerator(const string &rhs) : mainString(rhs)
{
        substrings.resize('z' - 'a' + 1);
        for(unsigned int i = 0; i < mainString.size(); ++i) {
                substrings[mainString[i] - 'a'].emplace_back(i, 1);
        }

        setInitialPos();
}

bool SubstringGenerator::nextSubstring(string &outSubstring)
{
        bool ret = false;
        get<2>(currentPos)++;
        if (get<2>(currentPos) == 0) {
                ret = true;
        } else {
                if((int)(mainString.size() - get<2>(currentPos) - get<1>(currentPos)->first) < 0) {
                        get<1>(currentPos)++;
                        if(get<1>(currentPos) == substrings[get<0>(currentPos)].end()) {
                                do {
                                        get<0>(currentPos)++;
                                } while(substrings[get<0>(currentPos)].empty() &&
                                                get<0>(currentPos) != substrings.size());

                                if(get<0>(currentPos) == substrings.size()) {
                                        setInitialPos();
                                } else {
                                        sortSubstringList(substrings[get<0>(currentPos)]);
                                        get<1>(currentPos) = substrings[get<0>(currentPos)].begin();
                                        get<2>(currentPos) = 1;
                                        ret = true;
                                }
                        } else {
                                get<2>(currentPos) = getDiffPos();
                                ret = true;
                        }
                } else {
                        ret = true;
                }
        }

        if (ret) {
                outSubstring = mainString.substr(get<1>(currentPos)->first, get<2>(currentPos));
        }

        return ret;
}

void SubstringGenerator::sortSubstringList(SubstringDataList &list)
{
        list.sort([this](const SubstringData & lhs, const SubstringData & rhs)
                        {
                                return this->mainString.substr(lhs.first) < this->mainString.substr(rhs.first);
                        });
}

int SubstringGenerator::getDiffPos()
{
        auto prevIt = get<1>(currentPos);
        prevIt--;
        string prevString = mainString.substr(prevIt->first);
        string curString = mainString.substr(get<1>(currentPos)->first);

        int size = max(prevString.size(), curString.size());

        for (int i = 0; i < size; ++i) {
                if (prevString[i] != curString[i]) {
                        return i + 1;
                }
        }

        return 0;
}

void SubstringGenerator::setInitialPos()
{
        for(unsigned int i = 0; i < substrings.size(); ++i) {
                if(!substrings[i].empty()) {
                        sortSubstringList(substrings[i]);
                        currentPos = make_tuple(i, substrings[i].begin(), -1);
                        break;
                }
        }
}

typedef pair<string, string> Position;

bool isWinningPosition(const string & A, const string & B, const Position & pos)
{
    // TODO: add scenario with empty strings in pos
    // TODO: add scenario with the same substring several times in A or B
    const string & firstSubstring = pos.first;
    const string & secondSubstring = pos.second;

    // cout << "firstSubstring = " << firstSubstring << endl;
    // cout << "secondSubstring = " << secondSubstring << endl;

    bool oddFreeA = false;
    bool evenFreeA = false;
    bool oddFreeB = false;
    bool evenFreeB = false;

    string::size_type p = 0;
    string::size_type index = 0;

    while ((index = A.find(firstSubstring, p)) != string::npos) {
        int freeNumber = A.size() - (index + firstSubstring.size());
        bool even = freeNumber % 2 ? false : true;
        // cout << " index in A = " << index << endl;
        if (even)
            evenFreeA = true;
        else
            oddFreeA = true;
        p = index + 1;
        if (evenFreeA && oddFreeA)
            break;
    }

    // cout << "evenFreeA = " << evenFreeA << " oddFreeA = " << oddFreeA << endl;
    p = 0;

    while ((index = B.find(secondSubstring, p)) != string::npos) {
        int freeNumber = B.size() - (index + secondSubstring.size());
        bool even = freeNumber % 2 ? false : true;
        // cout << " index in B = " << index << endl;
        if (even)
            evenFreeB = true;
        else
            oddFreeB = true;
        p = index + 1;
        if (evenFreeB && oddFreeB)
            break;
    }

    // cout << "evenFreeB = " << evenFreeB << " oddFreeB = " << oddFreeB << endl;

    bool loosingPos = (oddFreeA && evenFreeA && oddFreeB && evenFreeB) ||
        (oddFreeA && oddFreeB && !evenFreeA && !evenFreeB) ||
        (evenFreeA && evenFreeB && !oddFreeA && !oddFreeB);

    // cout << "loosingPos = " << loosingPos << endl;

    return loosingPos ? false : true;
}

void printOutput(Position & pos, unsigned long long int k)
{
    // cout << "k = " << k << ", position = " << pos.first << " , " << pos.second << endl;
    cout << pos.first << endl;
    cout << pos.second << endl;
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */

    string tempString;
    getline(cin, tempString, ' ');
    unsigned int N = stoi(tempString);
    getline(cin, tempString, ' ');
    unsigned int M = stoi(tempString);
    getline(cin, tempString);
    unsigned long long int K = stoll(tempString);

    string A, B;
    getline(cin, A);
    getline(cin, B);

    SubstringGenerator generatorA(A);
    SubstringGenerator generatorB(B);

    Position position;
    unsigned long long int k = 0;

    string substringA;
    string substringB;
    while (generatorA.nextSubstring(substringA)) {
        while (generatorB.nextSubstring(substringB)) {
            position = make_pair(substringA, substringB);
            if (isWinningPosition(A, B, position)) {
                ++k;
                if (k == K) {
                    printOutput(position, k);
                    return 0;
                }
            }
        }
    }
    
    cout << "no solution" << endl;

    return 0;
}

