#include "CartesianTree.hpp"

#include <stack>
#include <iostream>

CartesianTree::CartesianTree(const std::vector<int>& v)
{
	if (v.empty()) {
		std::cout << "CartesianTree constructor error: empty vector provided" << std::endl;
		return;
	}

	std::stack<CartesianTreeNodePtr> s;
	root.reset(new CartesianTreeNode(0));
	s.push(root);

	for (unsigned int index = 1; index < v.size(); ++index) {
		CartesianTreeNodePtr lastPopped;
		CartesianTreeNodePtr current;
		while (!s.empty() && v[index] < v[s.top()->getValue()]) {
			lastPopped = s.top();
			s.pop();
		}
		
		if (s.empty()) {
			root.reset(new CartesianTreeNode(index));
			root->setLeft(std::move(lastPopped));
			s.push(root);
		} else {
			s.top()->setRight(CartesianTreeNodePtr(new CartesianTreeNode(index)));
			s.top()->getRight()->setLeft(std::move(lastPopped));
			s.push(s.top()->getRight());
		}
	}
}

int CartesianTree::rangeMinimaQuery(int l, int h)
{
	if (l > h) {
		std::cout << "CartesianTree::rangeMinimaQuery error: low greater than high" << std::endl;
	}
	if (!root) {
		return -1;
	}
	return root->rangeMinimaQuery(l, h);
}

int CartesianTree::CartesianTreeNode::rangeMinimaQuery(int l, int h)
{
	int ret;

	if (getValue() > h) {
		if (!left) {
			ret = -1;
		} else {
			ret = left->rangeMinimaQuery(l, h);
		}
	} else if (getValue() < l) {
		if (!right) {
			ret = -1;
		} else {
			ret = right->rangeMinimaQuery(l, h);
		}
	} else {
		ret = getValue();
	}

	return ret;
}

CartesianTree::CartesianTreeNode::CartesianTreeNode(int v) : value(v)
{
}

int CartesianTree::CartesianTreeNode::getValue()
{
	return value;
}

void CartesianTree::CartesianTreeNode::setLeft(CartesianTreeNodePtr&& l)
{
	left = l;
}

void CartesianTree::CartesianTreeNode::setRight(CartesianTreeNodePtr&& r)
{
	right = r;
}

CartesianTree::CartesianTreeNodePtr CartesianTree::CartesianTreeNode::getLeft()
{
	return left;
}

CartesianTree::CartesianTreeNodePtr CartesianTree::CartesianTreeNode::getRight()
{
	return right;
}














