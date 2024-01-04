//
// Created by robcholz on 1/2/24.
//
#include <iostream>
#include <fstream>
#include <functional>
#include <vector>
#include <stack>
#include <set>

#include "Murphy.hpp"
#include "Path.h"


static constexpr auto TOTAL_CAPACITY = 500000;
static int orderedStorage[TOTAL_CAPACITY];
static int disorderedStorage[TOTAL_CAPACITY];

struct LinkedListNode {
    int data;
    LinkedListNode* next = nullptr;

    explicit LinkedListNode() {
        data = int{};
    }

    explicit LinkedListNode(int data) {
        this->data = data;
    }
};

struct LinkedList {
    size_t size = 0;
    LinkedListNode* head = nullptr;

    explicit LinkedList(int* array) {
        head = new LinkedListNode();
        auto res_head = head;
        for (auto i = 0; i < TOTAL_CAPACITY; ++i) {
            auto node = new LinkedListNode(array[i]);
            res_head->next = node;
            res_head = node;
        }
    }
};

struct BinaryTreeNode {
    int data;
    BinaryTreeNode* left;
    BinaryTreeNode* right;

    explicit BinaryTreeNode(int data) {
        this->left = nullptr;
        this->right = nullptr;
        this->data = data;
    }
};

struct BinaryTree {
    BinaryTreeNode* _root = nullptr;
    size_t _size;

    explicit BinaryTree(int* array) {
        _size = 0;

        for (size_t i = 0; i < TOTAL_CAPACITY; ++i) {
            insert(array[i]);
        }
    }

    ~BinaryTree() {
        // free();
    }

    void insert(int element) {
        BinaryTreeNode** node = &_root;
        bool duplicate = false;
        bool flag = (*node) != nullptr;;
        while (flag) {
            if (element > (*node)->data) {
                node = &(*node)->right;
                flag = (*node) != nullptr;
            } // go to right node
            else if (element < (*node)->data) {
                node = &(*node)->left;
                flag = (*node) != nullptr;
            } // go to the left one
            else {
                flag = false;
                duplicate = true;
            } // equality, quit
        }
        if (!duplicate) {
            *node = new BinaryTreeNode(element);
            ++_size;
        }
    }
};

static LinkedList* list = nullptr;
static BinaryTree* binaryTree = nullptr;

bool sequentialSearch(uint64_t capacity, uint16_t key) {
    for (size_t i = 0; i < capacity; ++i) {
        if (disorderedStorage[i] == key)
            return true;
    }
    return false;
}

bool linkedListSearch(uint64_t capacity, uint16_t key) {
    auto node = list->head;
    for (size_t i = 0; i < capacity; ++i) {
        node = node->next;
        if (node->data == key)
            return true;
    }
    return false;
}

bool binaryNonRecursiveArraySearch(uint64_t capacity, uint16_t key) {
    int64_t left, right, middle;
    left = 0;
    right = (int64_t) (capacity) - 1;
    while (left <= right) {
        middle = left + ((right - left) / 2);
        if (orderedStorage[middle] == key)
            return true;
        if (orderedStorage[middle] < key)
            left = middle + 1;
        else
            right = middle - 1;
    }
    return false;
}

bool binaryRecursiveArraySearch(int64_t left, int64_t right, uint16_t key) {
    if (left > right) // escape condition
        return false;
    int64_t middle = left + ((right - left) / 2);
    if (orderedStorage[middle] == key)
        return true;
    if (orderedStorage[middle] < key)
        return binaryRecursiveArraySearch(middle + 1, right, key);
    else
        return binaryRecursiveArraySearch(left, middle - 1, key);
}

bool binaryRecursiveArraySearch(uint64_t capacity, uint16_t key) {
    return binaryRecursiveArraySearch(0, (int64_t) capacity - 1, key);
}

bool binaryTreeSearch(uint64_t capacity, uint16_t key) {
    auto node = binaryTree->_root;
    while (node) {
        if (key > node->data) {
            node = node->right;
        } else if (key < node->data) {
            node = node->left;
        } else {
            return true;
        }
    }
    return false;
}

bool greaterThanX(uint64_t capacity, uint16_t key) {
    std::vector<int> result;
    std::stack<BinaryTreeNode*> stack;
    BinaryTreeNode* current = binaryTree->_root;

    uint32_t new_key = 499990;

    while (current != nullptr || !stack.empty()) {
        while (current != nullptr) {
            stack.push(current);
            current = current->left;
        }
        current = stack.top();
        stack.pop();
        if (current->data > new_key) {
            result.push_back(current->data);
        }
        current = current->right;
    }
    std::reverse(result.begin(), result.end());
    std::cout << "{ ";
    for (const auto &element: result) {
        std::cout << element << " ";
    }
    std::cout << "}" << std::endl;
    return !result.empty();
}

