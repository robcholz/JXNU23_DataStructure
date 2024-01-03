//
// Created by robcholz on 11/14/23.
//
#include <string>
#include <iostream>
#include <functional>
#include <stack>


template<typename T>
struct ListNode {
	ListNode<T>* next = nullptr;
	T data;
};

template<typename T>
struct LinkedList {
	ListNode<T>* head;
	size_t length;
};

template<typename T>
struct TreeNode {
	T data{0};
	LinkedList<TreeNode<T>> children;
};

template<typename T>
struct Tree {
	TreeNode<T>* root;
	size_t length;
};

template<typename T>
void ListInit(LinkedList<T>& list) {
	auto node = new ListNode<T>;
	node->next = nullptr;
	list.head = node;
	list.length = 0;
}

template<typename T>
void ListAddFront(LinkedList<T>& list, T data) {
	auto node = new ListNode<T>;
	node->next = nullptr;
	node->data = data;
	node->next = list.head->next;
	list.head->next = node;
	++list.length;
}

template<typename T>
void ListAddBack(LinkedList<T>& list, const T& data) {
	auto node = new ListNode<T>;
	node->next = nullptr;
	node->data = data;
	auto mov = list.head;
	while (mov->next) {
		mov = mov->next;
	}
	mov->next = node;
	++list.length;
}

template<typename T>
bool ListDeleteFront(LinkedList<T>& list) {
	if (list.length == 0)
		return false;
	auto node = list.head->next;
	list.head->next = node->next;
	delete node;
	--list.length;
	return true;
}

template<typename T>
bool ListDeleteBack(LinkedList<T>& list) {
	if (list.length == 0)
		return false;
	auto node = list.head;
	while (node->next->next) {
		node = node->next;
	}
	auto de = node->next;
	node->next = nullptr;
	delete de;
	--list.length;
	return true;
}

template<typename T>
T ListPopFront(LinkedList<T>& list) {
	if (list.length == 0)
		return T{0};
	return list.head->next->data;
}

template<typename T>
T ListPopBack(LinkedList<T>& list) {
	if (list.length == 0)
		return T{0};
	auto node = list.head;
	while (node->next->next) {
		node = node->next;
	}
	return node->next->data;
}

template<typename T>
bool ListIsEmpty(const LinkedList<T>& list) {
	return (list.length == 0);
}

template<typename T>
ListNode<T>* _ListFind(const std::function<bool(const T&)>& condition, ListNode<T>* prev, ListNode<T>* current) {
	if (current == nullptr)
		return nullptr;
	if (condition(current->data))
		return prev->next;
	return _ListFind(condition, prev->next, current->next);
}

template<typename T>
ListNode<T>* ListFind(const LinkedList<T>& list, const std::function<bool(const T&)>& condition) {
	return _ListFind(condition, list.head, list.head->next);
}


template<typename T>
void TreeInit(Tree<T>& tree) {
	tree.length = 0;
	tree.root = new TreeNode<T>;
	ListInit(tree.root->children);
}

template<typename T>
void TreeSetNodeValue(TreeNode<T>& treeNode, const T& data) {
	treeNode.data = data;
}

template<typename T>
void TreeAddChildren(Tree<T>& tree, const T& value) {
	++tree.length;
	auto treeNode = new TreeNode<T>;
	ListInit(treeNode->children);
	treeNode->data = value;
	ListAddFront(tree.root->children, *treeNode);
}

template<typename T>
void TreeAddChildren(Tree<T>& tree, Tree<T>& node) {
	tree.length += node.length;
	auto treeNode = node.root;
	ListAddFront(tree.root->children, *treeNode);
}

// 第一题 层次遍历树
template<typename T>
void levelorder(TreeNode<T>* tree) {
	if (tree != nullptr) {
		auto node = tree->children.head;
		for (auto i = 0; i < tree->children.length; ++i) {
			node = node->next;
			std::cout << node->data.data << ",";
		}
		node = tree->children.head;
		for (auto i = 0; i < tree->children.length; ++i) {
			node = node->next;
			levelorder(&node->data);
		}
	}
}

// 第一题 层次遍历树
template<typename T>
void levelorder(const Tree<T>& tree) {
	std::cout << tree.root->data << ",";
	levelorder(tree.root);
}

// 第二题 非递归前序遍历
template<typename T>
void PreOrder1(const Tree<T>& tree) {
	std::stack<std::pair<TreeNode<T>*, ListNode<TreeNode<T>>*>> treeStack; // 这里为了方便使用C++内置栈类型
	TreeNode<T>* currRoot = tree.root;
	ListNode<TreeNode<T>>* currChildNodeIter = tree.root->children.head;
	std::cout << currRoot->data << ",";
	while (!treeStack.empty() || currChildNodeIter != nullptr) {
		if (currChildNodeIter != nullptr) {
			currChildNodeIter = currChildNodeIter->next;
			if (currChildNodeIter != nullptr) {
				treeStack.emplace(currRoot, currChildNodeIter); // 保存分岔节点状态
				currRoot = &currChildNodeIter->data; // 迁移至子节点
				std::cout << currRoot->data << ",";
				currChildNodeIter = currRoot->children.head; // 迭代指针指向子节点的子节点头
			}
		} // 有子节点
		else {
			currRoot = treeStack.top().first; // 回溯
			currChildNodeIter = treeStack.top().second;
			treeStack.pop();
		} // 无子节点
	}
	std::cout << std::endl;
}

