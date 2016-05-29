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

        string operator[](unsigned int pos);

private:
        typedef unsigned int Position;
        typedef int MinLength;
        typedef int MaxLength;
        typedef int CurrentLength;
        typedef int SubstrNumber;
        typedef tuple<Position, MinLength, MaxLength> SubstringData;
        typedef pair<list<SubstringData>, SubstrNumber> SubstringDataList;
        typedef tuple<unsigned int, decltype(((SubstringDataList*)nullptr)->first.begin()),
                        CurrentLength> SubstringPosition;
        typedef vector<SubstringDataList> Substrings;

        void sortSubstringList(SubstringDataList &list);
        int getDiffPos();
        void setInitialPos();
        void sortFirst();
        void getMinLength();
        void getMaxLength();
        void setSubstrNumber();

        const string &mainString;
        Substrings substrings;
        SubstringPosition currentPos;
};

SubstringGenerator::SubstringGenerator(const string &rhs) : mainString(rhs)
{
        substrings.resize('z' - 'a' + 1);
        for(unsigned int i = 0; i < mainString.size(); ++i) {
                substrings[mainString[i] - 'a'].first.emplace_back(i, 1, 0);
                substrings[mainString[i] - 'a'].second = 0;
        }

        sortFirst();
        setInitialPos();
}

bool SubstringGenerator::nextSubstring(string &outSubstring)
{
        bool ret = false;
        get<2>(currentPos)++;
        if (get<2>(currentPos) == 0) {
                ret = true;
        } else {
                if((int)(mainString.size() - get<2>(currentPos) - get<0>(*get<1>(currentPos))) < 0) {
                        get<1>(currentPos)++;
                        if(get<1>(currentPos) == substrings[get<0>(currentPos)].first.end()) {
                                setSubstrNumber();
                                do {
                                        get<0>(currentPos)++;
                                } while(substrings[get<0>(currentPos)].first.empty() &&
                                                get<0>(currentPos) != substrings.size());

                                if(get<0>(currentPos) == substrings.size()) {
                                        setInitialPos();
                                } else {
                                        sortSubstringList(substrings[get<0>(currentPos)]);
                                        get<1>(currentPos) = substrings[get<0>(currentPos)].first.begin();
                                        getMinLength();
                                        getMaxLength();
                                        get<2>(currentPos) = 1;
                                        ret = true;
                                }
                        } else {
                                getMinLength();
                                getMaxLength();
                                get<2>(currentPos) = get<1>(*get<1>(currentPos));
                                ret = true;
                        }
                } else {
                        ret = true;
                }
        }

        if (ret) {
                outSubstring = mainString.substr(get<0>(*get<1>(currentPos)), get<2>(currentPos));
        }

        return ret;
}

void SubstringGenerator::sortSubstringList(SubstringDataList &list)
{
        list.first.sort([this](const SubstringData & lhs, const SubstringData & rhs)
                        {
                                return this->mainString.substr(get<0>(lhs)) < this->mainString.substr(get<0>(rhs));
                        });
}

int SubstringGenerator::getDiffPos()
{
        auto prevIt = get<1>(currentPos);
        if (prevIt == substrings[get<0>(currentPos)].first.begin()) {
                return 1;
        }
        prevIt--;
        string prevString = mainString.substr(get<0>(*prevIt));
        string curString = mainString.substr(get<0>(*get<1>(currentPos)));

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
                if(!substrings[i].first.empty()) {
                        currentPos = make_tuple(i, substrings[i].first.begin(), -1);
                        break;
                }
        }
}

void SubstringGenerator::sortFirst()
{
        for(unsigned int i = 0; i < substrings[i].first.size(); ++i) {
                if(!substrings[i].first.empty()) {
                        sortSubstringList(substrings[i]);
                        break;
                }
        }
}

void SubstringGenerator::getMinLength()
{
        get<1>(*get<1>(currentPos)) = getDiffPos();
}

void SubstringGenerator::getMaxLength()
{
        get<2>(*get<1>(currentPos)) = mainString.size() - get<0>(*get<1>(currentPos));
}

void SubstringGenerator::setSubstrNumber()
{
        int index = get<0>(currentPos);
        int sum = 0;
        for (auto element : substrings[index].first) {
                sum += get<2>(element) - get<1>(element) + 1;
        }
        substrings[index].second = sum;
}

string SubstringGenerator::operator[](unsigned int pos)
{
//        cout << __func__ << ": pos = " << pos << endl;

        if (pos == 0) {
                currentPos = make_tuple(0, substrings[0].first.begin(), 0);
                return "";
        }

        unsigned int substrNr = 1;

        for (unsigned int i = 0; i < substrings.size() - 1; ++i) {
                unsigned int substrWithLetterNr = 0;
                if (substrings[i].second > 0) {
                        substrWithLetterNr = substrings[i].second;
                        substrNr += substrWithLetterNr;
                }
                if (pos < substrNr || (!substrings[i].first.empty() && substrings[i].second == 0)) {
                        substrNr -= substrWithLetterNr;
                        for (auto it = substrings[i].first.begin(); it != substrings[i].first.end();
                                        ++it) {
                                unsigned int substrInListNr = 0;
                                if (get<2>(*it) == 0) {
                                        currentPos = make_tuple(i, it, 0);
                                        getMinLength();
                                        getMaxLength();
                                }

                                substrInListNr = get<2>(*it) - get<1>(*it) + 1;
                                substrNr += substrInListNr;

                                if (pos < substrNr) {
                                        substrNr -= substrInListNr;
                                        currentPos = make_tuple(i, it, get<1>(*it) + pos - substrNr);
//                                        cout << __func__ << ": currentPos = " << get<0>(currentPos) << ','
//                                                << get<2>(currentPos) << endl;
                                        return mainString.substr(get<0>(*it), get<1>(*it) + pos - substrNr);
                                }
                        }
                        setSubstrNumber();
                }
        }

        return "Not implemented yet";
}

