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
	typedef unsigned int Position;
	typedef int MinLength;
	typedef int MaxLength;
	typedef int CurrentLength;
	typedef int SubstrNumber;
	typedef tuple<Position, MinLength, MaxLength> SubstringData;
	typedef pair<list<SubstringData>, SubstrNumber> SubstringDataList;
	typedef tuple<unsigned int, decltype(((SubstringDataList*)nullptr)->first.begin()), 
			CurrentLength> SubstringPosition;
	typedef vector<SubstringDataList> Substrings;

	void sortSubstringList(SubstringDataList &list);
	int getDiffPos();
	void setInitialPos();
	void sortFirst();
	void getMinLength();
	void getMaxLength();
	void setSubstrNumber();

	const string &mainString;
	Substrings substrings;
	SubstringPosition currentPos;
};
