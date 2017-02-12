#include "substrSorterCartesian.hpp"

#include <iostream>
#include <cmath>

namespace {
	const char a = 97 - 1; // 'a' ASCII code minus one (word seperator)
	const char engAlphSize = 26 + 1; // +1 for word separator
}

void SubstrSorter::passAndSort(const std::vector<std::string>& strings)
{
	//std::cout << "passAndSort" << std::endl;
	//std::cout << "combineStrings" << std::endl;
	combineStrings(strings);
	//std::cout << "normalizeCombinedStrings" << std::endl;
	normalizeCombinedStrings();
	//std::cout << "divideCombinedStrings" << std::endl;
	divideCombinedStrings();
	//std::cout << "radixSort" << std::endl;
	radixSort(s12, engAlphSize, 3);

	//std::cout << "encodeS12" << std::endl;
	if (encodeS12()) {
		//std::cout << "translateS12" << std::endl;
		translateS12();
		//std::cout << "radixSortBasedOnS12" << std::endl;
		radixSortBasedOnS12(s12, 0);
	}

	//std::cout << "radixSortBasedOnS12" << std::endl;
	radixSortBasedOnS12(s0, 1);
	//std::cout << "radixSort" << std::endl;
	radixSort(s0, engAlphSize, 1);
	//std::cout << "merge" << std::endl;
	merge();
	//std::cout << "removeZeroesAndDuplicatesFromSA" << std::endl;
	removeZeroesAndDuplicatesFromSA();
	//std::cout << "createLCP" << std::endl;
	createLCP();
	//std::cout << "calcSubstrLen" << std::endl;
	calcSubstrLen();

	//std::cout << "revertNormalizedCombinedStrings" << std::endl;
	revertNormalizedCombinedStrings();
	//std::cout << "passAndSort finished" << std::endl;
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
//		for (int i : alphabetCounter) {
//			//std::cout << i << ' ';
//		}
		//std::cout << std::endl;

		// accumulate positions
		for (int i = 0, sum = 0; i < alphabetSize; ++i) {
			int temp = alphabetCounter[i];
			alphabetCounter[i] = sum;
			sum += temp;
		}
//		for (int i : alphabetCounter) {
//			//std::cout << i << ' ';
//		}
		//std::cout << std::endl;

		// sort
		std::vector<int> saToSortCopy = saToSort;
		for (unsigned int i = 0; i < saToSort.size(); ++i) {
			saToSort[alphabetCounter[abs(combinedStrings[saToSortCopy[i] + offset])]++] =
				saToSortCopy[i];
		}
//		for (int i : saToSort) {
//			//std::cout << i << ' ';
//		}
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

		s12encoded[getS12EncodedPosition(s12[i])] = wordEnd * currentValue;

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
	//std::cout << "translateS12 started" << std::endl;
	recursiveAlg.reset(new SubstrSorter); 
	recursiveAlg->combinedStrings = s12encoded;
	recursiveAlg->combinedStrings.push_back(0); // add trailing zeroes
	recursiveAlg->combinedStrings.push_back(0); // add trailing zeroes
	//std::cout << "divideCombinedStrings" << std::endl;
	recursiveAlg->divideCombinedStrings();
	//std::cout << "radixSort" << std::endl;
	recursiveAlg->radixSort(recursiveAlg->s12, recursiveAlg->combinedStrings.size(), 3);

	//std::cout << "encodeS12" << std::endl;
	if (recursiveAlg->encodeS12()) {
		//std::cout << "translateS12" << std::endl;
		recursiveAlg->translateS12();
		//std::cout << "radixSortBasedOnS12" << std::endl;
		recursiveAlg->radixSortBasedOnS12(recursiveAlg->s12, 0);
	}

	//std::cout << "radixSortBasedOnS12" << std::endl;
	recursiveAlg->radixSortBasedOnS12(recursiveAlg->s0, 1);
	//std::cout << "radixSort" << std::endl;
	recursiveAlg->radixSort(recursiveAlg->s0, recursiveAlg->combinedStrings.size(), 1);

	//std::cout << "merge" << std::endl;
	recursiveAlg->merge();
	//std::cout << "deepEncode" << std::endl;
	recursiveAlg->deepEncode(s12encoded);
	//std::cout << "removeZeroesAndDuplicatesFromSA" << std::endl;
	recursiveAlg->removeZeroesAndDuplicatesFromSA();
	//std::cout << "createLCP" << std::endl;
	recursiveAlg->createLCP();
//	recursiveAlg->rmqLCP.reset(new RMQ(recursiveAlg->LCP));
	recursiveAlg->lcpTree.reset(new CartesianTree(recursiveAlg->LCP));
	//std::cout << "calcSubstrLen" << std::endl;
	recursiveAlg->calcSubstrLen();
	//std::cout << "translateS12 finished" << std::endl;
}

