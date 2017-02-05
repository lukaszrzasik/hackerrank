#pragma once

#include "CartesianTree.hpp"

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
	std::unique_ptr<CartesianTree> lcpTree;
	
	friend class Tester;
};
