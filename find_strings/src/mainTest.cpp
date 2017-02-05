#include "substrSorter.hpp"

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Tester
{
public:
	Tester(SubstrSorter& s) : s(s) {}

	void printSortedStrings() {
		for (const int position : s.s12) {
			cout << s.combinedStrings[position];
		}
	}

	void testIt(const std::vector<std::string>& strings) {
		s.combineStrings(strings);
		for (const char c : s.combinedStrings) {
			cout << c;
		}
		cout << endl;

		s.normalizeCombinedStrings();

		s.divideCombinedStrings();
		for (const int i : s.s12) {
			cout << i << ' ';
		}
		cout << endl;

		s.revertNormalizedCombinedStrings();
		for (const int position : s.s12) {
			cout << char(s.combinedStrings[position]);
			cout << char(s.combinedStrings[position + 1]);
			cout << char(s.combinedStrings[position + 2]);
			cout << ' ';
		}
		cout << endl;
		s.normalizeCombinedStrings();

		s.radixSort(s.s12, 27, 3);

		bool recurse = s.encodeS12();
		for (const int i : s.s12) {
			cout << i << ' ';
		}
		cout << endl;

		for (int i : s.s12encoded) {
			cout << i << ' ';
		}
		cout << endl;

		if (recurse) {
			recurseIt(s);
		}

		for (const int i : s.s0) {
			cout << i << ' ';
		}
		cout << endl;
		
		s.revertNormalizedCombinedStrings();
		for (const int position : s.s0) {
			cout << char(s.combinedStrings[position]);
			cout << char(s.combinedStrings[position + 1]);
			cout << char(s.combinedStrings[position + 2]);
			cout << ' ';
		}
		cout << endl;
		s.normalizeCombinedStrings();

		s.radixSortBasedOnS12(s.s0, 1);
		s.radixSort(s.s0, 27, 1);

		for (const int i : s.s0) {
			cout << i << ' ';
		}
		cout << endl;

		s.merge();

		for (const int i : s.SA) {
			cout << i << ' ';
		}
		cout << endl;

		s.removeZeroesAndDuplicatesFromSA();
		
		for (const int i : s.SA) {
			cout << i << ' ';
		}
		cout << endl;

		s.createLCP();
		for (const int i : s.LCP) {
			cout << i << ' ';
		}
		cout << endl;

		s.calcSubstrLen();
		for (const int i : s.substrLength) {
			cout << i << ' ';
		}
		cout << endl;

		s.revertNormalizedCombinedStrings();
		for (const int position : s.SA) {
			cout << char(s.combinedStrings[position]);
			cout << char(s.combinedStrings[position + 1]);
			cout << char(s.combinedStrings[position + 2]);
			cout << ' ';
		}
		cout << endl;
	}

	void recurseIt(SubstrSorter& s)
	{
		cout << "Recurse" << endl;
		s.recursiveAlg.reset(new SubstrSorter);
		s.recursiveAlg->combinedStrings = s.s12encoded;
		if (s.recursiveAlg->combinedStrings[s.recursiveAlg->combinedStrings.size() - 1] != 0) {
			s.recursiveAlg->combinedStrings.push_back(0);
		}
		s.recursiveAlg->combinedStrings.push_back(0);
		s.recursiveAlg->combinedStrings.push_back(0);
		for (const int c : s.recursiveAlg->combinedStrings) {
			cout << c;
		}
		cout << endl;

		s.recursiveAlg->divideCombinedStrings();
		for (const int i : s.recursiveAlg->s12) { 
			cout << i << ' ';
		}
		cout << endl;

		for (const int position : s.recursiveAlg->s12) {
			cout << s.recursiveAlg->combinedStrings[position];
			cout << s.recursiveAlg->combinedStrings[position + 1];
			cout << s.recursiveAlg->combinedStrings[position + 2];
			cout << ' ';
		}
		cout << endl;
		
		s.recursiveAlg->radixSort(s.recursiveAlg->s12, s.recursiveAlg->combinedStrings.size(), 3);
		for (const int i : s.recursiveAlg->s12) {
			cout << i << ' ';
		}
		cout << endl;

		bool recurse = s.recursiveAlg->encodeS12();

		for (int i : s.recursiveAlg->s12encoded) {
			cout << i << ' ';
		}
		cout << endl;

		if (recurse) {
			recurseIt(*s.recursiveAlg);
		}

		for (const int i : s.recursiveAlg->s0) {
			cout << i << ' ';
		}
		cout << endl;

		for (const int position : s.recursiveAlg->s0) {
			cout << s.recursiveAlg->combinedStrings[position];
			cout << s.recursiveAlg->combinedStrings[position + 1];
			cout << s.recursiveAlg->combinedStrings[position + 2];
			cout << ' ';
		}
		cout << endl;

		s.recursiveAlg->radixSortBasedOnS12(s.recursiveAlg->s0, 1);
		s.recursiveAlg->radixSort(s.recursiveAlg->s0, s.recursiveAlg->combinedStrings.size(), 1);

		for (const int i : s.recursiveAlg->s0) {
			cout << i << ' ';
		}
		cout << endl;

		s.recursiveAlg->merge();

		for (int i : s.recursiveAlg->SA) {
			cout << i << ' ';
		}
		cout << endl;

		for (int i : s.s12encoded) {
			cout << i << ' ';
		}
		cout << endl;

		s.recursiveAlg->deepEncode(s.s12encoded);

		for (int i : s.s12encoded) {
			cout << i << ' ';
		}
		cout << endl;
		
		for (const int i : s.recursiveAlg->SA) {
			cout << i << ' ';
		}
		cout << endl;

		s.recursiveAlg->removeZeroesAndDuplicatesFromSA();
		
		for (const int i : s.recursiveAlg->SA) {
			cout << i << ' ';
		}
		cout << endl;
		
		s.recursiveAlg->createLCP();
		for (const int i : s.recursiveAlg->LCP) {
			cout << i << ' ';
		}
		cout << endl;

		s.recursiveAlg->lcpTree.reset(new CartesianTree(s.recursiveAlg->LCP));

		s.recursiveAlg->calcSubstrLen();
		for (const int i : s.recursiveAlg->substrLength) {
			cout << i << ' ';
		}
		cout << endl;

		for (const int i : s.s12) {
			cout << i << ' ';
		}
		cout << endl;

		s.radixSortBasedOnS12(s.s12, 0);

		for (const int i : s.s12) {
			cout << i << ' ';
		}
		cout << endl;

		cout << "Recurse finished" << endl;
	}

private:
	SubstrSorter& s;
};

int main()
{
	SubstrSorter s;
	Tester tester(s);
	vector<string> v;
	int n;
	cin >> n;

	for (int i = 0; i < n; ++i) {
		string temp;
		cin >> temp;
		v.push_back(temp);
	}

	tester.testIt(v);
}
