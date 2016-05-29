#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

#include "substringGenerator.hpp"

typedef pair<string, string> Position;

tuple<int,int,bool> isWinningPosition(const string & A, const string & B, const Position & pos)
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

	string::size_type p = 0;
	string::size_type index = 0;

	while ((index = A.find(firstSubstring, p)) != string::npos) {
		int freeNumber = A.size() - (index + firstSubstring.size());
		bool even = freeNumber % 2 ? false : true;
		cout << " index in A = " << index << endl;
		if (even)
			evenFreeA = true;
		else 
			oddFreeA = true;
		p = index + 1;
		if (evenFreeA && oddFreeA)
			break;
	}

	cout << "evenFreeA = " << evenFreeA << " oddFreeA = " << oddFreeA << endl;
 	p = 0;
	int freeNumberB;
	while ((index = B.find(secondSubstring, p)) != string::npos) {
		freeNumberB = B.size() - (index + secondSubstring.size());
		bool even = freeNumberB % 2 ? false : true;
		cout << " index in B = " << index << endl;
		if (even)
			evenFreeB = true;
		else 
			oddFreeB = true;
		p = index + 1;
		if (evenFreeB && oddFreeB)
			break;
	}
	
	cout << "evenFreeB = " << evenFreeB << " oddFreeB = " << oddFreeB << endl;

	bool isABoth = oddFreeA && evenFreeA;
	bool isBBoth = oddFreeB && evenFreeB;

	if (isABoth && isBBoth) {
		cout << "isABoth && isBBoth" << endl;
		return make_tuple(0, 0, false);
	} else if (isABoth) {
		cout << "isABoth" << endl;
		return make_tuple(freeNumberB + 1, freeNumberB, true);
	} else if (isBBoth) {
		// TODO: this can be optimized
		cout << "isBBoth" << endl;
		return make_tuple(1, 0, true);
	} else if (evenFreeA && oddFreeB) {
		cout << "evenFreeA && oddFreeB" << endl;
		return make_tuple(freeNumberB/2 + 1, freeNumberB, true);
	} else if (oddFreeA && oddFreeB) {
		cout << "oddFreeA && oddFreeB" << endl;
		return make_tuple(freeNumberB/2 + 1, freeNumberB, false);
	} else if (evenFreeA && evenFreeB) {
		cout << "evenFreeA && evenFreeB" << endl;
		return make_tuple(freeNumberB/2, freeNumberB, false);
	} else {
		cout << "else: oddFreeA && evenFreeB" << endl;
		return make_tuple(freeNumberB/2 + 1, freeNumberB, true);
	}

	cout << "Something is wrong, should not be here" << endl;
	return make_tuple(0, 0, false);
}

void printOutput(Position & pos, unsigned long long int k)
{
	cout << "k = " << k << ", position = " << pos.first << " , " << pos.second << endl; 
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

	cout << "K = " << K << endl;

	string substringA;
	string substringB;
	while (generatorA.nextSubstring(substringA)) {
		while (generatorB.nextSubstring(substringB)) {
			cout << "a = " << a << endl;
			cout << "b = " << b << endl;
			cout << "k = " << k << endl;
			position = make_pair(substringA, substringB);
			auto newPosition = isWinningPosition(A, B, position);
			cout << "newPosition = " << get<0>(newPosition) << ',' << get<1>(newPosition) << endl; 
			if (k + get<0>(newPosition) >= K) {
				cout << "k + get<0>(newPosition) >= K" << endl;
				int offset = K - k;
				k = K;
				if (get<0>(newPosition) != get<1>(newPosition) + 1) {
					if (get<2>(newPosition) == true) {
						offset = offset * 2 - 1;
					} else {
						offset = offset * 2;
					}
				}
				cout << "offset = " << offset << endl;
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
			cout << endl;
		}
		b = 0;
		++a;
	}

    return 0;
}
