//
// Created by robcholz on 11/4/23.
//
#include <iostream>
#include <memory>
#include <vector>
#include <utility>
#include <stack>

#include "Murphy.hpp"


template<typename T>
struct linklist {
    T data = T(0);
    linklist* next = nullptr;
};

template<typename T>
void create(linklist<T>* head, const std::vector<T> &vec) {
    auto node = head;
    for (auto i = 0; i < vec.size(); ++i) {
        auto element = (linklist<T>*) ::malloc(sizeof(linklist<T>));
        element->next = nullptr;
        element->data = vec[i];
        node->next = element;
        node = element;
    }
}

template<typename T>
void print(T array[], size_t size) {
    std::cout << "Array[";
    for (auto i = 0; i < size; ++i)
        std::cout << " " << array[i];
    std::cout << " ]";
}

template<typename T>
void print(linklist<T>* list) {
    std::cout << "[ ";
    while (list) {
        list = list->next;
        if (list) {
            std::cout << list->data;
            std::cout << " ";
        }
    }
    std::cout << "]";
}


template<typename T>
T max(T* currentIndex, const T* finalIndex, T localMax) {
    if (currentIndex == finalIndex) // 出口条件 搜索指针等于数组尾指针
        return localMax;
    if (*currentIndex > localMax) // 搜索指针指向的元素与当前最大值比较
        localMax = *currentIndex;
    return max(++currentIndex, finalIndex, localMax); // 指针前移
}

template<typename T>
T max(T array[], size_t size) {
    return max<T>(array, &array[size - 1], array[0]);
}

template<typename T>
void relocate(const T* frontIndex, T* currentIndex, const T* backIndex) {
    if (currentIndex == backIndex)
        return;
    if ((*currentIndex & 1) == 0) { // 偶数
        T temp = *currentIndex;
        T* curr = currentIndex;
        while (curr != backIndex) {
            *curr = *(curr + 1);
            ++curr;
        }
        *curr = temp;
    } else {
        T temp = *currentIndex;
        T* curr = currentIndex;
        while (curr != frontIndex) {
            *curr = *(curr - 1);
            --curr;
        }
        *curr = temp;
    } // 奇数
    return relocate<T>(frontIndex, ++currentIndex, backIndex);
}

template<typename T>
void relocate(T array[], size_t size) {
    relocate<T>(array, array, &array[size - 1]);
}

template<typename T>
void bubbleSort(T* frontIndex, T* firstIndex, T* secondIndex, T* backIndex, bool swapped) {
    if (firstIndex == backIndex)
        return;
    if (secondIndex == (backIndex - firstIndex + frontIndex)) {
        if (!swapped)
            return;
        else
            return bubbleSort(frontIndex, ++firstIndex, frontIndex, backIndex, false);
    }
    if (*secondIndex > *(secondIndex + 1)) {
        T temp = *secondIndex;
        *secondIndex = *(secondIndex + 1);
        *(secondIndex + 1) = temp;
        swapped = true;
    } else
        swapped = false;
    return bubbleSort(frontIndex, firstIndex, ++secondIndex, backIndex, swapped);
}

template<typename T>
void bubbleSort(T array[], size_t size) {
    return bubbleSort(array, array, array, &array[size - 1], false);
}

template<typename T>
long binarySearch(T* frontIndex, T* left, T* right, T key) {
    if (left > right)
        return -1;
    size_t middle = (right - left) >> 1;
    if (*(left + middle) > key)
        return binarySearch(frontIndex, left, left + middle - 1, key);
    else if (*(left + middle) < key)
        return binarySearch(frontIndex, left + middle + 1, right, key);
    return (long) (left - frontIndex) + middle;
}

template<typename T>
long binarySearch(T array[], size_t size, T key) {
    return binarySearch(array, array, &array[size - 1], key);
}

template<typename T>
linklist<T>* max(linklist<T>* node, T localMax, linklist<T>* localMaxNode) {
    if (node == nullptr) // 出口条件 搜索指针等于尾指针
        return localMaxNode;
    if (node->data > localMax) { // 搜索指针指向的元素与当前最大值比较
        localMax = node->data;
        localMaxNode = node;
    }
    return max(node->next, localMax, localMaxNode); // 指针前移
}

