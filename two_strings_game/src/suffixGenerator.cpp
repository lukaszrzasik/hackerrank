#include <iostream>
#include "substringGenerator.hpp"

SubstringGenerator::SubstringGenerator(const string &rhs) : mainString(rhs)
{
	substrings.resize('z' - 'a' + 1);
	for(unsigned int i = 0; i < mainString.size(); ++i) {
		substrings[mainString[i] - 'a'].first.emplace_back(i, 1, 0);
		substrings[mainString[i] - 'a'].second = 0;
	}

	sortFirst();
	setInitialPos();
}

bool SubstringGenerator::nextSubstring(string &outSubstring)
{
	bool ret = false;
	get<2>(currentPos)++;
	if (get<2>(currentPos) == 0) {
		ret = true;
	} else {
		if((int)(mainString.size() - get<2>(currentPos) - get<0>(*get<1>(currentPos))) < 0) {
			get<1>(currentPos)++;
			if(get<1>(currentPos) == substrings[get<0>(currentPos)].first.end()) {
				setSubstrNumber();
				do {
					get<0>(currentPos)++;
				} while(substrings[get<0>(currentPos)].first.empty() && 
						get<0>(currentPos) != substrings.size());
				
				if(get<0>(currentPos) == substrings.size()) {
					setInitialPos();
				} else {
					sortSubstringList(substrings[get<0>(currentPos)]);
					get<1>(currentPos) = substrings[get<0>(currentPos)].first.begin();
					getMinLength();
					getMaxLength();
					get<2>(currentPos) = 1;
					ret = true;
				}
			} else {
				getMinLength();
				getMaxLength();
				get<2>(currentPos) = get<1>(*get<1>(currentPos));
				ret = true;
			}
		} else {
			ret = true;
		}
	}

	if (ret) {
		outSubstring = mainString.substr(get<0>(*get<1>(currentPos)), get<2>(currentPos));
	}

	return ret;
}

void SubstringGenerator::sortSubstringList(SubstringDataList &list)
{
	list.first.sort([this](const SubstringData & lhs, const SubstringData & rhs) 
			{
				return this->mainString.substr(get<0>(lhs)) < this->mainString.substr(get<0>(rhs));
			});
}

int SubstringGenerator::getDiffPos()
{
	auto prevIt = get<1>(currentPos);
	if (prevIt == substrings[get<0>(currentPos)].first.begin()) {
		return 1;
	}
	prevIt--;
	string prevString = mainString.substr(get<0>(*prevIt));
	string curString = mainString.substr(get<0>(*get<1>(currentPos)));

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
		if(!substrings[i].first.empty()) {
			currentPos = make_tuple(i, substrings[i].first.begin(), -1);
			break;
		}
	}
}

void SubstringGenerator::sortFirst()
{
	for(unsigned int i = 0; i < substrings[i].first.size(); ++i) {
		if(!substrings[i].first.empty()) {
			sortSubstringList(substrings[i]);
			break;
		}
	}
}

void SubstringGenerator::getMinLength()
{
	get<1>(*get<1>(currentPos)) = getDiffPos();
}

void SubstringGenerator::getMaxLength()
{
	get<2>(*get<1>(currentPos)) = mainString.size() - get<0>(*get<1>(currentPos));
}

void SubstringGenerator::setSubstrNumber()
{	
	int index = get<0>(currentPos);
	int sum = 0;
	for (auto element : substrings[index].first) {
		sum += get<2>(element) - get<1>(element) + 1;
	}	
	substrings[index].second = sum;
}

string SubstringGenerator::operator[](unsigned int pos)
{
	cout << __func__ << ": pos = " << pos << endl;

	if (pos == 0) {
		currentPos = make_tuple(0, substrings[0].first.begin(), 0);
		return "";
	}

	unsigned int substrNr = 1;

	for (unsigned int i = 0; i < substrings.size() - 1; ++i) {
		unsigned int substrWithLetterNr = 0;
		if (substrings[i].second > 0) {
			substrWithLetterNr = substrings[i].second;
			substrNr += substrWithLetterNr;
		}
		if (pos < substrNr || (!substrings[i].first.empty() && substrings[i].second == 0)) {
			substrNr -= substrWithLetterNr;
			for (auto it = substrings[i].first.begin(); it != substrings[i].first.end();
					++it) {
				unsigned int substrInListNr = 0;
				if (get<2>(*it) == 0) {
					currentPos = make_tuple(i, it, 0);
					getMinLength();
					getMaxLength();
				}

				substrInListNr = get<2>(*it) - get<1>(*it) + 1;
				substrNr += substrInListNr;

				if (pos < substrNr) {
					substrNr -= substrInListNr;
					currentPos = make_tuple(i, it, get<1>(*it) + pos - substrNr);
					cout << __func__ << ": currentPos = " << get<0>(currentPos) << ',' 
						<< get<2>(currentPos) << endl;
					return mainString.substr(get<0>(*it), get<1>(*it) + pos - substrNr);
				}
			}
			setSubstrNumber();
		}
	}

	return "Not implemented yet";
}
