//
// Created by robcholz on 10/7/23.
//
#include <cstdlib>
#include <vector>
#include <iostream>
#include <memory>

#include "Murphy.hpp"


template<typename T>
struct linklist {
    T data = T(0);
    linklist* next = nullptr;
}; // 链表定义，T为任意类型


/**
 * @description 从vector创建一个带头节点的链表
 * @tparam T
 * @param head
 * @param vec
 */
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

/**
 * @description 打印链表
 * @tparam T
 * @param list
 */
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

/**
 * @description 删除链表中为x的元素 没有则不做修改
 * @tparam T
 * @param head
 * @param x
 */
template<typename T>
void delx(linklist<T>* head, T x) {
    auto node = head;
    linklist<T>* node_prev;
    while (node) {
        node_prev = node;
        node = node->next;
        if (node->data == x) {
            node_prev->next = node->next; // 将游标前一个节点的next指向游标的下一个节点地址
            ::free(node);
            return;
        } // 当游标data为x时
    }
}

/**
 * @description 倒置链表
 * @tparam T
 * @param head
 */
template<typename T>
void reverse(linklist<T>* head) {
    auto new_head = (linklist<T>*) ::malloc(sizeof(linklist<T>)); // 为了方便这里创建一个新头
    new_head->next = nullptr;
    linklist<T>* node;
    while (head->next) {
        node = head->next;
        head->next = head->next->next; // 取出头节点的下一个节点

        node->next = new_head->next;
        new_head->next = node; // 插入到新表
    }
    head->next = new_head->next;
    ::free(new_head); // 释放新头节点
}

/**
 * @description 升序插入x
 * @tparam T
 * @param head
 * @param x
 */
template<typename T>
void insert(linklist<T>* head, T x) {
    auto node = head;
    linklist<T>* node_prev;
    while (node) {
        node_prev = node;
        node = node->next;
        if (node->data >= x) {
            auto new_node = (linklist<T>*) ::malloc(sizeof(linklist<T>));
            new_node->data = x;
            new_node->next = node;
            node_prev->next = new_node;
            return;
        } // 没啥好说的
    }
}

/**
 * @description 删除所有值为x的节点
 * @tparam T
 * @param head
 * @param x
 */
template<typename T>
void delallx(linklist<T>* head, T x) {
    auto node = head;
    linklist<T>* node_prev;
    node_prev = node;
    node = node->next;
    while (node) {
        if (node->data == x) { // 搜索到了
            node_prev->next = node->next;
            ::free(node); // 删除node
            node = node_prev->next; // 将游标前移一个节点
        } else {
            node_prev = node;
            node = node->next;
        } // 继续遍历
    }
}

/**
 * @description 按照comp函数排序
 * @tparam T
 * @param head
 * @param comp 函数指针，自定义类型T的排序规则。可依据结构体内部数据自定义排序规则
 */
template<typename T>
void sort(linklist<T>* head, bool(* comp)(T a, T b)) {
    auto node = head;
    ::size_t length = 0;
    auto swap = [](linklist<T>* a, linklist<T>* b) {
        static T temp;
        temp = a->data;
        a->data = b->data;
        b->data = temp;
    }; // 交换两个节点数据。因为只在这个域中使用，所以这里用lambda函数
    node = node->next;
    while (node) {
        node = node->next;
        ++length;
    } // 获取链表长度
    linklist<T>* node_j;
    for (auto i = 0; i < length - 1; ++i) {
        node_j = head;
        for (auto j = 0; j < length - i - 1; ++j) {
            node_j = node_j->next;
            if (comp(node_j->data, node_j->next->data))
                swap(node_j, node_j->next);
        }
    } // 为了简单用冒泡排序
}

template<typename T>
linklist<T>* mergeAscend(linklist<T>* l1, linklist<T>* l2) {
    if (l1->next && !l2->next)
        return l1;
    if (!l1->next && l2->next)
        return l2;
    if (!l1->next && !l2->next)
        return nullptr;
    if (l1->next && l2->next && l1->next->data < l2->next->data) {
        auto k = l1->next;
        l1->next = l2->next;
        l2->next = k;
    }
    auto node1 = l1->next;
    auto node2 = l2->next;
    linklist<T>* l1_tail = nullptr;
    linklist<T>* l2_tail = nullptr;
    while (node1 && node2) {
        auto node1_next = node1->next;
        auto node2_next = node2->next;
        if (node1->data >= node2->data) {
            if (node2_next && node2->next->data >= node1->data) {
                l1->next = node1_next;

                node1->next = node2->next;
                node2->next = node1;
                node1 = node1_next;
                node2 = node2->next;
            } else
                node2 = node2_next;
        }
        l1_tail = node1;
        l2_tail = node2;
    }
    if (node2) {
        l2_tail->next = node1;
        return l2;
    }
    l1_tail->next = node2;
    return l1;
}

template<typename T>
linklist<T>* mergeDescend(linklist<T>* l1, linklist<T>* l2) {
    auto l = mergeAscend(l1, l2);
    reverse(l);
    return l;
}

/**
 * @description 倒数第k个节点
 * @tparam T
 * @param head
 * @param k
 * @return
 */
template<typename T>
linklist<T>* search(linklist<T>* head, T k) {
    ::size_t length = 1;
    auto node_front = head->next;
    auto node_back = head->next;
    while (node_front && length <= k) {
        node_front = node_front->next;
        ++length;
    }
    if (length < k) return nullptr;
    while (node_front) {
        node_back = node_back->next;
        node_front = node_front->next;
    }
    return node_back;
}

