#include "RMQ.hpp"

#include <cmath>
#include <iostream>

RMQ::RMQ(const std::vector<int>& a) : array(a)
{
	n = array.size();
	b = static_cast<std::size_t>(log2(n)); // / 4;
	b = b == 0 ? 1 : b;

	blockMinima.resize((n + b - 1) / b);
	calcBlockMinima();
	createSparseTable();
}

void RMQ::calcBlockMinima()
{
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
	sparseTable.resize(n);
	std::size_t length = 1;
	std::size_t acum = 1 << (length - 1);
	std::cout << "Resize and initialize" << std::endl;
 	for (std::size_t i = 0; i < n; ++i) {
		if (i == acum) {
			++length;
			acum += 1 << (length - 1);
		}
		std::cout << "i = " << i << " length = " << length << " acum = " << acum << std::endl;
		sparseTable[n - i - 1].resize(length);
		sparseTable[n - i - 1][0] = n - i - 1;
	}

	std::cout << "Create sparse table" << std::endl;
	for (std::size_t j = 1; j < sparseTable[0].size(); ++j) {
		for (std::size_t i = 0; i < sparseTable.size(); ++i) {
			if (i + (1 << (j - 1)) >= sparseTable.size()
				|| j - 1 >= sparseTable[i + (1 << (j - 1))].size()) {
				std::cout << "break" << std::endl;
				break;
			}
			std::cout << "i = " << i << " j = " << j << std::endl;
			if (array[sparseTable[i][j - 1]] < array[sparseTable[i + (1 << (j - 1))][j - 1]]) {
				sparseTable[i][j] = sparseTable[i][j - 1];
			} else {
				sparseTable[i][j] = sparseTable[i + (1 << (j - 1))][j - 1];
			}
		}
	}
}