bool findValueAtLevel(uint64_t capacity, uint16_t key) {
    std::set<size_t> result;
    std::stack<BinaryTreeNode*> stack;
    BinaryTreeNode* current = binaryTree->_root;
    long long curr_level = 0;

    while (current != nullptr || !stack.empty()) {
        while (current != nullptr) {
            stack.push(current);
            current = current->left;
            ++curr_level;
        }
        current = stack.top();
        stack.pop();
        --curr_level;
        if (current->data == key) {
            result.insert(curr_level);
        }
        current = current->right;
        ++curr_level;
    }
    std::cout << "{ ";
    for (const auto &element: result) {
        std::cout << element << " ";
    }
    short res = 1;
    if (binaryTree->_size == 0)
        res = -1;
    if (result.empty())
        res = 0;
    std::cout << "} " << "返回: " << res << std::endl;
    return !result.empty();
}

bool isSearchSequence(BinaryTreeNode* root, uint16_t key, const std::vector<int> &sequence, int &index) {
    if (root == nullptr)
        return true;
    if (root->data == key)
        return true;
    if (root->data != sequence[index])
        return false;
    ++index;
    return (isSearchSequence(root->left, key, sequence, index) ||
            isSearchSequence(root->right, key, sequence, index));
}

bool findSearchSequence(uint64_t capacity, uint16_t key) {
    std::vector<int> sequence{12, 213};
    int index = 0;
    bool result = isSearchSequence(binaryTree->_root, key, sequence, index)
                  && (index == sequence.size());
    std::cout << "测试序列: { ";
    for (const auto &ele: sequence) {
        std::cout << ele << " ";
    }
    std::cout << " }" << std::endl;
    return result;
}

void profiler(bool (* func)(uint64_t, uint16_t)) {
    mur_profiler_output_lambda(
            "Size: 10000",
            [&]() -> bool { return func(10000, 1989); }
    );
    mur_profiler_output_lambda(
            "Size: 50000",
            [func]() -> bool { return func(50000, 1989); }
    );
    mur_profiler_output_lambda(
            "Size: 100000",
            [func]() -> bool {return func(100000, 1989);}
            );
    mur_profiler_output_lambda(
            "Size: 200000",
            [func]() -> bool {return func(200000, 1989);}
            );
    mur_profiler_output_lambda(
            "Size: 300000",
            [func]() -> bool {return func(300000, 1989);}
    );
    mur_profiler_output_lambda(
            "Size: 400000",
            [func]() -> bool {return func(400000, 1989);}
    );
    mur_profiler_output_lambda(
            "Size: 500000",
            [func]() -> bool {return func(500000, 1989);}
    );
}

void initArrayData(int* array, const std::string &filename) {
    std::fstream file{filename};
    if (file.is_open()) {
        char c;
        size_t index = 0;
        std::string str;
        while (file.get(c)) {
            if (index != TOTAL_CAPACITY) {
                if (isnumber(c)) {
                    str.push_back(c);
                } else if (isspace(c)) {
                    array[index] = atoi(str.c_str());
                    str.clear();
                    ++index;
                }
            }
        }
        file.close();
    } else {
        std::cout << "Failed to open the file." << std::endl;
    }
}

int main() {
    initArrayData(orderedStorage,
                  ORDERED_SAMPLE_PATH
                  );
    initArrayData(disorderedStorage,
                  DISORDERED_SAMPLE_PATH
                  );
    list = new LinkedList(orderedStorage);
    // since this is not a balanced tree, we do not use ordered data
    binaryTree = new BinaryTree(disorderedStorage);
    std::cout << "1.基于数组顺序查找:" << std::endl;
    profiler(sequentialSearch);
    std::cout << "2.基于单链表顺序查找:" << std::endl;
    profiler(linkedListSearch);
    std::cout << std::endl << "3.基于数组非递归二分查找:" << std::endl;
    profiler(binaryNonRecursiveArraySearch);
    std::cout << std::endl << "4.基于数组递归二分查找:" << std::endl;
    profiler(binaryRecursiveArraySearch);
    std::cout << std::endl << "5.为数组a的数据建立二叉排序树并查找:" << std::endl;
    profiler(binaryTreeSearch);
    std::cout << std::endl << "6.递归查找输出二叉排序树中关键字不小于k(499990)的节点值:" << std::endl;
    profiler(greaterThanX);
    std::cout << std::endl << "7.求二叉排序树指定节点层次, 若树为空返回-1 若节点不在树中返回0:" << std::endl;
    profiler(findValueAtLevel);
    std::cout << std::endl << "8.判断是否从查找序列搜索的k关键字:" << std::endl;
    profiler(findSearchSequence);
    return 0;
}