/**
 * @description 计算交集
 * @efficiency O(len1*len2)
 * @tparam T
 * @param l1
 * @param l2
 * @return 新链表，为l1 l2的交集
 */
template<typename T>
linklist<T>* intersection(linklist<T>* l1, linklist<T>* l2) {
    auto node_l1 = l1->next;
    auto node_l2 = l2->next;
    ::size_t len_l1 = 0, len_l2 = 0;
    auto new_list = (linklist<T>*) ::malloc(sizeof(linklist<T>));
    new_list->next = nullptr;
    auto indexOf = [](linklist<T>* head, T value) {
        auto node = head->next;
        while (node) {
            if (node->data == value)
                return true;
            node = node->next;
        }
        return false;
    }; // 计算value在不在链表head中
    while (node_l1) {
        node_l1 = node_l1->next;
        ++len_l1;
    } // l1长度
    while (node_l2) {
        node_l2 = node_l2->next;
        ++len_l2;
    } //l2长度
    node_l1 = l1;
    node_l2 = l2;
    for (auto i = 0; i < len_l1; ++i) {
        node_l1 = node_l1->next;
        node_l2 = l2;
        for (auto j = 0; j < len_l2; ++j) {
            node_l2 = node_l2->next;
            if (node_l1->data == node_l2->data && !indexOf(new_list, node_l1->data)) { //防止新链表中元素重复
                auto new_list_node = (linklist<T>*) ::malloc(sizeof(linklist<T>));
                new_list_node->data = node_l1->data;
                new_list_node->next = new_list->next;
                new_list->next = new_list_node;
            }
        }
    }
    return new_list;
}

/**
 * @description 将基数节点移动至链表前
 * @tparam T
 * @param head
 */
template<typename T>
void apart(linklist<T>* head) {
    // 构建新链表头，根据奇偶性分别放前面/后面。最后删除新表头
    if (!head) return; // 边界检查
    auto new_head = (linklist<T>*) ::malloc(sizeof(linklist<T>));
    new_head->next = nullptr;
    auto new_head_tail = new_head;
    auto new_head_push_front = [&](linklist<T>* node) {
        node->next = new_head->next;
        new_head->next = node;
    }; // 放前面
    auto new_head_push_back = [&](linklist<T>* node) {
        new_head_tail->next = node;
        node->next = nullptr;
        new_head_tail = node;
    }; // 放后面
    auto node = head->next;
    while (node) {
        auto node_next = node->next;
        node->next = nullptr;
        if ((node->data & 1) != 0) // 奇数
            new_head_push_front(node);
        else // 偶数
            new_head_push_back(node);
        node = node_next;
    } // 拆表
    head->next = new_head->next;
    ::free(new_head);
}

int main() {
    mur_profiler_module("Lab3");

    auto list_warp = std::make_unique<linklist<int>>();
    auto list = list_warp.get();
    create(list, {1, 1, 2, 3, 3, 4, 5, 5, 10, 20, 20});
    std::cout << "Original: ";
    print(list);
    std::cout << std::endl;
    mur_profiler_test_lambda(
            "Delete 1 5 3",
            true,
            [&]() {
                delx(list, 1);
                delx(list, 5);
                delx(list, 3);
                print(list);
                std::cout << std::endl;
            }
    );
    mur_profiler_test_lambda(
            "Reversed",
            true,
            [&]() {
                reverse(list);
                print(list);
                std::cout << std::endl;
            }
    );
    mur_profiler_test_lambda(
            "Delex 20 10 3 1",
            true,
            [&](){
                delallx(list, 20);
                delallx(list, 10);
                delallx(list, 3);
                delallx(list, 1);
                print(list);
                std::cout << std::endl;
            }
            );
    auto list1_warp = std::make_unique<linklist<int>>();
    auto list1 = list1_warp.get();
    mur_profiler_test_lambda(
            "Merge 1 6 and ascend",
            true,
            [&](){
                create(list1, {1, 6});
                reverse(list);
                list1 = mergeAscend(list1, list);
                print(list1);
                std::cout << std::endl;
            }
            );
    mur_profiler_test_lambda(
            "Merge 1 6 and Descend",
            true,
            [&](){
                auto list12_warp = std::make_unique<linklist<int>>();
                list1 = list12_warp.get();
                create(list1, {1, 6});
                list1 = mergeDescend(list, list1);
                print(list1);
                std::cout << std::endl;
            }
            );
    auto list2_warp = std::make_unique<linklist<int>>();
    auto list2 = list2_warp.get();
    mur_profiler_test_lambda(
            "Intersection",
            true,
            [&](){
                create(list2, {1, 6, 3, 2});
                std::cout << ": of ";
                print(list1);
                std::cout << " and ";
                print(list2);
                std::cout << " is ";
                list2 = intersection(list1, list2);
                print(list2);
                std::cout << std::endl;
            }
            );
    mur_profiler_test_lambda(
            "Search: last 1, 2",
            true,
            [&](){
                print(list2);
                std::cout << " is ";
                std::cout << search(list2, 1)->data << " ";
                std::cout << search(list2, 2)->data << std::endl;
            }
            );
    mur_profiler_test_lambda(
            "Relocate odd number",
            true,
            [&](){
                auto list3_warp = std::make_unique<linklist<int>>();
                auto list3 = list3_warp.get();
                create(list3, {12, 3, 4, 12, 3, 3, 7, 9, 0, 1});
                apart(list3);
                print(list3);
                std::cout << std::endl;
            }
            );
}