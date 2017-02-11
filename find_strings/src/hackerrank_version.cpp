#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <cstdio>
#include <limits>
#include <vector>
#include <cstdlib>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <map>

#include <vector>

#include <string>

class IStringer
{
public:
	virtual std::string operator[] (const unsigned int query) const = 0;
	virtual void addString(const std::string& w) = 0;
	virtual void clearStrings() = 0;
};

#include <vector>
#include <iostream>

class RMQ
{
public:
	RMQ() = delete;
	RMQ(const RMQ&) = delete;
	RMQ(RMQ&&) = delete;

	RMQ(const std::vector<int>& a);

	std::size_t operator()(std::size_t low, std::size_t high);

private:
	void calcBlockMinima();
	void createSparseTable();
	void createFullTables();
	std::size_t getCartesianNo(std::size_t index);
	void createFullTable(std::vector<std::vector<std::size_t>>& fullTable, std::size_t index);

	const std::vector<int>& array;

	std::vector<std::size_t> blockMinima;
	std::vector<std::vector<std::size_t>> sparseTable;
	std::vector<std::size_t> blockToCartesian;
	std::vector<std::vector<std::vector<std::size_t>>> cartesianToFullTable;

	std::size_t n;
	std::size_t b;

	friend class RmqTester;
};

#include <cmath>
#include <iostream>
#include <stack>
#include <bitset>
#include <algorithm>

using namespace std;

RMQ::RMQ(const std::vector<int>& a) : array(a)
{
	n = array.size();
	if (n == 0) {
		//std::cout << "RMQ ctor error: array is empty" << std::endl;
		return;
	}
	b = static_cast<std::size_t>(log2(n)) / 4;
	b = b == 0 ? 1 : b;

	//std::cout << "calcBlockMinima" << std::endl;
	calcBlockMinima();
	//std::cout << "createSparseTable" << std::endl;
	createSparseTable();
	//std::cout << "createFullTables" << std::endl;
	createFullTables();
	//std::cout << "createFullTables end" << std::endl;
}

void RMQ::calcBlockMinima()
{
	blockMinima.resize((n + b - 1) / b);
	for (std::size_t i = 0; i < n; i += b) {
		std::size_t min = i;
		for (std::size_t j = 0; j < b && i + j < array.size(); ++j) {
			if (array[i + j] < array[min]) {
				min = i + j;
			}
		}
		blockMinima[i / b] = min;
	}
}

void RMQ::createSparseTable()
{
	std::size_t blkMinTblSize = blockMinima.size();
	sparseTable.resize(blkMinTblSize);
	std::size_t length = 1;
	std::size_t acum = 1 << (length - 1);
	//std::cout << "Resize and initialize" << std::endl;
 	for (std::size_t i = 0; i < blkMinTblSize; ++i) {
		if (i == acum) {
			++length;
			acum += 1 << (length - 1);
		}
		//std::cout << "i = " << i << " length = " << length << " acum = " << acum << std::endl;
		sparseTable[blkMinTblSize - i - 1].resize(length);
		sparseTable[blkMinTblSize - i - 1][0] = blkMinTblSize - i - 1;
	}

	//std::cout << "Create sparse table" << std::endl;
	for (std::size_t j = 1; j < sparseTable[0].size(); ++j) {
		for (std::size_t i = 0; i < sparseTable.size(); ++i) {
			if (i + (1 << (j - 1)) >= sparseTable.size()
				|| j - 1 >= sparseTable[i + (1 << (j - 1))].size()) {
				//std::cout << "break" << std::endl;
				break;
			}
			//std::cout << "i = " << i << " j = " << j << std::endl;
			if (array[blockMinima[sparseTable[i][j - 1]]]
				< array[blockMinima[sparseTable[i + (1 << (j - 1))][j - 1]]]) {
				sparseTable[i][j] = sparseTable[i][j - 1];
			} else {
				sparseTable[i][j] = sparseTable[i + (1 << (j - 1))][j - 1];
			}
		}
	}
}

void RMQ::createFullTables()
{
	blockToCartesian.resize(blockMinima.size());
	cartesianToFullTable.resize(pow(4, b));
	for (std::size_t i = 0; i < blockToCartesian.size(); ++i) {
		std::size_t cartesianNo = getCartesianNo(i * b);
		blockToCartesian[i] = cartesianNo;
		if (cartesianToFullTable[cartesianNo].empty()) {
			createFullTable(cartesianToFullTable[cartesianNo], i * b);
		}
	}
}

