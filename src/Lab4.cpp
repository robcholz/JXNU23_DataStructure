//
// Created by robcholz on 10/26/23.
//
#include <cstdio>
#include <iostream>
#include <memory>
#include <vector>

#include "Murphy.hpp"


template<typename T>
struct listnode {
    T data;
    listnode* next;
};

template<typename T>
struct sequence {
    T* str;
    size_t length;
};

template<typename T>
struct linkedlist {
    listnode<T>* head;
    size_t length;
};
using linkstring = linkedlist<char>;
using seqstring = sequence<char>;
template<typename T>
using linkstack = linkedlist<T>;

template<typename T>
void create(linkedlist<T>* list, const std::vector<T> &vec) {
    list->head = new listnode<T>;
    list->head->next = nullptr;
    list->length = vec.size();
    auto node = list->head;
    for (auto i = 0; i < vec.size(); ++i) {
        auto element = new listnode<T>;
        element->next = nullptr;
        element->data = vec[i];
        node->next = element;
        node = element;
    }
}

linkstring create_linkstr(const std::string &str) {
    linkstring s;
    s.length = str.length();
    s.head = new listnode<char>;
    s.head->next = nullptr;
    auto node = s.head;
    for (char i: str) {
        auto element = new listnode<char>;
        element->next = nullptr;
        element->data = i;
        node->next = element;
        node = element;
    }
    return s;
}

seqstring create_seqstr(const std::string &str) {
    seqstring s;
    s.length = str.length();
    s.str = new char[s.length];
    size_t index = 0;
    for (char i: str) {
        s.str[index] = i;
        ++index;
    }
    return s;
}

void lab4_print(listnode<char>* i) {
    if (i) {
        std::cout << i->data;
        lab4_print(i->next);
    } else
        return;
}

void lab4_print(listnode<size_t>* i) {
    if (i) {
        std::cout << i->data;
        lab4_print(i->next);
    } else
        return;
}


void print(const linkstring &str) {
    if (str.head)
        lab4_print(str.head->next);
}

void print(const linkedlist<size_t> &str) {
    if (str.head)
        lab4_print(str.head->next);
}

/****链栈****/
template<typename T>
void StackInit(linkstack<T>* stack) {
    stack->length = 0;
    stack->head = new listnode<T>;
    stack->head->next = nullptr;
}

template<typename T>
bool StackIsEmpty(linkstack<T>* stack) {
    return (stack->head->next == nullptr);
}

template<typename T>
T StackRead(linkstack<T>* stack) {
    if (!StackIsEmpty(stack))
        return stack->head->next->data;
    return T{0};
}

template<typename T>
void ___StackDisplay(listnode<T>* node) {
    if (node) {
        std::cout << node->data;
        return ___StackDisplay(node->next);
    }
}

template<typename T>
void StackDisplay(linkstack<T>* stack) {
    ___StackDisplay(stack->head->next);
}

template<typename T>
listnode<T>* StackPush(linkstack<T>* stack, T x) {
    auto node = new listnode<T>;
    node->data = x;
    node->next = stack->head->next;
    stack->head->next = node;
    ++stack->length;
    return stack->head->next;
}

template<typename T>
listnode<T>* StackPop(linkstack<T>* stack) {
    if (!StackIsEmpty(stack)) {
        auto pop = stack->head->next;
        stack->head->next = pop->next;
        pop->next = nullptr;
        --stack->length;
        return pop;
    }
    return nullptr;
}
/****链栈****/

/**
 * @description DEC2HEX
 * @param m DEC
 */
void Dto16(int m) {
    linkstack<char> s;
    StackInit(&s);
    printf("十进制%d对应的十六进制数是：", m);
    while (m) {
        int remainder = m % 16;
        char hexDigit;
        if (remainder < 10) {
            hexDigit = '0' + remainder;
        } else {
            hexDigit = 'A' + (remainder - 10);
        }
        StackPush(&s, hexDigit);
        m /= 16;
    }
    while (!StackIsEmpty(&s))
        std::cout << StackPop(&s)->data;
    std::cout << std::endl;
}

