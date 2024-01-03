//
// Created by robcholz on 9/27/23.
//
#include <iostream>
#include <vector>


template<typename T>
struct ListNode {
	T data;
	ListNode<T>* next;
};

template<typename T>
void ListCreate(ListNode<T>** listNode, const std::vector<T>& values) {
	(*listNode) = new ListNode<T>;
	ListNode<T>* head = *listNode;
	head->next = nullptr;
	(*listNode)->data = values[0];
	for (auto i = 1; i < values.size(); ++i) {
		(*listNode)->next = new ListNode<T>;
		*listNode = (*listNode)->next;
		(*listNode)->data = values[i];
	}
	*listNode = head;
}

template<typename T>
void ListDelete(ListNode<T>** listNode, const T& element) {
	auto reserveNode = *listNode;
	while (reserveNode->next != nullptr) {
		if (reserveNode->next->data == element) {
			auto node = reserveNode->next->next;
			::free(reserveNode->next);
			reserveNode->next = node;
			return;
		}
		reserveNode = reserveNode->next;
	}
}

template<typename T>
void ListDeleteAll(ListNode<T>** listNode, const T& element) {
	auto reserveNode = *listNode;
	ListNode<T>* before, * next;
	while (reserveNode != nullptr) {
		before = reserveNode;
		next = reserveNode->next;
		if(before->data==element){
			auto temp=reserveNode->next;
			::free(reserveNode);
			*listNode=temp;
			reserveNode=temp;
		}
		else if (next!= nullptr&&next->data == element) {
			before->next = next->next;
			::free(next);
			next=before->next;
		} else
			reserveNode = reserveNode->next;
	}
}

template<typename T>
void ListPrint(ListNode<T>* listNode) {
	std::cout << "[ ";
	while (listNode != nullptr) {
		std::cout << listNode->data;
		listNode = listNode->next;
		std::cout << " ";
	}
	std::cout << "]" << std::endl;
}

template<typename T>
void ListReserve(ListNode<T>** listNode) {
	auto reserveNode = *listNode;
	ListNode<T>* nodeNext;
	auto newHead = new ListNode<T>;
	newHead->next = nullptr;
	while (reserveNode != nullptr) {
		nodeNext = reserveNode->next;

		reserveNode->next = newHead->next;
		newHead->next = reserveNode;

		reserveNode = nodeNext;
	}
	*listNode = newHead->next;
	::free(newHead);
}

template<typename T>
void ListInsert(ListNode<T>** listNode, const T& element) {
	auto reserveNode = *listNode;
	ListNode<T>* nodeBefore, * nodeNext;
	while (reserveNode != nullptr) {
		nodeBefore = reserveNode;
		nodeNext = nodeBefore->next;
		if (nodeBefore->data >= element) {
			auto node = new ListNode<T>;
			node->data = element;
			node->next = *listNode;
			*listNode = node;
			return;
		}
		if (nodeNext == nullptr || nodeNext->data >= element) {
			auto node = new ListNode<T>;
			node->data = element;
			node->next = nodeBefore->next;
			nodeBefore->next = node;
			return;
		}
		reserveNode = reserveNode->next;
	}
}

int main() {
	ListNode<int>* first = nullptr;
	ListCreate(&first, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	std::cout << "Original: ";
	ListPrint(first);
	ListDelete(&first, 9);
	ListDelete(&first, 10);
	std::cout << "Deleted 9 and 10: ";
	ListPrint(first);
	ListReserve(&first);
	std::cout << "Reversed: ";
	ListPrint(first);
	ListReserve(&first);
	std::cout << "Reversed: ";
	ListPrint(first);
	ListInsert(&first, 0);
	std::cout << "Insert:0 ";
	ListPrint(first);
	ListInsert(&first, 5);
	std::cout << "Insert:5 ";
	ListPrint(first);
	ListInsert(&first, 12);
	std::cout << "Insert:12 ";
	ListPrint(first);
	ListDeleteAll(&first, 5);
	std::cout << "DeleteAll: 5";
	ListPrint(first);
	return 0;
}