#include "RMQ.hpp"

#include <cmath>
#include <iostream>
#include <stack>
#include <bitset>
#include <algorithm>

using namespace std;

RMQ::RMQ(const std::vector<int>& a) : array(a)
{
	n = array.size();
	if (n == 0) {
		//std::cout << "RMQ ctor error: array is empty" << std::endl;
		return;
	}
	b = static_cast<std::size_t>(log2(n)) / 4;
	b = b == 0 ? 1 : b;

	//std::cout << "calcBlockMinima" << std::endl;
	calcBlockMinima();
	//std::cout << "createSparseTable" << std::endl;
	createSparseTable();
	//std::cout << "createFullTables" << std::endl;
	createFullTables();
	//std::cout << "createFullTables end" << std::endl;
}

void RMQ::calcBlockMinima()
{
	blockMinima.resize((n + b - 1) / b);
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
	std::size_t blkMinTblSize = blockMinima.size();
	sparseTable.resize(blkMinTblSize);
	std::size_t length = 1;
	std::size_t acum = 1 << (length - 1);
	//std::cout << "Resize and initialize" << std::endl;
 	for (std::size_t i = 0; i < blkMinTblSize; ++i) {
		if (i == acum) {
			++length;
			acum += 1 << (length - 1);
		}
		//std::cout << "i = " << i << " length = " << length << " acum = " << acum << std::endl;
		sparseTable[blkMinTblSize - i - 1].resize(length);
		sparseTable[blkMinTblSize - i - 1][0] = blkMinTblSize - i - 1;
	}

	//std::cout << "Create sparse table" << std::endl;
	for (std::size_t j = 1; j < sparseTable[0].size(); ++j) {
		for (std::size_t i = 0; i < sparseTable.size(); ++i) {
			if (i + (1 << (j - 1)) >= sparseTable.size()
				|| j - 1 >= sparseTable[i + (1 << (j - 1))].size()) {
				//std::cout << "break" << std::endl;
				break;
			}
			//std::cout << "i = " << i << " j = " << j << std::endl;
			if (array[blockMinima[sparseTable[i][j - 1]]]
				< array[blockMinima[sparseTable[i + (1 << (j - 1))][j - 1]]]) {
				sparseTable[i][j] = sparseTable[i][j - 1];
			} else {
				sparseTable[i][j] = sparseTable[i + (1 << (j - 1))][j - 1];
			}
		}
	}
}

void RMQ::createFullTables()
{
	blockToCartesian.resize(blockMinima.size());
	cartesianToFullTable.resize(pow(4, b));
	for (std::size_t i = 0; i < blockToCartesian.size(); ++i) {
		std::size_t cartesianNo = getCartesianNo(i * b);
		blockToCartesian[i] = cartesianNo;
		if (cartesianToFullTable[cartesianNo].empty()) {
			createFullTable(cartesianToFullTable[cartesianNo], i * b);
//			if (cartesianNo == 0b111000) {
//				std::cout << "i = " << i << " b = " << b << std::endl;
//				std::cout << "index = " << i * b << std::endl;
//				std::cout << "111000 full table after creation:" << std::endl;
//				for (const auto& i : cartesianToFullTable[cartesianNo]) {
//					for (const auto& j : i) {
//						cout << j << ' ';
//					}
//					std::cout << std::endl;
//				}
//				std::cout << std::endl;
//
//			}
		}
	}
}

std::size_t RMQ::getCartesianNo(std::size_t index)
{
	std::size_t cartesianNo = 0;
	std::size_t cartesianNoLen = 2 * b;
	std::size_t bit = 0;

	std::stack<int> s;
	s.push(index);
	cartesianNo |= 1 << (cartesianNoLen - 1 - bit++);

	for (std::size_t i = index + 1; i < index + b && i < array.size(); ++i) {
		while (!s.empty() && array[i] < array[s.top()]) {
			s.pop();
			bit++;
		}
		
		s.push(i);
		cartesianNo |= 1 << (cartesianNoLen - 1 - bit++);
	}

	return cartesianNo;
}

