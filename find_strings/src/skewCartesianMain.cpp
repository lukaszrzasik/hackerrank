#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <cstdlib>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <map>

#include "stringerCartesian.hpp"

using namespace std;

/* Head ends here */
void findStrings (vector<string> a, vector<int> a_query) {
	Stringer stringer;

	for (const string& s : a) {
		stringer.addString(s);
	}

	for (const int& k : a_query) {
		cout << stringer[k] << endl;
	}
}

/* Tail starts here */
int main() {
    int res;
    
    int _cases,_query, _a_i;
    scanf("%d", &_cases);
    vector <string> _a;
    char _a_item[2001];
    for (_a_i = 0; _a_i<_cases; _a_i++) {
        scanf("%s",_a_item);
        _a.push_back(_a_item);
    }
    vector <int> _a_query;
    scanf("%d", &_query);
    int _q_item;
    for(_a_i = 0; _a_i < _query; _a_i++) { 
        scanf("%d", &_q_item);
        _a_query.push_back(_q_item);
    }
    findStrings(_a, _a_query);    
    return 0;
}
