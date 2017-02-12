#include <iostream>
#include <bitset>

#include "RMQ.hpp"

using namespace std;

class RmqTester
{
public:
	void testIt(vector<int> a);

private:
};

void RmqTester::testIt(vector<int> a)
{
	RMQ dut(a);

/*	cout << "testArray:" << endl;
	for (const auto& i : a) {
		cout << i << ' ';
	}
	cout << endl;

	cout << "Block minima: " << endl;
	for (const auto& i : dut.blockMinima) {
		cout << i << ' ';
	}
	cout << endl;

	cout << "Sparse Table: " << endl;
	for (const auto& i : dut.sparseTable) {
		for (const auto& j : i) {
			cout << j << ' ';
		}
		cout << endl;
	}
	cout << endl;

	cout << "Cartesian numbers:" << endl;
	cout << "CartesianNo(0) = " << std::bitset<sizeof(std::size_t)>(dut.getCartesianNo(0)) << endl;
	cout << "CartesianNo(3) = " << std::bitset<sizeof(std::size_t)>(dut.getCartesianNo(3)) << endl;
	cout << "CartesianNo(6) = " << std::bitset<sizeof(std::size_t)>(dut.getCartesianNo(6)) << endl;
	cout << "CartesianNo(9) = " << std::bitset<sizeof(std::size_t)>(dut.getCartesianNo(9)) << endl;

	cout << "Full tables:" << endl; */
//	dut.createFullTable(dut.cartesianToFullTable[dut.getCartesianNo(0)], 0);
	size_t cartesianNo = 0;
	for (const auto& i : dut.cartesianToFullTable) {
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
	cout << endl;

	cout << "testArray[1074-1079]" << endl;
	for (int i = 1074; i <= 1079; ++i) {
		cout << a[i] << ' ';
	}
	cout << endl;

	cout << "min = " << dut(1075, 1077) << endl;

	cout << "Test finished" << endl;
}

int main()
{
	vector<int> testArray;
	int temp;
	while (cin >> temp) {
		testArray.push_back(temp);
	}
	RmqTester tester;
	tester.testIt(testArray);
	cout << "Back from testIt. dut is destroyed" << endl;
}
