#include "substrSorter.h"

#include <iostream>
#include <cmath>

namespace {
	const char a = 97 - 1; // 'a' ASCII code minus one (word seperator)
	const char engAlphSize = 26 + 1; // +1 for word separator
}

void SubstrSorter::passAndSort(const std::vector<std::string>& strings)
{
	combineStrings(strings);
	normalizeCombinedStrings();
	divideCombinedStrings();
	radixSort(s12, engAlphSize, 3);

	if (encodeS12()) {
		translateS12();
	}

	decodeS12();

	revertNormalizedCombinedStrings();
}

void SubstrSorter::combineStrings(const std::vector<std::string>& strings)
{
	combinedStrings.clear();
	int combinedStringsLen = strings.size() + 2;
	for (const std::string& str : strings) {
		combinedStringsLen += str.length();
	}
	combinedStrings.resize(combinedStringsLen);

	auto combStrIt = combinedStrings.begin();
	for (const std::string& str : strings) {
		for (const char c : str) {
			*combStrIt++ = c;
		}
		*combStrIt++ = 'a' - 1;
	}
	*combStrIt++ = 'a' - 1;
	*combStrIt = 'a' - 1;
}

void SubstrSorter::normalizeCombinedStrings()
{
	for (unsigned int i = 0; i < combinedStrings.size(); ++i) {
		combinedStrings[i] = combinedStrings[i] - a;
	}
}

void SubstrSorter::revertNormalizedCombinedStrings()
{
	for (unsigned int i = 0; i < combinedStrings.size(); ++i) {
		combinedStrings[i] = combinedStrings[i] + a;
	}
}

void SubstrSorter::divideCombinedStrings()
{
	s0.clear();
	s12.clear();
	for (unsigned int i = 0; i < combinedStrings.size() - 2; ++i) {
		if (i % 3 == 0) {
			s0.push_back(i);
		} else {
			s12.push_back(i);
		}
	}
}

void SubstrSorter::radixSort(std::vector<int>& saToSort, int alphabetSize, int size)
{
	for (int offset = size - 1; offset >= 0; --offset) {
		// count items
		std::vector<int> alphabetCounter(alphabetSize, 0);
		for (const int i : saToSort) {
			alphabetCounter[abs(combinedStrings[i + offset])]++;
		}	

		// accumulate positions
		for (int i = 0, sum = 0; i < alphabetSize; ++i) {
			int temp = alphabetCounter[i];
			alphabetCounter[i] = sum;
			sum += temp;
		}

		// sort
		std::vector<int> saToSortCopy = saToSort;
		for (unsigned int i = 0; i < saToSort.size(); ++i) {
			saToSort[alphabetCounter[abs(combinedStrings[saToSortCopy[i] + offset])]++] =
				saToSortCopy[i];
		}
	}
}

// returns true if the s12 needs further sorting
bool SubstrSorter::encodeS12()
{
	s12encoded.clear();
	s12encoded.resize(s12.size());

	int previousElement = s12[0];
	unsigned int currentValue = combinedStrings[s12[0]] == 0 ? 0 : 1;
	bool needsSorting = false;
	for (unsigned int i = 0; i < s12.size(); ++i) {
		char wordEnd = isWordEnd(s12[i]);

		if (!equal(s12[i], previousElement)) {
			currentValue++;
		} else if (wordEnd == 1 && currentValue != 0) {
			needsSorting = true;
		}

		if (s12[i] % 3 == 1) {
			s12encoded[s12[i] / 3] = wordEnd * currentValue;
		} else {
			s12encoded[s12[i] / 3 + (s12.size() + 1) / 2] = wordEnd * currentValue;
		}

		previousElement = s12[i];
	}

	return needsSorting;
}

bool SubstrSorter::equal(int a, int b)
{
	if (abs(combinedStrings[a]) != abs(combinedStrings[b])) {
		return false;
	}

	if (combinedStrings[a] == 0) {
		return true;
	}

	if (abs(combinedStrings[a + 1]) != abs(combinedStrings[b + 1])) {
		return false;
	}

	if (combinedStrings[a + 1] == 0) {
		return true;
	}

	if (abs(combinedStrings[a + 2]) != abs(combinedStrings[b + 2])) {
		return false;
	}

	return true;
}

char SubstrSorter::isWordEnd(int a)
{
	if (combinedStrings[a] < 1 || combinedStrings[a + 1] < 1 || combinedStrings[a + 2] < 1) {
		return -1;
	}
	return 1;
}

void SubstrSorter::decodeS12()
{
	unsigned int i = 0;
	int lastPosition = 0;
	for (; i < (s12encoded.size() + 1) / 2; ++i) {
		if (s12encoded[i] > 0) {
			lastPosition = lastPosition < s12encoded[i] ? s12encoded[i] : lastPosition;
			s12[s12encoded[i] - 1] = i * 3 + 1;
		}
	}

	for (; i < s12encoded.size(); ++i) {
		if (s12encoded[i] > 0) {
			lastPosition = lastPosition < s12encoded[i] ? s12encoded[i] : lastPosition;
			s12[s12encoded[i] - 1] = (i - s12encoded.size()) * 3 + 2;
		}
	}

	s12.erase(s12.begin() + lastPosition, s12.end());
	s12.shrink_to_fit();
}

