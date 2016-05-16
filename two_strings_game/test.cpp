#include <iostream>
#include "substringGenerator.hpp"

using namespace std;


int main()
{
	string str = "cd";
	SubstringGenerator generator(str);

	cout << "Given string = " << str << endl;

	string outStr;
	string tempStr;
	while(generator.nextSubstring(outStr)) {
		cout << outStr << endl;
		getline(cin, tempStr);
	}
}