/**
 * @description 计算字符表达式的值 只支持+-* \/ % 不支持优先级运算 不支持一位以上的常数 表达式必须常数开头结尾
 * @param str
 */
void evaluate(linkstack<char> str) {
    if (StackIsEmpty(&str))
        return;
    int (* operatorArray[5])(int, char) ={
            [](int a, char b) -> int { return (a) + (b - '0'); },
            [](int a, char b) -> int { return (a) - (b - '0'); },
            [](int a, char b) -> int { return (a) * (b - '0'); },
            [](int a, char b) -> int { return (a) / (b - '0'); },
            [](int a, char b) -> int { return (a) % (b - '0'); }
    };
    auto const char2Index = [](char i) -> int {
        if (i == '+')return 0;
        if (i == '-')return 1;
        if (i == '*')return 2;
        if (i == '/')return 3;
        return 4;
    };
    auto isOperator = [](char get) -> bool {
        return (get == '+' || get == '-' || get == '*' || get == '/' || get == '%');
    };
    int base = (StackPop(&str)->data) - '0';
    while (!StackIsEmpty(&str)) {
        char get = StackPop(&str)->data;
        char get1 = StackPop(&str)->data;
        if (isOperator(get) && !isOperator(get1)) {
            base = operatorArray[char2Index(get)](base, get1);
        } else {
            std::cout << "非法的表达式！" << std::endl;
        }
    }
    std::cout << "表达式结果为：" << base << std::endl;
}

linkstring substring(linkstring str, size_t i, size_t len) {
    if (i + len > str.length)
        return linkstring{};
    auto head = new listnode<char>;
    head->next = nullptr;
    listnode<char>* tail;
    tail = head;
    auto node = str.head->next;
    for (auto j = 0; j < i; ++j) {
        node = node->next;
    }
    for (auto k = 0; k < len; ++k) {
        auto n = new listnode<char>;
        n->data = node->data;
        n->next = nullptr;
        tail->next = n;
        tail = n;
        node = node->next;
    }
    return {head, len};
}

/**
 * @description 删除从i开始，长度为len的子串
 * @param str
 * @param i
 * @param len
 */
void delstring(linkstring &str, size_t i, size_t len) {
    if (i + len > str.length)
        return;
    auto node = str.head->next;
    for (auto j = 0; j < (int) i - 1; ++j) {
        node = node->next;
    }
    auto index = 0;
    while (index < len) {
        auto de = node->next;
        node->next = de->next;
        delete de;
        --str.length;
        ++index;
    }
}

/**
 * @description 查找pattern在str中第一次出现的位置 朴素模式匹配
 * @return 第一次出现的位置
 */
int index(linkstring str, linkstring pattern) {
    if (str.length < pattern.length)
        return -1;
    auto node_s = str.head;
    auto node_s_mov = node_s;
    auto node_p = pattern.head;
    for (auto i = 0; i < str.length; ++i) {
        if (i + pattern.length > str.length)
            return -1;
        if (i > 0)
            node_s = node_s->next;
        node_s_mov = node_s;
        node_p = pattern.head;
        for (auto j = 0; j < pattern.length; ++j) {
            node_s_mov = node_s_mov->next;
            node_p = node_p->next;
            if (node_s_mov->data == node_p->data) {
                if (j == pattern.length - 1)
                    return i;
            } else
                j = pattern.length;
        }
    }
    return -1;
}

int index(seqstring text, seqstring pattern) {
    for (auto i = 0; i < text.length; ++i) {
        for (auto j = 0; j < pattern.length; ++j) {
            if (i + pattern.length > text.length)
                return -1; // 如果此次匹配超出text大小则结束
            if (text.str[i + j] == pattern.str[j]) {
                if (j == pattern.length - 1)
                    return i;
            } else
                j = pattern.length;
        }
    }
    return -1;
}

