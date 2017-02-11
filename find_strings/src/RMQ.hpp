#include <vector>
#include <iostream>

class RMQ
{
public:
	RMQ() = delete;
	RMQ(const RMQ&) = delete;
	RMQ(RMQ&&) = delete;

	RMQ(const std::vector<int>& a);

	std::size_t operator()(std::size_t low, std::size_t high);

private:
	void calcBlockMinima();
	void createSparseTable();
	void createFullTables();
	std::size_t getCartesianNo(std::size_t index);
	void createFullTable(std::vector<std::vector<std::size_t>>& fullTable, std::size_t index);

	const std::vector<int>& array;

	std::vector<std::size_t> blockMinima;
	std::vector<std::vector<std::size_t>> sparseTable;
	std::vector<std::size_t> blockToCartesian;
	std::vector<std::vector<std::vector<std::size_t>>> cartesianToFullTable;

	std::size_t n;
	std::size_t b;

	friend class RmqTester;
};
