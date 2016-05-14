#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

typedef pair<string, string> Position;

bool isWinningPosition(string A, string B, const Position& pos)
{
    // TODO: add scenario with empty strings in pos
    // TODO: add scenario with the same substring several times in A or B
    string firstSubstring = pos.first;
    string secondSubstring = pos.second;
    
    int index = A.find(firstSubstring);
    if(index == npos)
    {
        cout << "isWinningPosition: invalid situation, A = " << A << " firstSubstring = "
            << firstSubstring << endl;
        return false;
    }

    index += firstSubstring.length();
    int numberFreeInA = A.length() - index;
    
    index = B.find(secondSubstring);
    if(index == npos)
    {
        cout << "isWinningPosition: invalid situation, B = " << B << " secondSubstring = "
            << secondSubstring << endl;
        return false;
    }
    index += secondSubstring.length();
    int numberFreeInB = B.length() - index;
    
    if((numberFreeInA + numberFreeInB) % 2 == 0) {
        return false;
    } else {
        return true;
    }
}

void findNextPosition(string A, string B, Position& pos) 
{
    if (pos.first == "") {
        if (pos.second == "") {
            for (char c = 'a'; c < 'z' + 1; ++c) {
                if (npos != B.find(c)) {
                    pos.second = c;
                }
            }
        } else {
            
        }
    }
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
    
    Position position("", "");
    
    return 0;
}

