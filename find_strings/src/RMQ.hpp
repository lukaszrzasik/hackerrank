#include <vector>
#include <iostream>

class RMQ
{
public:
	RMQ() = delete;
	RMQ(const RMQ&) = delete;
	RMQ(RMQ&&) = delete;

	RMQ(const std::vector<int>& a);
	~RMQ() {std::cout << "RMQ dtor" << std::endl;}

	int rangeMinimaQuery(int low, int high);

private:
	void calcBlockMinima();
	void createSparseTable();

	const std::vector<int>& array;

	std::vector<std::size_t> blockMinima;
	std::vector<std::vector<std::size_t>> sparseTable;

	std::size_t n;
	std::size_t b;

	friend class RmqTester;
};