void SubstrSorter::radixSortBasedOnS12(std::vector<int>& saToSort, int offset)
{
	int alphabetSize = s12encoded.size();
	// count items
	std::vector<int> alphabetCounter(alphabetSize, 0);
	for (const int i : saToSort) {
		int basePos = getS12EncodedPosition(i + offset);
		alphabetCounter[abs(s12encoded[basePos])]++;
	}	
//	for (int i : alphabetCounter) {
//		//std::cout << i << ' ';
//	}
	//std::cout << std::endl;

	// accumulate positions
	for (int i = 0, sum = 0; i < alphabetSize; ++i) {
		int temp = alphabetCounter[i];
		alphabetCounter[i] = sum;
		sum += temp;
	}
//	for (int i : alphabetCounter) {
//		//std::cout << i << ' ';
//	}
	//std::cout << std::endl;

	// sort
	std::vector<int> saToSortCopy = saToSort;
	for (unsigned int i = 0; i < saToSort.size(); ++i) {
		int basePos = getS12EncodedPosition(saToSortCopy[i] + offset);
		saToSort[alphabetCounter[abs(s12encoded[basePos])]++] =
			saToSortCopy[i];
	}
//	for (int i : saToSort) {
//		//std::cout << i << ' ';
//	}
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
	//std::cout << "a = " << a << " b = " << b << std::endl;
	int result;
	int l = 0;
	while ((a + l) % 3 == 0 || (b + l) % 3 == 0) {
		++l;
	}
	//std::cout << "l = " << l << std::endl;

	for (int i = 0; i < l; ++i) {
		result = compareSingle(combinedStrings[a + i], combinedStrings[b + i]);
		//std::cout << "compareSingle result = " << result << std::endl;
		if (result != 0 || combinedStrings[a + i] < 1) {
			return result;
		}
	}

	result = compareSingle(s12encoded[getS12EncodedPosition(a + l)],
			               s12encoded[getS12EncodedPosition(b + l)]);
	
	return result;
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
		if (static_cast<size_t>(abs(SA[i])) >= result.size()) {
			continue;
		}

		int compared = compare(SA[i], previousElement);
		//std::cout << "compared = " << compared << std::endl;
		if (compared) {
			currentValue++;
		}

		int wordEnd = combinedStrings[SA[i]] < 1 ? -1 : 1;

		//std::cout << "currentValue = " << currentValue << std::endl;
		result[SA[i]] = currentValue * wordEnd;
//		for (int j : result) {
//			//std::cout << j << ' ';
//		}
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
	LCP.resize(SA.size());
	for (size_t i = 0; i < SA.size() - 1; ++i) {
		//std::cout << "i = " << i << std::endl;
		int a = SA[i];
		int b = SA[i + 1];
		int l = 0;

		while ((a + l) % 3 == 0 || (b + l) % 3 == 0) {
			++l;
		}
		
		int equalNo = getSimpleEqualNo(a, b, l);
		//std::cout << "l = " << l << std::endl;
		//std::cout << "equalNo after getSimpleEqualNo = " << equalNo << std::endl;

		if (equalNo == l && combinedStrings[a + l] != 0 && combinedStrings[b + l] != 0) {
			if (recursiveAlg) {
				int s12encodedPosA = getS12EncodedPosition(a + l);
				int s12encodedPosB = getS12EncodedPosition(b + l);
				//std::cout << "getS12EncodedPosition(a + l) = " << s12encodedPosA << std::endl;
				//std::cout << "getS12EncodedPosition(b + l) = " << s12encodedPosB << std::endl;
				int aa = abs(s12encoded[s12encodedPosA]);
				int bb = abs(s12encoded[s12encodedPosB]);
				int low = aa < bb ? aa : bb;
				int high = aa < bb ? bb : aa;
				//std::cout << "low = " << low << " high = " << high << std::endl;
//				if (static_cast<std::size_t>(low - 1) > 100000) {
//					std::cout << "low = " << low << std::endl;
//				}
//				int temp = (*recursiveAlg->rmqLCP)(low - 1, high - 2);
				int temp = recursiveAlg->lcpTree->rangeMinimaQuery(low - 1, high - 2);
//				if (low - 1 == 1075 && high - 2 == 1077 && temp == 1075) {
//					std::cout << "low = " << low - 1 << " high = " << high - 2 << std::endl;
//					std::cout << "temp = " << temp << std::endl;
//					std::cout << "recursiveAlg->LCP[temp] = " << recursiveAlg->LCP[temp] << std::endl;
//					std::cout << "a = " << a << " b = " << b << " l = " << l << std::endl;
//
//					std::cout << "combinedStrings = " << std::endl;
//					for (const auto& i : combinedStrings) {
//						std::cout << i << ' ';
//					}
//					std::cout << std::endl;
//					std::cout << std::endl;
//
//					std::cout << "s12encoded = " << std::endl;
//					for (const auto& i : s12encoded) {
//						std::cout << i << ' ';
//					}
//					std::cout << std::endl;
//					std::cout << std::endl;
//
//					std::cout << "SA = " << std::endl;
//					for (const auto& i : SA) {
//						std::cout << i << ' ';
//					}
//					std::cout << std::endl;
//					std::cout << std::endl;
//
//					std::cout << "LCP = " << std::endl;
//					for (const auto& i : LCP) {
//						std::cout << i << ' ';
//					}
//					std::cout << std::endl;
//					std::cout << std::endl;
//
//					std::cout << "recursiveAlg->LCP = " << std::endl;
//					for (int i = 1075; i <= 1077; ++i) {
//						std::cout << recursiveAlg->LCP[i] << ' ';
//					}
//					std::cout << std::endl;
//					std::cout << std::endl;
//				}
//				int temp = recursiveAlg->lcpTree->rangeMinimaQuery(low - 1, high - 2);
				equalNo += 3 * recursiveAlg->LCP[temp];
				//std::cout << "equalNo after rangeMinimaQuery = " << equalNo << std::endl;
			}
			
			equalNo += getSimpleEqualNo(a + equalNo, b + equalNo, 3);
			//std::cout << "equalNo after second getSimpleEqualNo = " << equalNo << std::endl;
		}

		LCP[i] = equalNo;
	}
}