void RMQ::createFullTable(std::vector<std::vector<std::size_t>>& fullTable, std::size_t index)
{
	std::size_t fullTableSize = std::min(b, n - index);
	fullTable.resize(fullTableSize);
//	if (index == 0) {
//		std::cout << "array when index 0:" << std::endl;
//		for (int i = index; i < 3; ++i) {
//			std::cout << array[i] << ' ';
//		}
//		std::cout << std::endl;
//	}
	for (std::size_t i = 0; i < fullTable.size(); ++i) {
		fullTable[i].resize(fullTableSize - i);
		fullTable[i][0] = i;
	}

	for (std::size_t j = 1; j < fullTable[0].size(); ++j) {
		for (std::size_t i = 0; i < fullTable.size() - j; ++i) {
			if (array[index + fullTable[i][j - 1]] <= array[index + fullTable[i + 1][j - 1]]) {
				fullTable[i][j] = fullTable[i][j - 1];
			} else {
				fullTable[i][j] = fullTable[i + 1][j - 1];
			}
		}
	}
}

std::size_t RMQ::operator()(std::size_t low, std::size_t high)
{
	//std::cout << "RMQ::operator() enetered: low = " << low << " high = " << high << std::endl;
	if (low > high) {
		std::cout << "RMQ::operator() enetered: low = " << low << " high = " << high << std::endl;
		std::cout << "RMQ::operator() error: low is greater than high" << std::endl;
		return 0;
	}

	if (low /  b == high / b) {
		std::size_t ret = cartesianToFullTable[blockToCartesian[low / b]][low % b][high % b - low % b] + low / b * b;
		//std::cout << "RMQ::operator() return = " << ret << std::endl;
		return ret;
	}

	std::size_t lowBlockHigh = low / b * b + b - 1;
	std::size_t highBlockLow = high / b * b;
	//std::cout << "lowBlockHigh = " << lowBlockHigh << " highBlockLow = " << highBlockLow << std::endl;
	//std::cout << "low block cartesianNo = " << std::bitset<sizeof(std::size_t)>(blockToCartesian[low / b]) << std::endl;
	//std::cout << "low block full table:" << std::endl;
//	for (const auto& i : cartesianToFullTable[blockToCartesian[low / b]]) {
//		for (const auto& j : i) {
//			cout << j << ' ';
//		}
//		//std::cout << std::endl;
//	}
	//std::cout << std::endl;
//	for (const auto& i : cartesianToFullTable[blockToCartesian[low / b]]) {
//		for (const auto& j : i) {
//			cout << array[j  + low / b * b] << ' ';
//		}
//		//std::cout << std::endl;
//	}
	//std::cout << std::endl;
	std::size_t lowBlockMin = cartesianToFullTable[blockToCartesian[low / b]][low % b][lowBlockHigh % b - low % b] + low / b * b;
	//std::cout << "lowBlockMin = " << lowBlockMin << std::endl;
	//std::cout << "high block cartesianNo = " << std::bitset<sizeof(std::size_t)>(blockToCartesian[high / b]) << std::endl;
	std::size_t highBlockMin = cartesianToFullTable[blockToCartesian[high / b]][highBlockLow % b][high % b - highBlockLow % b] + high / b * b;
	//std::cout << "highBlockMin = " << highBlockMin << std::endl;

	if (low / b + 1 == high / b) {
		std::size_t ret = array[lowBlockMin] < array[highBlockMin] ? lowBlockMin : highBlockMin;
		//std::cout << "RMQ::operator() return = " << ret << std::endl;
		return ret;
	}

	std::size_t sparseLow = low / b + 1;
	std::size_t sparseHigh = high / b - 1;
	std::size_t j = static_cast<std::size_t>(log2(sparseHigh - sparseLow + 1));
	std::size_t sparseMin;
	if (array[blockMinima[sparseTable[sparseLow][j]]] < array[blockMinima[sparseTable[sparseHigh - std::pow(2, j) + 1][j]]]) {
		sparseMin = blockMinima[sparseTable[sparseLow][j]];
	} else {
		sparseMin = blockMinima[sparseTable[sparseHigh - std::pow(2, j) + 1][j]];
	}
	//std::cout << "sparseMin = " << sparseMin << std::endl;

	size_t min = array[lowBlockMin] < array[highBlockMin] ? lowBlockMin : highBlockMin;
	std::size_t ret = array[min] < array[sparseMin] ? min : sparseMin;
	//std::cout << "RMQ::operator() return = " << ret << std::endl;
	return ret;
}



