// 第三题 后续遍历
template<typename T>
void PostOrder1(const Tree<T>& tree) {
	std::stack<std::pair<TreeNode<T>*, ListNode<TreeNode<T>>*>> treeStack;
	TreeNode<T>* currRoot = tree.root;
	ListNode<TreeNode<T>>* currChildNodeIter = tree.root->children.head;
	while (!treeStack.empty() || currChildNodeIter != nullptr) {
		if (currChildNodeIter != nullptr) {
			currChildNodeIter = currChildNodeIter->next;
			if (currChildNodeIter != nullptr) {
				treeStack.emplace(currRoot, currChildNodeIter); // 保存分岔节点状态
				currRoot = &currChildNodeIter->data; // 迁移至子节点
				currChildNodeIter = currRoot->children.head; // 迭代指针指向子节点的子节点头
			}
		} // 有子节点
		else {
			std::cout << currRoot->data << ",";
			currRoot = treeStack.top().first; // 回溯
			currChildNodeIter = treeStack.top().second;
			treeStack.pop();
		} // 无子节点
	}
	std::cout << currRoot->data << ",";
}

// 第四题 测试树是否相等
template<typename T>
bool TreeIsEquivalent(TreeNode<T>* tree1, TreeNode<T>* tree2) {
	if (tree1 != nullptr) {
		if (tree2 == nullptr || tree1->data != tree2->data)
			return false;
		auto node1 = tree1->children.head;
		auto node2 = tree2->children.head;
		for (auto i = 0; i < tree1->children.length; ++i) {
			bool flag;
			node1 = node1->next;
			node2 = node2->next;
			flag = TreeIsEquivalent(&node1->data, &node2->data);
			if (!flag)
				return false;
		}
	}
	return true;
}

// 第五题 从表达式建立树
Tree<std::string>* TreeSetupFromExprO2(const std::string& str) {
	Tree<std::string>* tree = nullptr;
	LinkedList<std::string> list{};
	LinkedList<Tree<std::string>> tempTreeList{};
	ListInit(list);
	ListInit(tempTreeList);

	size_t curr = 0;
	size_t tree_size = 1;
	while (curr < str.size()) {
		size_t index = 0;
		if (isalpha(str[curr])) {
			while (isalpha(str[curr + index]))
				++index;
			ListAddFront(list, str.substr(curr, index));
		} else if (str[curr] == '(') {
			ListAddFront(list, std::string("("));
			++index;
		} else if (str[curr] == ',') {
			ListAddFront(list, std::string(","));
			++index;
		} else if (str[curr] == ')') {
			bool flag = true;
			auto temp_tree = new Tree<std::string>();
			TreeInit(*temp_tree);
			while (flag) {
				auto node_value = ListPopFront(list);
				ListDeleteFront(list);
				auto comma = ListPopFront(list);
				ListDeleteFront(list);
				if (comma == std::string(","))
					flag = true;
				if (comma == std::string("(")) {
					auto rootChar = ListPopFront(list);
					TreeSetNodeValue(*temp_tree->root, rootChar);
					ListAddBack(tempTreeList, *temp_tree);
					flag = false;
				}
				auto node_appearance = ListFind<Tree<std::string>>(tempTreeList,
				                                                   [&](Tree<std::string> a) -> bool { return a.root->data == node_value; });
				if (node_appearance != nullptr)
					TreeAddChildren(*temp_tree, node_appearance->data);
				else
					TreeAddChildren(*temp_tree, node_value);
				++tree_size;
				tree = temp_tree;
			}
			++index;
		}
		curr += index;
	}
	tree->length = tree_size;
	return tree;
}

// 第五题 从表达式建立树
Tree<std::string>* TreeSetupFromExpr(const std::string& str) {
	Tree<std::string>* tree = nullptr;
	std::stack<std::string> charStack;
	std::stack<Tree<std::string>*> tempTreeStack;
	std::stack<Tree<std::string>*> constructTreeStack;
	size_t curr = 0;
	size_t tree_size = 1;
	while (curr < str.size()) {
		size_t index = 0;
		char c = str[curr];
		if (isalpha(c)) {
			while (isalpha(str[curr + index]))
				++index;
			auto node = str.substr(curr, index);
			charStack.push(node);
			auto temp = new Tree<std::string>;
			TreeInit(*temp);
			TreeSetNodeValue(*temp->root, node);
			tempTreeStack.push(temp);
		} else if (c == '(') {
			charStack.emplace("(");
			++index;
		} else if (c == ',') {
			++index;
		} else if (c == ')') {
			bool flag = true;
			while (flag) {
				auto node_value = charStack.top();
				charStack.pop();
				if (node_value == "(") {
					auto root = tempTreeStack.top();
					while (!constructTreeStack.empty()) {
						TreeAddChildren(*root, *constructTreeStack.top());
						constructTreeStack.pop();
					}
					flag = false;
				} else {
					constructTreeStack.push(tempTreeStack.top());
					tempTreeStack.pop();
				}
				++tree_size;
				tree = tempTreeStack.top();
			}
			++index;
		}
		curr += index;
	}
	if (tree)
		tree->length = tree_size;
	return tree;
}

int main() {
	auto tree1 = TreeSetupFromExpr("A(B,C(E,F(H,I),G),D)");
	auto tree2 = TreeSetupFromExpr("AL(B,C(D,E),F(G,H(I,J)))");
	std::cout << TreeIsEquivalent(tree1->root, tree2->root) << std::endl;
	levelorder(*tree1);
	std::cout << std::endl;
	PreOrder1(*tree1);
	PostOrder1(*tree1);
	return 0;
}
