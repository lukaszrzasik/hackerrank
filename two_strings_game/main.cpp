#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

typedef pair<string, string> Position;

bool isWinningPosition(const string & A, const string & B, const Position & pos)
{
    // TODO: add scenario with empty strings in pos
    // TODO: add scenario with the same substring several times in A or B
    string & firstSubstring = pos.first;
    string & secondSubstring = pos.second;

	bool oddFreeA = false;
	bool evenFreeA = false;
	bool oddFreeB = false;
	bool evenFreeB = false;

	unsigned int pos = 0;

	while (int index = A.find(firstSubstring, pos) != npos) {
		int freeNumber = A.size() - (index + firstSubstring.size());
		bool even = freeNumber % 2 ? false : true;
		if (even)
			evenFreeA = true;
		else 
			oddFreeA = true;
		++pos;
		if (evenFreeA && oddFreeA)
			break;
	}

 	pos = 0;

	while (int index = B.find(secondSubstring, pos) != npos) {
		int freeNumber = B.size() - (index + secondSubstring.size());
		bool even = freeNumber % 2 ? false : true;
		if (even)
			evenFreeB = true;
		else 
			oddFreeB = true;
		++pos;
		if (evenFreeB && oddFreeB)
			break;
	}
	
	bool loosingPos = (oddFreeA && evenFreeA && oddFreeB && evenFreeB) || 
		(oddFreeA && oddFreeB && !evenFreeA && !evenFreeB) ||
		(evenFreeA && evenFreeB && !oddFreeA && !oddFreeB);
   
	return loosingPos ? false : true;
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
    
    Position position("", "");
	int k = 0;
	while (k != K) {
	}
    
    return 0;
}