int SubstrSorter::getSimpleEqualNo(int a, int b, int n)
{
	int result = 0;
	for (int i = 0; i < n; ++i) {
		if (combinedStrings[a + i] == 0 || combinedStrings[b + i] == 0) {
			break;
		}

		if (abs(combinedStrings[a + i]) == abs(combinedStrings[b + i])) {
			++result;
		} else {
			break;
		}

		if (combinedStrings[a + i] < 0 || combinedStrings[b + i] < 0) {
			break;
		}
	}

	return result;
}

int SubstrSorter::getS12EncodedPosition(int a)
{
	int result;
	if (a % 3 == 0) {
		std::cout << "SubstrSorter::getS12EncodedValue: error a = " << a << std::endl;
		result = 0;
	} else if (a % 3 == 1) {
		result = a / 3;
	} else {
		result = a / 3 + (s12encoded.size() + 1) / 2;
	}

	return result;
}

void SubstrSorter::calcSubstrLen()
{
	substrLength.resize(SA.size());
	for (size_t i = 0; i < SA.size(); ++i) {
		//std::cout << "i = " << i << std::endl;
		int a = SA[i];
		int l = 0;

		while ((a + l) % 3 == 0) {
			++l;
		}
		//std::cout << "l = " << l << std::endl;
		
		int length = getSimpleLength(a, l);
		//std::cout << "length after getSimpleLength = " << length << std::endl;

		if (length == l && recursiveAlg) {
			int s12encodedPosA = getS12EncodedPosition(a + l);
			//std::cout << "getS12EncodedPosition(a + l) = " << s12encodedPosA << std::endl;
			int aa = abs(s12encoded[s12encodedPosA]);
			//std::cout << "aa = " << aa << std::endl;
			length += 3 * recursiveAlg->substrLength[aa - 1];
			//std::cout << "length after substrLength = " << length << std::endl;
		}

		length += getSimpleLength(a + length, 0);
		//std::cout << "length after second getSimpleLength = " << length << std::endl;

		substrLength[i] = length;
	}
}

int SubstrSorter::getSimpleLength(int a, int n)
{
	int result = 0;
	int i = a;
	while (combinedStrings[i] > 0) {
		if (n != 0 && result < n) {
			break;
		}
		++result;
		++i;
	}
	return result;
}

std::string SubstrSorter::getSubstr(int k) const
{
	//std::cout << "k = " << k << std::endl;
	size_t substrPos = 0;
	int lcp = 0;
	int kp = 0;
//	for (int i : combinedStrings) {
//		//std::cout << static_cast<char>(i);
//	}
	//std::cout << std::endl;

//	for (int i : SA) {
//		//std::cout << i << ' ';
//	}
	//std::cout << std::endl;

//	for (int i : substrLength) {
//		//std::cout << i << ' ';
//	}
	//std::cout << std::endl;

//	for (int i : LCP) {
//		//std::cout << i << ' ';
//	}
	//std::cout << std::endl;

	while (kp + substrLength[substrPos] - lcp < k && substrPos < SA.size()) {
		kp += substrLength[substrPos] - lcp;
		++substrPos;
		lcp = LCP[substrPos - 1];
		//std::cout << "substrPos = " << substrPos << std::endl;
		//std::cout << "lcp = " << lcp << std::endl;
		//std::cout << "kp = " << kp << std::endl;
	}

	if (substrPos >= SA.size()) {
		return std::string("INVALID");
	}

	std::string result;
	for (int i = 0; i < lcp; ++i) {
		result.push_back(static_cast<char>(combinedStrings[SA[substrPos] + i]));
	}
	for (int i = 0; i < k - kp; ++i) {
		result.push_back(static_cast<char>(combinedStrings[SA[substrPos] + lcp + i]));
	}

	return result;
}









