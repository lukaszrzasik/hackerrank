#include <iostream>
#include "substringGenerator.hpp"

using namespace std;


int main()
{
	string str = "acegabzbfy";
	SubstringGenerator generator(str);

	string outStr;
	string tempStr;
	while(generator.nextSubstring(outStr)) {
		cout << outStr << endl;
		getline(cin, tempStr);
	}
}