std::size_t RMQ::getCartesianNo(std::size_t index)
{
	std::size_t cartesianNo = 0;
	std::size_t cartesianNoLen = 2 * b;
	std::size_t bit = 0;

	std::stack<int> s;
	s.push(index);
	cartesianNo |= 1 << (cartesianNoLen - 1 - bit++);

	for (std::size_t i = index + 1; i < index + b && i < array.size(); ++i) {
		while (!s.empty() && array[i] < array[s.top()]) {
			s.pop();
			bit++;
		}
		
		s.push(i);
		cartesianNo |= 1 << (cartesianNoLen - 1 - bit++);
	}

	return cartesianNo;
}

void RMQ::createFullTable(std::vector<std::vector<std::size_t>>& fullTable, std::size_t index)
{
	std::size_t fullTableSize = std::min(b, n - index);
	fullTable.resize(fullTableSize);
	for (std::size_t i = 0; i < fullTable.size(); ++i) {
		fullTable[i].resize(fullTableSize - i);
		fullTable[i][0] = i;
	}

	for (std::size_t j = 1; j < fullTable[0].size(); ++j) {
		for (std::size_t i = 0; i < fullTable.size() - j; ++i) {
			if (array[index + fullTable[i][j - 1]] < array[index + fullTable[i + 1][j - 1]]) {
				fullTable[i][j] = fullTable[i][j - 1];
			} else {
				fullTable[i][j] = fullTable[i + 1][j - 1];
			}
		}
	}
}

std::size_t RMQ::operator()(std::size_t low, std::size_t high)
{
	//std::cout << "RMQ::operator() enetered: low = " << low << " high = " << high << std::endl;
	if (low > high) {
		//std::cout << "RMQ::operator() error: low is greater than high" << std::endl;
		return 0;
	}

/*	cout << "b = " << b << endl;

	for (const auto& i : blockToCartesian) {
		cout << bitset<sizeof(size_t)>(i) << endl;
	}
	cout << endl;

	size_t cartesianNo = 0;
	for (const auto& i : cartesianToFullTable) {
		if (!i.empty()) {
			cout << "CartesianNo = " << bitset<sizeof(size_t)>(cartesianNo) << endl;
		}
		for (const auto& j : i) {
			for (const auto& k : j) {
				cout << k << ' ';
			}
			cout << endl;
		}
		if (!i.empty()) {
			cout << endl;
		}
		cartesianNo++;
	}
	cout << endl; */

	if (low /  b == high / b) {
		std::size_t ret = cartesianToFullTable[blockToCartesian[low / b]][low % b][0] + low / b * b;
		//std::cout << "RMQ::operator() return = " << ret << std::endl;
		return ret;
	}

	std::size_t lowBlockHigh = low / b * b + b - 1;
	std::size_t highBlockLow = high / b * b;
	//std::cout << "lowBlockHigh = " << lowBlockHigh << " highBlockLow = " << highBlockLow << std::endl;
	std::size_t lowBlockMin = cartesianToFullTable[blockToCartesian[low / b]][low % b][lowBlockHigh % b - low % b] + low / b * b;
	//std::cout << "lowBlockMin = " << lowBlockMin << std::endl;
	std::size_t highBlockMin = cartesianToFullTable[blockToCartesian[high / b]][highBlockLow % b][high % b - highBlockLow % b] + high / b * b;
	//std::cout << "highBlockMin = " << highBlockMin << std::endl;

	if (low / b + 1 == high / b) {
		std::size_t ret = array[lowBlockMin] < array[highBlockMin] ? lowBlockMin : highBlockMin;
		//std::cout << "RMQ::operator() return = " << ret << std::endl;
		return ret;
	}

	std::size_t sparseLow = low / b + 1;
	std::size_t sparseHigh = high / b - 1;
	std::size_t j = static_cast<std::size_t>(log2(sparseHigh - sparseLow + 1));
	std::size_t sparseMin;
	if (array[blockMinima[sparseTable[sparseLow][j]]] < array[blockMinima[sparseTable[sparseHigh - std::pow(2, j) + 1][j]]]) {
		sparseMin = blockMinima[sparseTable[sparseLow][j]];
	} else {
		sparseMin = blockMinima[sparseTable[sparseHigh - std::pow(2, j) + 1][j]];
	}
	//std::cout << "sparseMin = " << sparseMin << std::endl;

	size_t min = array[lowBlockMin] < array[highBlockMin] ? lowBlockMin : highBlockMin;
	std::size_t ret = array[min] < array[sparseMin] ? min : sparseMin;
	//std::cout << "RMQ::operator() return = " << ret << std::endl;
	return ret;
}




















