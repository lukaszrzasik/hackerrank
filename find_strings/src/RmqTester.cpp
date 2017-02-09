#include <iostream>

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
	cout << "testArray:" << endl;
	for (const auto& i : a) {
		cout << i << ' ';
	}
	cout << endl;

	RMQ dut(a);

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

	cout << "Test finished" << endl;
}

int main()
{
	vector<int> testArray = {5, 2, 8, 7, 4, 6, 1, 3, 9, 0};
	RmqTester tester;
	tester.testIt(testArray);
	cout << "Back from testIt. dut is destroyed" << endl;
}
