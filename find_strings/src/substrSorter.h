#pragma once

#include <string>
#include <vector>

class SubstrSorter
{
public:
	void passAndSort(const std::vector<std::string>& strings);
	std::string getSubstr(int k) const;

private:
	void translateS12(std::vector<int>& strings);
	void sort();
	int getSA(int k);
	int getLCP(int k);
	int getSubstrLength(int k);
	void combineStrings(const std::vector<std::string>& strings);
	void normalizeCombinedStrings();
	void revertNormalizedCombinedStrings();
	void divideCombinedStrings();
	void radixSort(std::vector<int>& saToSort, int alphabetSize, int size);
	bool encodeS12(std::vector<int>& s12encoded);
	bool equal(int a, int b);
	char isWordEnd(int a);
	void decodeS12(const std::vector<int>& s12encoded);
	void radixSortBasedOnS12(std::vector<int>& saToSort, const std::vector<int>& baseString);
	void merge(std::vector<int>& mergeString, const std::vector<int>& s12encoded);

	std::vector<int> combinedStrings;
	std::vector<int> SA;
	std::vector<int> LCP;
	std::vector<int> substrLength;
	std::vector<int> s0;
	std::vector<int> s12;
	
	friend class Tester;
};
