#include <string>
#include <tuple>
#include <list>
#include <vector>
using namespace std;

class SubstringGenerator
{
public:
	SubstringGenerator() = delete;
	SubstringGenerator(const string &rhs) : mainString(rhs) {}
	bool nextSubstring(string &outSubstring);	

private:
	typedef list<tuple<unsigned int, unsigned int, bool>> substringData;

	string &mainString;
	vector<substringData> substrings;
};
