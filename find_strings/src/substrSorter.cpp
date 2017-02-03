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
		for (int i : alphabetCounter) {
			//std::cout << i << ' ';
		}
		//std::cout << std::endl;

		// accumulate positions
		for (int i = 0, sum = 0; i < alphabetSize; ++i) {
			int temp = alphabetCounter[i];
			alphabetCounter[i] = sum;
			sum += temp;
		}
		for (int i : alphabetCounter) {
			//std::cout << i << ' ';
		}
		//std::cout << std::endl;

		// sort
		std::vector<int> saToSortCopy = saToSort;
		for (unsigned int i = 0; i < saToSort.size(); ++i) {
			saToSort[alphabetCounter[abs(combinedStrings[saToSortCopy[i] + offset])]++] =
				saToSortCopy[i];
		}
		for (int i : saToSort) {
			//std::cout << i << ' ';
		}
		//std::cout << std::endl;
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

	if (combinedStrings[a] < 1) {
		return true;
	}

	if (abs(combinedStrings[a + 1]) != abs(combinedStrings[b + 1])) {
		return false;
	}

	if (combinedStrings[a + 1] < 1) {
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

void SubstrSorter::translateS12()
{
	recursiveAlg.reset(new SubstrSorter); 
	recursiveAlg->combinedStrings = s12encoded;
	if (recursiveAlg->combinedStrings[recursiveAlg->combinedStrings.size() - 1] != 0) {
		recursiveAlg->combinedStrings.push_back(0);
	}
	recursiveAlg->combinedStrings.push_back(0); // add trailing zeroes
	recursiveAlg->combinedStrings.push_back(0); // add trailing zeroes
	recursiveAlg->divideCombinedStrings();
	recursiveAlg->radixSort(recursiveAlg->s12, recursiveAlg->s12.size(), 3);

	if (encodeS12()) {
		recursiveAlg->translateS12();
		recursiveAlg->radixSortBasedOnS12(recursiveAlg->s12, 0);
	}

	recursiveAlg->radixSortBasedOnS12(recursiveAlg->s0, 1);
	recursiveAlg->radixSort(recursiveAlg->s0, recursiveAlg->combinedStrings.size(), 1);

	recursiveAlg->merge();

	recursiveAlg->deepEncode(s12encoded);
}

void SubstrSorter::radixSortBasedOnS12(std::vector<int>& saToSort, int offset)
{
	int alphabetSize = s12encoded.size();
	// count items
	std::vector<int> alphabetCounter(alphabetSize, 0);
	for (const int i : saToSort) {
		int basePos;
		if ((i + offset) % 3 == 2) {
			basePos = (i + offset) / 3 + (s12encoded.size() + 1) / 2;
		} else {
			basePos = (i + offset) / 3;
		}
		alphabetCounter[abs(s12encoded[basePos])]++;
	}	
	for (int i : alphabetCounter) {
		//std::cout << i << ' ';
	}
	//std::cout << std::endl;

	// accumulate positions
	for (int i = 0, sum = 0; i < alphabetSize; ++i) {
		int temp = alphabetCounter[i];
		alphabetCounter[i] = sum;
		sum += temp;
	}
	for (int i : alphabetCounter) {
		//std::cout << i << ' ';
	}
	//std::cout << std::endl;

	// sort
	std::vector<int> saToSortCopy = saToSort;
	for (unsigned int i = 0; i < saToSort.size(); ++i) {
		int basePos;
		if ((saToSortCopy[i] + offset) % 3 == 2) {
			basePos = (saToSortCopy[i] + offset) / 3 + (s12encoded.size() + 1) / 2;
		} else {
			basePos = (saToSortCopy[i] + offset) / 3;
		}
		saToSort[alphabetCounter[abs(s12encoded[basePos])]++] =
			saToSortCopy[i];
	}
	for (int i : saToSort) {
		//std::cout << i << ' ';
	}
	//std::cout << std::endl;
}

void SubstrSorter::merge()
{
	SA.clear();
	SA.reserve(s0.size() * 3);
	auto s0it = s0.cbegin();
	auto s12it = s12.cbegin();
	while (s0it != s0.cend() && s12it != s12.cend()) {
		//std::cout << "*s0it = " << *s0it << ' ';
		//std::cout << "*s12it = " << *s12it << ' ';
		//std::cout << std::endl;
		int result = compare(*s0it, *s12it);
		//std::cout << "result = " << result << std::endl;
		switch (result) {
		case -1:
			SA.push_back(*s0it++);
			break;
		case 1:
			SA.push_back(*s12it++);
			break;
		case 0:
			if (*s0it < *s12it) {
				SA.push_back(*s0it++);
				SA.push_back(*s12it++);
			} else {
				SA.push_back(*s12it++);
				SA.push_back(*s0it++);
			}
			break;
		default:
			break;
		}
	}

	while (s0it != s0.cend()) {
		SA.push_back(*s0it++);
	}

	while (s12it != s12.cend()) {
		SA.push_back(*s12it++);
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

	//std::cout << "a0 = " << a0 << ' ';
	//std::cout << "a1 = " << a1 << ' ';
	//std::cout << "a2 = " << a2 << ' ';
	//std::cout << "b0 = " << b0 << ' ';
	//std::cout << "b1 = " << b1 << ' ';
	//std::cout << "b2 = " << b2 << ' ';
	//std::cout << std::endl;

	if (!a0 && !b0) {
		int aEncoded = a1 ? s12encoded[a / 3] : s12encoded[a / 3 + (s12encoded.size() + 1) / 2];
		int bEncoded = b1 ? s12encoded[b / 3] : s12encoded[b / 3 + (s12encoded.size() + 1) / 2];
		return compareSingle(aEncoded, bEncoded);
	}

	if (a0 && b0) {
		int result = compareSingle(combinedStrings[a], combinedStrings[b]);
		if (result != 0) {
			return result;
		} else if (combinedStrings[a] < 1) {
			return 0;
		}
		int aEncoded = s12encoded[(a + 1) / 3];
		int bEncoded = s12encoded[(b + 1) / 3];
		result = compareSingle(aEncoded, bEncoded);
		if (result != 0) {
			return result;
		} else if (aEncoded < 1) {
			return 0;
		}
		return result;
	}

	if (a0 && b1) {
		int result = compareSingle(combinedStrings[a], combinedStrings[b]);
		if (result != 0) {
			return result;
		} else if (combinedStrings[a] < 1) {
			return 0;
		}
		int aEncoded = s12encoded[(a + 1) / 3];
		int bEncoded = s12encoded[(b + 1) / 3 + (s12encoded.size() + 1) / 2];
		result = compareSingle(aEncoded, bEncoded);
		if (result != 0) {
			return result;
		} else if (aEncoded < 1) {
			return 0;
		}
		return result;
	}

	if (a0 && b2) {
		//std::cout << "combineStrings[a] = " << combinedStrings[a] << std::endl;
		//std::cout << "combineStrings[b] = " << combinedStrings[b] << std::endl;
		int result = compareSingle(combinedStrings[a], combinedStrings[b]);
		if (result != 0) {
			return result;
		} else if (combinedStrings[a] < 1) {
			return 0;
		}
		result = compareSingle(combinedStrings[a + 1], combinedStrings[b + 1]);
		if (result != 0) {
			return result;
		} else if (combinedStrings[a + 1] < 1) {
			return 0;
		}
		int aEncoded = s12encoded[(a + 2) / 3 + (s12encoded.size() + 1) / 2];
		int bEncoded = s12encoded[(b + 2) / 3];
		result = compareSingle(aEncoded, bEncoded);
		if (result != 0) {
			return result;
		} else if (aEncoded < 1) {
			return 0;
		}
		return result;
	}

	if (a1 && b0) {
		int result = compareSingle(combinedStrings[a], combinedStrings[b]);
		if (result != 0) {
			return result;
		} else if (combinedStrings[a] < 1) {
			return 0;
		}
		int aEncoded = s12encoded[(a + 1) / 3 + (s12encoded.size() + 1) / 2];
		int bEncoded = s12encoded[(b + 1) / 3];
		result = compareSingle(aEncoded, bEncoded);
		if (result != 0) {
			return result;
		} else if (aEncoded < 1) {
			return 0;
		}
		return result;
	}

	if (a2 && b0) {
		int result = compareSingle(combinedStrings[a], combinedStrings[b]);
		if (result != 0) {
			return result;
		} else if (combinedStrings[a] < 1) {
			return 0;
		}
		result = compareSingle(combinedStrings[a + 1], combinedStrings[b + 1]);
		if (result != 0) {
			return result;
		} else if (combinedStrings[a + 1] < 1) {
			return 0;
		}
		int aEncoded = s12encoded[(a + 2) / 3];
		int bEncoded = s12encoded[(b + 2) / 3 + (s12encoded.size() + 1) / 2];
		result = compareSingle(aEncoded, bEncoded);
		if (result != 0) {
			return result;
		} else if (aEncoded < 1) {
			return 0;
		}
		return result;
	}

	// it should never be reached
	return 0;
}

int SubstrSorter::compareSingle(int a, int b)
{
	//std::cout << "abs(a) = " << abs(a) << ' ';
	//std::cout << "abs(b) = " << abs(b) << ' ';
	//std::cout << std::endl;
	if (abs(a) < abs(b)) {
		return -1;
	} else if (abs(a) > abs(b)) {
		return 1;
	} else if (a < b) {
		return -1;
	} else if (a > b) {
		return 1;
	}
	return 0;
}

void SubstrSorter::deepEncode(std::vector<int>& result) {
	int previousElement = SA[0];
	unsigned int currentValue = combinedStrings[SA[0]] == 0 ? 0 : 1;
	//std::cout << "previousElement = " << previousElement << std::endl;
	//std::cout << "currentValue = " << currentValue << std::endl;
	for (unsigned int i = 0; i < SA.size(); ++i) {
		//std::cout << "i = " << i << std::endl;
		//std::cout << "SA[i] = " << SA[i] << std::endl;
		if (static_cast<unsigned>(abs(SA[i])) >= result.size()) {
			continue;
		}

		int compared = compare(SA[i], previousElement);
		//std::cout << "compared = " << compared << std::endl;
		if (compared) {
			currentValue++;
		}

		//std::cout << "currentValue = " << currentValue << std::endl;
		result[SA[i]] = currentValue;
		for (int j : result) {
			//std::cout << j << ' ';
		}
		//std::cout << std::endl;

		previousElement = SA[i];
	}
}

bool SubstrSorter::isEncodedZero(int a)
{
	bool ret = false;
	if (a % 3 == 0) {
		ret = combinedStrings[a] == 0;
	} else if (a % 3 == 1) {
		ret = s12encoded[a / 3] == 0;
	} else {
		ret = s12encoded[a / 3 + (s12encoded.size() + 1) / 2] == 0;
	}
	return ret;
}

bool SubstrSorter::isEncodedWordEnd(int a)
{
	bool ret = false;
	if (a % 3 == 0) {
		ret = combinedStrings[a] < 1 || s12encoded[(a + 1) / 3] < 1;
	} else if (a % 3 == 1) {
		ret = s12encoded[a / 3] < 1;
	} else {
		ret = s12encoded[a / 3 + (s12encoded.size() + 1) / 2] < 1;
	}
	return ret;
}

void SubstrSorter::removeZeroesAndDuplicatesFromSA()
{
	auto saCopy = SA;
	int previousElement = saCopy[0];
	int currentPosition = 0;
	for (unsigned int i = 0; i < saCopy.size(); ++i) {
		if (combinedStrings[saCopy[i]] == 0) {
			continue;
		}
		if (compare(saCopy[i], previousElement) == 0) {
			continue;
		}

		SA[currentPosition++] = saCopy[i];
		previousElement = saCopy[i];
	}

	SA.erase(SA.begin() + currentPosition, SA.end());
	SA.shrink_to_fit();
}

void SubstrSorter::createLCP()
{
	bool isRecursive = true;
	LCP.reserve(SA.size());
	LCP.push_back(-1);

	if (recursiveAlg.get() == nullptr) {
		isRecursive = false;
	}

	for (unsigned int i = 1; i < SA.size(); ++i) {
		int a = SA[i - 1];
		int b = SA[i];

		if (!isRecursive) {
//			LCP[i] = getEqualNo(a, b);
			continue;
		}

		bool a0 = a % 3 == 0;
		bool a1 = a % 3 == 1;
		bool a2 = a % 3 == 2;
		bool b0 = b % 3 == 0;
		bool b1 = b % 3 == 1;
		bool b2 = b % 3 == 2;
		
		int encodedA = -1;
		int encodedB = -1;

		if (a1) {
			encodedA = a / 3;
		} else if (a2) {
			encodedA = a / 3 + (s12encoded.size() + 1) / 2;
		}

		if (b1) {
			encodedB = b / 3;
		} else if (b2) {
			encodedB = b / 3 + (s12encoded.size() + 1) / 2;
		}

		if (!a0 && !b0) {
			int equalNo = 3 * recursiveAlg->LCP[s12encoded[encodedB]];
//			LCP[i] = equalNo + getEqualNo(a + equalNo, b + equalNo);
			continue;
		}
	}
}



