void SubstrSorter::translateS12()
{
	SubstrSorter recursiveAlg;
	recursiveAlg.combinedStrings = s12encoded;
	if (recursiveAlg.combinedStrings[recursiveAlg.combinedStrings.size() - 1] != 0) {
		recursiveAlg.combinedStrings.push_back(0);
	}
	recursiveAlg.combinedStrings.push_back(0); // add trailing zeroes
	recursiveAlg.combinedStrings.push_back(0); // add trailing zeroes
	recursiveAlg.divideCombinedStrings();
	recursiveAlg.radixSort(recursiveAlg.s12, recursiveAlg.s12.size(), 3);

	if (encodeS12()) {
		recursiveAlg.translateS12();
	}

	recursiveAlg.radixSortBasedOnS12(recursiveAlg.s0);
	recursiveAlg.radixSort(recursiveAlg.s0, recursiveAlg.combinedStrings.size(), 1);

	recursiveAlg.radixSortBasedOnS12(recursiveAlg.s12);

	decodeS12();
}

void SubstrSorter::radixSortBasedOnS12(std::vector<int>& saToSort)
{
	int alphabetSize = s12encoded.size();
	// count items
	std::vector<int> alphabetCounter(alphabetSize, 0);
	for (const int i : saToSort) {
		int basePos;
		if (i % 3 == 2) {
			basePos = i / 3 + (s12encoded.size() + 1) / 2;
		} else {
			basePos = i / 3;
		}
		alphabetCounter[abs(s12encoded[basePos])]++;
	}	

	// accumulate positions
	for (int i = 0, sum = 0; i < alphabetSize; ++i) {
		int temp = alphabetCounter[i];
		alphabetCounter[i] = sum;
		sum += temp;
	}

	// sort
	std::vector<int> saToSortCopy = saToSort;
	for (unsigned int i = 0; i < saToSort.size(); ++i) {
		int basePos;
		if (saToSortCopy[i] % 3 == 2) {
			basePos = saToSortCopy[i] / 3 + (s12encoded.size() + 1) / 2;
		} else {
			basePos = saToSortCopy[i] / 3;
		}
		saToSort[alphabetCounter[abs(s12encoded[basePos])]++] =
			saToSortCopy[i];
	}
}

void SubstrSorter::merge(std::vector<int>& mergeString)
{
	mergeString.clear();
	mergeString.reserve(s0.size() * 3);
	auto s0it = s0.cbegin();
	auto s12it = s12.cbegin();
	while (s0it != s0.cend() && s12it != s12.cend()) {
		int result = compare(*s0it, *s12it);
		switch (result) {
		case -1:
			mergeString.push_back(*s0it++);
			break;
		case 1:
			mergeString.push_back(*s12it++);
			break;
		case 0:
			if (*s0it < *s12it) {
				mergeString.push_back(*s0it++);
				mergeString.push_back(*s12it++);
			} else {
				mergeString.push_back(*s12it++);
				mergeString.push_back(*s0it++);
			}
			break;
		default:
			break;
		}
	}

	while (s0it != s0.cend()) {
		mergeString.push_back(*s0it++);
	}

	while (s12it != s12.cend()) {
		mergeString.push_back(*s12it++);
	}
}

int SubstrSorter::compare(int a, int b)
{
	bool a0 = a % 3 == 0;
	bool a1 = a % 3 == 1;
	bool a2 = a % 3 == 2;
	bool b0 = b % 3 == 0;
	bool b1 = b % 3 == 1;
	bool b2 = b % 3 == 2;

	if (!a0 && !b0) {
		int aEncoded = a1 ? s12encoded[a / 3] : s12encoded[a / 3 + (s12encoded.size() + 1) / 2];
		int bEncoded = a1 ? s12encoded[b / 3] : s12encoded[b / 3 + (s12encoded.size() + 1) / 2];
		if  (abs(aEncoded) < abs(bEncoded)) {
			return -1;
		} else if (abs(aEncoded) > abs(bEncoded)) {
			return 1;
		} else {
			return 0;
		}
	}

	if (a0 && b1) {
		if (abs(combinedStrings[a]) < abs(combinedStrings[b])) {
			return -1;
		} else if (abs(combinedStrings[a]) > abs(combinedStrings[b])) {
			return 1;
		}
		int aEncoded = s12encoded[(a + 1) / 3];
		int bEncoded = s12encoded[(b + 1) / 3 + (s12encoded.size() + 1) / 2];
		if  (abs(aEncoded) < abs(bEncoded)) {
			return -1;
		} else if (abs(aEncoded) > abs(bEncoded)) {
			return 1;
		} else {
			return 0;
		}
	}

	if (a0 && b2) {
		if (abs(combinedStrings[a]) < abs(combinedStrings[b])) {
			return -1;
		} else if (abs(combinedStrings[a]) > abs(combinedStrings[b])) {
			return 1;
		}
		if (abs(combinedStrings[a + 1]) < abs(combinedStrings[b + 1])) {
			return -1;
		} else if (abs(combinedStrings[a + 1]) > abs(combinedStrings[b + 1])) {
			return 1;
		}
		int aEncoded = s12encoded[(a + 2) / 3 + (s12encoded.size() + 1) / 2];
		int bEncoded = s12encoded[(b + 2) / 3];
		if  (abs(aEncoded) < abs(bEncoded)) {
			return -1;
		} else if (abs(aEncoded) > abs(bEncoded)) {
			return 1;
		} else {
			return 0;
		}
	}

	return 0;
}












