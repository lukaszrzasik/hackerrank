#include <vector>
#include <memory>

class CartesianTree
{
public:
	CartesianTree() = delete;
	CartesianTree(const CartesianTree&) = delete;
	CartesianTree(CartesianTree&&) = delete;

	CartesianTree(const std::vector<int>& v);

	int rangeMinimaQuery(int l, int h);

private:
	class CartesianTreeNode
	{
	public:
		typedef std::shared_ptr<CartesianTreeNode> CartesianTreeNodePtr;

		CartesianTreeNode() = delete;
		CartesianTreeNode(const CartesianTreeNode&) = delete;
		CartesianTreeNode(CartesianTreeNode&&) = delete;

		CartesianTreeNode(int v);

		int rangeMinimaQuery(int l, int h);

		int getValue();
		void setLeft(CartesianTreeNodePtr&& l);
		void setRight(CartesianTreeNodePtr&& r);
		CartesianTreeNodePtr getLeft();
		CartesianTreeNodePtr getRight();

	private:
		int value;
		CartesianTreeNodePtr left;
		CartesianTreeNodePtr right;
	};

	typedef CartesianTreeNode::CartesianTreeNodePtr CartesianTreeNodePtr;

	CartesianTreeNodePtr root;

	friend class CartesianTreeTester;
};

