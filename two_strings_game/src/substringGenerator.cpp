#include "substringGenerator.hpp"

SubstringGenerator::SubstringGenerator(const string &rhs) : mainString(rhs)
{
	substrings.resize('z' - 'a' + 1);
	for(unsigned int i = 0; i < mainString.size(); ++i) {
		substrings[mainString[i] - 'a'].emplace_back(i, 1);
	}

	setInitialPos();
}

bool SubstringGenerator::nextSubstring(string &outSubstring)
{
	bool ret = false;
	get<2>(currentPos)++;
	if (get<2>(currentPos) == 0) {
		ret = true;
	} else {
		if((int)(mainString.size() - get<2>(currentPos) - get<1>(currentPos)->first) < 0) {
			get<1>(currentPos)++;
			if(get<1>(currentPos) == substrings[get<0>(currentPos)].end()) {
				do {
					get<0>(currentPos)++;
				} while(substrings[get<0>(currentPos)].empty() && 
						get<0>(currentPos) != substrings.size());
				
				if(get<0>(currentPos) == substrings.size()) {
					setInitialPos();
				} else {
					sortSubstringList(substrings[get<0>(currentPos)]);
					get<1>(currentPos) = substrings[get<0>(currentPos)].begin();
					get<2>(currentPos) = 1;
					ret = true;
				}
			} else {
				get<2>(currentPos) = getDiffPos();
				ret = true;
			}
		} else {
			ret = true;
		}
	}

	if (ret) {
		outSubstring = mainString.substr(get<1>(currentPos)->first, get<2>(currentPos));
	}

	return ret;
}

void SubstringGenerator::sortSubstringList(SubstringDataList &list)
{
	list.sort([this](const SubstringData & lhs, const SubstringData & rhs) 
			{
				return this->mainString.substr(lhs.first) < this->mainString.substr(rhs.first);
			});
}

int SubstringGenerator::getDiffPos()
{
	auto prevIt = get<1>(currentPos);
	prevIt--;
	string prevString = mainString.substr(prevIt->first);
	string curString = mainString.substr(get<1>(currentPos)->first);

	int size = max(prevString.size(), curString.size());

	for (int i = 0; i < size; ++i) {
		if (prevString[i] != curString[i]) {
			return i + 1;
		}
	}

	return 0;
}

void SubstringGenerator::setInitialPos()
{
	for(unsigned int i = 0; i < substrings.size(); ++i) {
		if(!substrings[i].empty()) {
			sortSubstringList(substrings[i]);
			currentPos = make_tuple(i, substrings[i].begin(), -1);
			break;
		}
	}
}