/**
 * @description 朴素模式匹配
 * @complexity O(m*n)
 * @param text
 * @param pattern
 * @return 模式pattern在text中所有出现的位置，结构为带头节点的单链表
 */
linkedlist<size_t> indexall(seqstring text, seqstring pattern) {
    auto head = new listnode<size_t>;
    size_t len = 0;
    head->next = nullptr;
    auto push_front = [&](size_t index) {
        auto node = new listnode<size_t>;
        node->data = index;
        node->next = head->next;
        head->next = node;
        ++len;
    }; // 加入链表
    for (auto i = 0; i < text.length; ++i) {
        for (auto j = 0; j < pattern.length; ++j) {
            if (i + pattern.length > text.length)
                return {head, len}; // 如果此次匹配超出text大小则结束
            if (text.str[i + j] == pattern.str[j]) {
                if (j == pattern.length - 1) {
                    push_front(i);
                }
            } else {
                j = pattern.length;
            }
        }
    }
    return {head, len};
}

void getnext(seqstring p, int next[]) {
    int ps_len = 0, i = 1;
    next[0] = 0;
    while (i < p.length) {
        if (p.str[ps_len] == p.str[i]) {
            ++ps_len;
            next[i] = ps_len;
            ++i;
        } else if (ps_len == 0) {
            next[i] = ps_len;
            ++i;
        } else
            ps_len = next[ps_len - 1];
    }
}

int kmp(seqstring t, seqstring p, int next[]) {
    int i = 0, j = 0;
    while (i < t.length) {
        if (t.str[i] == p.str[j]) {
            ++i;
            ++j;
        } else if (j > 0)
            j = next[j - 1];
        else
            ++i;
        if (j == p.length)
            return i - j;
    }
    return -1;
}

int main() {
    mur_profiler_module("Lab4");
    mur_profiler_test_lambda(
            "DEC转换HEX",
            true,
            [&]() {
                Dto16(255);
                Dto16(510);
            }
    );
    mur_profiler_test_lambda(
            "算数表达式求值：{'3','+','4','*','5'}",
            true,
            [&]() {
                auto eva_str_wrapper = std::make_unique<linkedlist<char>>();
                auto eva_str = eva_str_wrapper.get();
                create(eva_str, {'3', '+', '4', '*', '5'});
                evaluate(*eva_str);
            }
    );
    auto test_str = create_linkstr("test_string");
    mur_profiler_test_lambda(
            "子字符串：test_string, i=5,len=6",
            true,
            [&]() {
                auto substr = substring(test_str, 5, 6);
                print(substr);
                std::cout<<std::endl;
            }
    );
    mur_profiler_test_lambda(
            "删除子字符串：test_string, i=5,len=6",
            true,
            [&]() {
                delstring(test_str, 5, 6);
                print(test_str);
                std::cout<<std::endl;
            }
    );
    mur_profiler_test_lambda(
            "查找第一次出现的位置：test_, t=es",
            true,
            [&]() {
                std::cout << index(test_str, create_linkstr("es"))<<std::endl;
            }
    );
    mur_profiler_test_lambda(
            "朴素匹配",
            true,
            [&]() {
                std::cout << "第一次的位置：testes_es_es_, t=es"<<std::endl;
                std::cout << index(create_seqstr("test_"), create_seqstr("es"));
                std::cout<<std::endl;
                std::cout << "所有位置：testes_es_es_, t=es" << std::endl;
                print(indexall(create_seqstr("tes_es_es_es"), create_seqstr("es")));
                std::cout<<std::endl;
            }
    );
    mur_profiler_test_lambda(
            "KMP",
            true,
            [&]() {
                int next[20];
                getnext(create_seqstr("_ess"), next);
                std::cout << kmp(create_seqstr("tdes_es_ess_es"), create_seqstr("_ess"), next);
                std::cout<<std::endl;
            }
    );
    return 0;
}