#include <string>
#include <vector>
#include <memory>

class SubstrSorter
{
public:
	void passAndSort(const std::vector<std::string>& strings);
	std::string getSubstr(int k) const;

private:
	void translateS12();
	void sort();
	int getSA(int k);
	int getLCP(int k);
	int getSubstrLength(int k);
	void combineStrings(const std::vector<std::string>& strings);
	void normalizeCombinedStrings();
	void revertNormalizedCombinedStrings();
	void divideCombinedStrings();
	void radixSort(std::vector<int>& saToSort, int alphabetSize, int size);
	bool encodeS12();
	bool equal(int a, int b);
	char isWordEnd(int a);
	void radixSortBasedOnS12(std::vector<int>& saToSort, int offset);
	void merge();
	int compare(int a, int b);
	int compareSingle(int a, int b);
	void deepEncode(std::vector<int>& result);
	bool isEncodedZero(int a);
	bool isEncodedWordEnd(int a);
	void removeZeroesAndDuplicatesFromSA();
	void createLCP();
	int getSimpleEqualNo(int a, int b, int n);
	int getS12EncodedPosition(int a);
	void calcSubstrLen();
	int getSimpleLength(int a, int n);

	std::vector<int> combinedStrings;
	std::vector<int> SA;
	std::vector<int> LCP;
	std::vector<int> substrLength;
	std::vector<int> s0;
	std::vector<int> s12;
	std::vector<int> s12encoded;

	std::unique_ptr<SubstrSorter> recursiveAlg;
	std::unique_ptr<RMQ> rmqLCP;
	
	friend class Tester;
};
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
	recursiveAlg->rmqLCP.reset(new RMQ(recursiveAlg->LCP));
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
		int basePos = getS12EncodedPosition(saToSortCopy[i] + offset);
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

		if (equalNo == l) {
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
				int temp = (*recursiveAlg->rmqLCP)(low - 1, high - 2);
				//std::cout << "temp = " << temp << std::endl;
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
	for (int i : combinedStrings) {
		//std::cout << static_cast<char>(i);
	}
	//std::cout << std::endl;

	for (int i : SA) {
		//std::cout << i << ' ';
	}
	//std::cout << std::endl;

	for (int i : substrLength) {
		//std::cout << i << ' ';
	}
	//std::cout << std::endl;

	for (int i : LCP) {
		//std::cout << i << ' ';
	}
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










class Stringer : public IStringer
{
public:
	virtual std::string operator[] (const unsigned int query) const;
	virtual void addString(const std::string& w);
	virtual void clearStrings();

private:
	std::vector<std::string> strings;
	mutable SubstrSorter sorter;
	mutable bool invalidated = true;
};

std::string Stringer::operator[] (unsigned int query) const
{
	if (invalidated) {
		sorter.passAndSort(strings);
		invalidated = false;
	}
	return sorter.getSubstr(query);
}

void Stringer::addString(const std::string& w)
{
	strings.push_back(w);
	invalidated = true;
}

void Stringer::clearStrings()
{
	strings.clear();
	invalidated = true;
}

using namespace std;

/* Head ends here */
void findStrings (vector<string> a, vector<int> a_query) {
	Stringer stringer;

	for (const string& s : a) {
		stringer.addString(s);
	}

	for (const int& k : a_query) {
		cout << stringer[k] << endl;
	}
}

/* Tail starts here */
int main() {
    int res;
    
    int _cases,_query, _a_i;
    scanf("%d", &_cases);
    vector <string> _a;
    char _a_item[2001];
    for (_a_i = 0; _a_i<_cases; _a_i++) {
        scanf("%s",_a_item);
        _a.push_back(_a_item);
    }
    vector <int> _a_query;
    scanf("%d", &_query);
    int _q_item;
    for(_a_i = 0; _a_i < _query; _a_i++) { 
        scanf("%d", &_q_item);
        _a_query.push_back(_q_item);
    }
    findStrings(_a, _a_query);    
    return 0;
}
