#include <iostream>
#include "substringGenerator.hpp"

using namespace std;


int main()
{
	string str = "cd";
	SubstringGenerator generator(str);

	string outStr;
	string tempStr;
	while(generator.nextSubstring(outStr)) {
		cout << outStr << endl;
		getline(cin, tempStr);
	}
}
