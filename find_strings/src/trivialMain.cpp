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

using namespace std;

/* Head ends here */
void findStrings (vector <string>a, vector <int> a_query) {
	// cout << "Input strings:" << endl;

    set<string> strings;
    for (string& s : a)
    {
		// cout << s << endl;

		for (unsigned long i = 0; i < s.size(); ++i)
		{
			for (unsigned long j = 1; i + j <= s.size(); ++j)
			{
				strings.emplace(s.substr(i,j));
			}
		}
    }

	// cout << endl << "All strings combined:" << endl;
	// int no = 1;
//	for (const string& s : strings)
//	{
		// cout << no++ << ": " << s << endl;
//	}
	
	// cout << endl << "Queries:" << endl;

    for (int q : a_query) 
    {
		// cout << q << endl;

		auto it = strings.cbegin();
		while(--q>0)
		{
			if (++it == strings.cend())
				break;
		}

		if (it != strings.cend())
		{
			cout << *it << endl;
		}
		else 
		{
			cout << "INVALID" << endl;
		}
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
