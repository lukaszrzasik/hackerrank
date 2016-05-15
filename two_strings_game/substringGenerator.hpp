#include <string>
#include <utility>
#include <list>
#include <vector>
#include <tuple>

using namespace std;

class SubstringGenerator
{
public:
	SubstringGenerator() = delete;
	SubstringGenerator(const string &rhs);

	/* Generates lexicographically next substring of mainString
	 * 
	 * parameters:
	 * outSubstring - generated substring is returned here
	 *
	 * return: true - substring generated, false - no more substrings
	 */
	bool nextSubstring(string &outSubstring);	

private:
	typedef pair<unsigned int, unsigned int> SubstringData;
	typedef list<SubstringData> SubstringDataList;
	typedef tuple<unsigned int, decltype(((SubstringDataList*)nullptr)->begin()), 
			unsigned int> SubstringPosition;

	void sortSubstringList(SubstringDataList &list);
	int getDiffPos();

	const string &mainString;
	vector<SubstringDataList> substrings;
	SubstringPosition currentPos;
};
