#include <iostream>
#include <random>

using namespace std;

int main()
{
	mt19937_64 generator;
	uniform_int_distribution<unsigned int> distribution(1U,300000U);
	unsigned int N = distribution(generator);
	unsigned int M = distribution(generator);
	
	uniform_int_distribution<unsigned long long int> distributionBig(1ULL,1000000000000000000ULL);
	unsigned long long int K = distributionBig(generator);

	uniform_int_distribution<int> distributionAlph('a','z');

	string A, B;
	A.resize(N);
	B.resize(M);

	for(unsigned int i = 0; i < N; ++i) {
		A[i] = distributionAlph(generator);
	}

	for(unsigned int i = 0; i < M; ++i) {
		B[i] = distributionAlph(generator);
	}

	cout << N << ' ' << M << ' ' << K << endl;
	cout << A << endl;
	cout << B << endl;
}
