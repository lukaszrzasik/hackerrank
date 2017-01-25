#include "substrSorter.h"

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
			cout << "Recurse" << endl;
			SubstrSorter recursiveAlg;
			recursiveAlg.combinedStrings = s.s12encoded;
			if (recursiveAlg.combinedStrings[recursiveAlg.combinedStrings.size() - 1] != 0) {
				recursiveAlg.combinedStrings.push_back(0);
			}
			recursiveAlg.combinedStrings.push_back(0);
			recursiveAlg.combinedStrings.push_back(0);
			for (const int c : recursiveAlg.combinedStrings) {
				cout << c;
			}
			cout << endl;

			recursiveAlg.divideCombinedStrings();
			for (const int i : recursiveAlg.s12) { 
				cout << i << ' ';
			}
			cout << endl;

			for (const int position : recursiveAlg.s12) {
				cout << recursiveAlg.combinedStrings[position];
				cout << recursiveAlg.combinedStrings[position + 1];
				cout << recursiveAlg.combinedStrings[position + 2];
				cout << ' ';
			}
			cout << endl;
			
			recursiveAlg.radixSort(recursiveAlg.s12, recursiveAlg.combinedStrings.size(), 3);
			for (const int i : recursiveAlg.s12) {
				cout << i << ' ';
			}
			cout << endl;

			bool recurse = recursiveAlg.encodeS12();

			for (int i : recursiveAlg.s12encoded) {
				cout << i << ' ';
			}
			cout << endl;

			if (recurse) {
				cout << "Have to recurse again? wtf?" << endl;
				for (const int i : recursiveAlg.s12) {
					cout << i << ' ';
				}
				cout << endl;

				recursiveAlg.radixSortBasedOnS12(recursiveAlg.s12, 0);

				for (const int i : recursiveAlg.s12) {
					cout << i << ' ';
				}
				cout << endl;
			}

			for (const int i : recursiveAlg.s0) {
				cout << i << ' ';
			}
			cout << endl;

			recursiveAlg.radixSortBasedOnS12(recursiveAlg.s0, 1);
			recursiveAlg.radixSort(recursiveAlg.s0, recursiveAlg.combinedStrings.size(), 1);

			for (const int i : recursiveAlg.s0) {
				cout << i << ' ';
			}
			cout << endl;

			recursiveAlg.merge();

			for (int i : recursiveAlg.SA) {
				cout << i << ' ';
			}
			cout << endl;

			for (int i : s.s12encoded) {
				cout << i << ' ';
			}
			cout << endl;

			recursiveAlg.deepEncode(s.s12encoded);

			for (int i : s.s12encoded) {
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
		}

		for (const int i : s.s0) {
			cout << i << ' ';
		}
		cout << endl;

		s.radixSortBasedOnS12(s.s0, 1);
		s.radixSort(s.s0, 27, 1);

		for (const int i : s.s0) {
			cout << i << ' ';
		}
		cout << endl;

		s.merge();

		for (int i : s.SA) {
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
