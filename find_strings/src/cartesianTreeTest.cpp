#include <vector>
#include <iostream>

#include "CartesianTree.hpp"

using namespace std;

class CartesianTreeTester
{
public:
	void test();
};

int main()
{
	CartesianTreeTester tester;
	tester.test();
}

void CartesianTreeTester::test()
{
	vector<int> testVector = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

	CartesianTree tree(testVector);
	cout << "Range minima query = " << tree.rangeMinimaQuery(2, 20) << endl;
}
