#include <iostream>
#include <random>
#include <chrono>

using namespace std;

int main()
{
	int n = 50;
	int max_m =	2000;
	int q = 500;

	int maxSize = 1000000000; // max size of S

	cout << n << endl;

	unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
	mt19937 generator(seed1);
	uniform_int_distribution<int> stringSizeDistribution(1,max_m);
	uniform_int_distribution<char> charDistribution('a','z');

    for (int i = 0; i < n; ++i) 
	{
		int stringSize = stringSizeDistribution(generator);
	    string s;
		
	    for (int j = 0; j < stringSize; ++j)
		{
			char c = charDistribution(generator);
			s.push_back(c);
//			maxSize += (1 + j + 1) / 2 * (j + 1) / 100; // arithmetic progression sum
		}	

		cout << s << endl;
	}

	cout << q << endl;

	uniform_int_distribution<int> queryDistribution(1,maxSize);

	for (int i = 0; i < q; ++i)
	{
		cout << queryDistribution(generator) << endl;
	}

}