typedef pair<string, string> Position;

tuple<int,int,bool> isWinningPosition(const string & A, const string & B, const Position & pos)
{
    // TODO: add scenario with empty strings in pos
    // TODO: add scenario with the same substring several times in A or B
    const string & firstSubstring = pos.first;
    const string & secondSubstring = pos.second;

//        cout << "firstSubstring = " << firstSubstring << endl;
//        cout << "secondSubstring = " << secondSubstring << endl;

        bool oddFreeA = false;
        bool evenFreeA = false;
        bool oddFreeB = false;
        bool evenFreeB = false;

        string::size_type p = 0;
        string::size_type index = 0;

        while ((index = A.find(firstSubstring, p)) != string::npos) {
                int freeNumber = A.size() - (index + firstSubstring.size());
                bool even = freeNumber % 2 ? false : true;
//                cout << " index in A = " << index << endl;
                if (even)
                        evenFreeA = true;
                else
                        oddFreeA = true;
                p = index + 1;
                if (evenFreeA && oddFreeA)
                        break;
        }

//        cout << "evenFreeA = " << evenFreeA << " oddFreeA = " << oddFreeA << endl;
        p = 0;
        int freeNumberB;
        while ((index = B.find(secondSubstring, p)) != string::npos) {
                freeNumberB = B.size() - (index + secondSubstring.size());
                bool even = freeNumberB % 2 ? false : true;
//                cout << " index in B = " << index << endl;
                if (even)
                        evenFreeB = true;
                else
                        oddFreeB = true;
                p = index + 1;
                if (evenFreeB && oddFreeB)
                        break;
        }

//        cout << "evenFreeB = " << evenFreeB << " oddFreeB = " << oddFreeB << endl;

        bool isABoth = oddFreeA && evenFreeA;
        bool isBBoth = oddFreeB && evenFreeB;

        if (isABoth && isBBoth) {
//                cout << "isABoth && isBBoth" << endl;
                return make_tuple(0, 0, false);
        } else if (isABoth) {
//                cout << "isABoth" << endl;
                return make_tuple(freeNumberB + 1, freeNumberB, true);
        } else if (isBBoth) {
                // TODO: this can be optimized
//                cout << "isBBoth" << endl;
                return make_tuple(1, 0, true);
        } else if (evenFreeA && oddFreeB) {
//                cout << "evenFreeA && oddFreeB" << endl;
                return make_tuple(freeNumberB/2 + 1, freeNumberB, true);
        } else if (oddFreeA && oddFreeB) {
//                cout << "oddFreeA && oddFreeB" << endl;
                return make_tuple(freeNumberB/2 + 1, freeNumberB, false);
        } else if (evenFreeA && evenFreeB) {
//                cout << "evenFreeA && evenFreeB" << endl;
                return make_tuple(freeNumberB/2, freeNumberB, false);
        } else {
//                cout << "else: oddFreeA && evenFreeB" << endl;
                return make_tuple(freeNumberB/2 + 1, freeNumberB, true);
        }

//        cout << "Something is wrong, should not be here" << endl;
        return make_tuple(0, 0, false);
}

void printOutput(Position & pos, unsigned long long int k)
{
//        cout << "k = " << k << ", position = " << pos.first << " , " << pos.second << endl;
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
        unsigned int a = 0;
        unsigned int b = 0;

//        cout << "K = " << K << endl;

        string substringA;
        string substringB;
        while (generatorA.nextSubstring(substringA)) {
                while (generatorB.nextSubstring(substringB)) {
//                        cout << "a = " << a << endl;
//                        cout << "b = " << b << endl;
//                        cout << "k = " << k << endl;
                        position = make_pair(substringA, substringB);
                        auto newPosition = isWinningPosition(A, B, position);
//                        cout << "newPosition = " << get<0>(newPosition) << ',' << get<1>(newPosition) << endl;
                        if (k + get<0>(newPosition) >= K) {
//                                cout << "k + get<0>(newPosition) >= K" << endl;
                                int offset = K - k;
                                k = K;
                                if (get<0>(newPosition) != get<1>(newPosition) + 1) {
                                        if (get<2>(newPosition) == true) {
                                                offset = offset * 2 - 1;
                                        } else {
                                                offset = offset * 2;
                                        }
                                }
//                                cout << "offset = " << offset << endl;
                                substringB = generatorB[b + offset - 1];
                                position = make_pair(substringA, substringB);
                                printOutput(position, k);
                                return 0;
                        } else {
                                k += get<0>(newPosition);
                                b += get<1>(newPosition);
                                generatorB[b];
                        }
                        ++b;
//                        cout << endl;
                }
                b = 0;
                ++a;
        }

    return 0;
}