template<typename T>
linklist<T>* max(linklist<T>* head) {
    if (head == nullptr)
        return nullptr;
    return max<T>(head, head->next->data, head->next);
}

template<typename T>
void delallx(linklist<T>* prev, linklist<T>* current, T x) {
    if (current == nullptr)
        return;
    if (current->data == x) {
        prev->next = current->next;
        ::free(current);
        return delallx(prev, prev->next, x);
    } // 搜索到了
    return delallx(prev->next, current->next, x);
}

template<typename T>
void delallx(linklist<T>* head, T x) {
    return delallx(head, head->next, x);
}

template<typename T>
void preserve(linklist<T>* prev, linklist<T>* node, T x, std::stack<std::pair<linklist<T>*, linklist<T>*>>* stack) {
    if (node == nullptr) {
        while (stack->size() > 1) {
            auto ele = stack->top();
            ele.first->next = ele.second->next;
            ::free(ele.second);
            stack->pop();
        }
        delete stack;
        return;
    }
    if (node->data == x)
        stack->push(std::make_pair(prev, node));
    return preserve(prev->next, node->next, x, stack);
} // 使用C++内置栈类型s

template<typename T>
void preserve(linklist<T>* prev, linklist<T>* node, T x, size_t count) {
    if (node == nullptr)
        return;
    if (node->data == x)
        ++count;
    preserve(prev->next, node->next, x, count);
    if (node->data == x && count > 1) {
        prev->next = node->next;
        ::free(node);
    }
} // 使用系统栈

template<typename T>
void preserve(linklist<T>* head, T x) {
    //return preserve<T>(head,head->next, x, new std::stack<std::pair<linklist<T>*, linklist<T>*>>);
    return preserve(head, head->next, x, 0);
}


int main() {
    mur_profiler_module("Lab5");
    int test[] = {1, 2, 3, 10, 5, 6, 7, 8, 9, 9, 9, 9, 9};

    mur_profiler_test_lambda(
            "1.求a[left..right]最大数",
            true,
            [&]() {
                std::cout << max(test, 13) << std::endl;
            }
    );
    mur_profiler_test_lambda(
            "2.调整[left..right]中的奇数到左边 偶数到右边",
            true,
            [&]() {
                std::cout << "Before: ";
                print(test, 13);
                relocate(test, 13);
                std::cout << std::endl << "After: ";
                print(test, 13);
                std::cout << std::endl;
            }
    );
    mur_profiler_test_lambda(
            "3.1.冒泡升序排序[left..right]",
            true,
            [&](){
                bubbleSort(test, 13);
                print(test, 13);
                std::cout<<std::endl;
            }
            );
    mur_profiler_test_lambda(
            "3.2.二分查找[left..right]中的key[8]",
            true,
            [&](){
                std::cout << binarySearch(test, 13, 8);
            }
            );
    std::unique_ptr<linklist<int>> list = std::make_unique<linklist<int>>();
    create(list.get(), {7, 2, 3, 4, 5, 6, 7, 7, 4, 4, 10, 7});
    std::cout << "原链表: ";
    print(list.get());
    std::cout << std::endl;
    mur_profiler_test_lambda(
            "4.1.查找链表最大值",
            true,
            [&](){
                std::cout << max(list.get())->data<<std::endl;
            }
    );
    mur_profiler_test_lambda(
            "4.2.删除链表所有x",
            true,
            [&](){
                delallx(list.get(), 7);
                print(list.get());
                std::cout<<std::endl;
            }
            );
    mur_profiler_test_lambda(
            "4.2.删除链表直到只剩一个x",
            true,
            [&](){
                std::unique_ptr<linklist<int>> list1 = std::make_unique<linklist<int>>();
                create(list1.get(), {7, 2, 3, 4, 5, 6, 7, 7, 4, 4, 10, 7});
                preserve(list1.get(), 7);
                print(list1.get());
                std::cout<<std::endl;
            }
            );
    return 0;
}