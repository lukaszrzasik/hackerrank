#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

#include "substringGenerator.hpp"

typedef pair<string, string> Position;

bool isWinningPosition(const string & A, const string & B, const Position & pos)
{
    // TODO: add scenario with empty strings in pos
    // TODO: add scenario with the same substring several times in A or B
    const string & firstSubstring = pos.first;
    const string & secondSubstring = pos.second;

	cout << "firstSubstring = " << firstSubstring << endl;
	cout << "secondSubstring = " << secondSubstring << endl;

	bool oddFreeA = false;
	bool evenFreeA = false;
	bool oddFreeB = false;
	bool evenFreeB = false;

	unsigned int p = 0;

	while (int index = A.find(firstSubstring, p) != string::npos) {
		int freeNumber = A.size() - (index + firstSubstring.size());
		bool even = freeNumber % 2 ? false : true;
		if (even)
			evenFreeA = true;
		else 
			oddFreeA = true;
		++p;
		if (evenFreeA && oddFreeA)
			break;
	}

 	p = 0;

	while (int index = B.find(secondSubstring, p) != string::npos) {
		int freeNumber = B.size() - (index + secondSubstring.size());
		bool even = freeNumber % 2 ? false : true;
		if (even)
			evenFreeB = true;
		else 
			oddFreeB = true;
		++p;
		if (evenFreeB && oddFreeB)
			break;
	}
	
	bool loosingPos = (oddFreeA && evenFreeA && oddFreeB && evenFreeB) || 
		(oddFreeA && oddFreeB && !evenFreeA && !evenFreeB) ||
		(evenFreeA && evenFreeB && !oddFreeA && !oddFreeB);

	cout << "loosingPos = " << loosingPos << endl;
   
	return loosingPos ? false : true;
}

void printOutput(Position & pos, int k)
{
	cout << "k = " << k << ", position = " << pos.first << " , " << pos.second << endl; 
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    
    string tempString;
    getline(cin, tempString, ' ');
	int N = stoi(tempString);
    getline(cin, tempString, ' ');
    int M = stoi(tempString);
    getline(cin, tempString);
    int K = stoi(tempString);
    
    string A, B;
    getline(cin, A);
    getline(cin, B);

	SubstringGenerator generatorA(A);
	SubstringGenerator generatorB(B);
    
    Position position;
	int k = 0;

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
    
    return 0;
}
