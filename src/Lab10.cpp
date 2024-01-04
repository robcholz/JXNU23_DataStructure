//
// Created by robcholz on 1/2/24.
//
#include <string>
#include <fstream>
#include <iostream>
#include <stack>

#include "Path.h"
#include "Murphy.hpp"


struct linklist {
    int data;
    struct linklist* next = nullptr;
};

static constexpr auto TOTAL_CAPACITY = 500000;
static int disorderedStorage[TOTAL_CAPACITY];
static linklist list;

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
};

void profile(const char* name, void (* func)(int array[], size_t size)) {
    mur_profiler_output_lambda(
            (std::string(name) + " Size: 10000").c_str(),
            [&]() {
                func(disorderedStorage, 10000);
                return true;
            }
    );
    mur_profiler_output_lambda(
            (std::string(name) + " Size: 20000").c_str(),
            [&]() {
                func(disorderedStorage, 20000);
                return true;
            }
    );
    mur_profiler_output_lambda(
            (std::string(name) + " Size: 50000").c_str(),
            [&]() {
                func(disorderedStorage, 50000);
                return true;
            }
    );
    mur_profiler_output_lambda(
            (std::string(name) + " Size: 100000").c_str(),
            [&]() {
                func(disorderedStorage, 100000);
                return true;
            }
    );
    mur_profiler_output_lambda(
            (std::string(name) + " Size: 300000").c_str(),
            [&]() {
                func(disorderedStorage, 300000);
                return true;
            }
    );
    mur_profiler_output_lambda(
            (std::string(name) + " Size: 400000").c_str(),
            [&]() {
                func(disorderedStorage, 400000);
                return true;
            }
    );
    mur_profiler_output_lambda(
            (std::string(name) + " Size: 500000").c_str(),
            [&]() {
                func(disorderedStorage, 500000);
                return true;
            }
    );
}

void insertSort(int a[], size_t n) {
    int key, j, i;
    for (i = 1; i < n; i++) {
        key = a[i];
        j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j = j - 1;
        }
        a[j + 1] = key;
    }
}

int binarySearch(int a[], int item,
                 int low, int high) {
    if (high <= low)
        return (item > a[low]) ?
               (low + 1) : low;
    int mid = (low + high) / 2;
    if (item == a[mid])
        return mid + 1;

    if (item > a[mid])
        return binarySearch(a, item,
                            mid + 1, high);
    return binarySearch(a, item, low,
                        mid - 1);
}

void binaryInsertSort(int a[], size_t n) {
    int i, loc, j, k, selected;
    for (i = 1; i < n; ++i) {
        j = i - 1;
        selected = a[i];
        loc = binarySearch(a, selected, 0, j);
        while (j >= loc) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = selected;
    }
}

void shellSort(int a[], size_t n) {
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i += 1) {
            int temp = a[i];
            int j;
            for (j = i; j >= gap && a[j - gap] > temp; j -= gap)
                a[j] = a[j - gap];
            a[j] = temp;
        }
    }
}

void selectSort(int a[], size_t n) {
    size_t min_idx;
    for (auto i = 0; i < n - 1; i++) {
        min_idx = i;
        for (auto j = i + 1; j < n; j++) {
            if (a[j] < a[min_idx])
                min_idx = j;
        }
        if (min_idx != i) {
            int temp = a[min_idx];
            a[min_idx] = a[i];
            a[i] = temp;
        }
    }
}

void heapify(int arr[], int N, int i) {
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < N && arr[l] > arr[largest])
        largest = l;
    if (r < N && arr[r] > arr[largest])
        largest = r;
    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, N, largest);
    }
}

void heapSort(int a[], size_t n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(a, n, i);
    for (int i = n - 1; i > 0; i--) {
        swap(&a[0], &a[i]);
        heapify(a, i, 0);
    }
}

void bubbleSort(int a[], size_t n) {
    for (auto i = 0; i < n - 1; ++i) {
        for (auto j = 0; j < n - i - 1; ++j) {
            if (a[j] < a[j + 1]) {
                int temp;
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}

void quickSort(int a[], int low, int high) {
    const auto partition = [&](int arr[], int low, int high) {
        int pivot = arr[high];
        int i = (low - 1);
        for (auto j = low; j <= high; j++) {
            if (arr[j] < pivot) {
                i++;
                swap(&arr[i], &arr[j]);
            }
        }
        swap(&arr[i + 1], &arr[high]);
        return (i + 1);
    };
    if (low < high) {
        int pi = partition(a, low, high);
        quickSort(a, low, pi - 1);
        quickSort(a, pi + 1, high);
    }
}

void quickSortWrapper(int a[], size_t n) {
    quickSort(a, 0, n - 1);
}

void merge(int a[], int const left, int const mid,
           int const right) {
    int const subarr_1 = mid - left + 1;
    int const subarr_2 = right - mid;
    auto* l_arr = new int[subarr_1],
            * r_arr = new int[subarr_2];
    for (auto i = 0; i < subarr_1; i++)
        l_arr[i] = a[left + i];
    for (auto j = 0; j < subarr_2; j++)
        r_arr[j] = a[mid + 1 + j];
    auto index_subarr_1 = 0, index_subarr_2 = 0;
    int index_mer_arr = left;
    while (index_subarr_1 < subarr_1
           && index_subarr_2 < subarr_2) {
        if (l_arr[index_subarr_1]
            <= r_arr[index_subarr_2]) {
            a[index_mer_arr]
                    = l_arr[index_subarr_1];
            index_subarr_1++;
        } else {
            a[index_mer_arr]
                    = r_arr[index_subarr_2];
            index_subarr_2++;
        }
        index_mer_arr++;
    }
    while (index_subarr_1 < subarr_1) {
        a[index_mer_arr]
                = l_arr[index_subarr_1];
        index_subarr_1++;
        index_mer_arr++;
    }
    while (index_subarr_2 < subarr_2) {
        a[index_mer_arr]
                = r_arr[index_subarr_2];
        index_subarr_2++;
        index_mer_arr++;
    }
    delete[] l_arr;
    delete[] r_arr;
}

// begin is for left index and end is right index
// of the sub-array of arr to be sorted
void mergeSort(int array[], int const begin, int const end) {
    if (begin >= end)
        return;
    int mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merge(array, begin, mid, end);
}

void mergeSortWrapper(int a[], size_t n) {
    mergeSort(a, 0, n - 1);
}

int getMax(linklist* node) {
    int max_val = INT_MIN;
    linklist* temp = node;
    while (temp != nullptr) {
        max_val = std::max(max_val, temp->data);
        temp = temp->next;
    }
    return max_val;
}

void radixSort(linklist** head) {
    if (*head == nullptr) {
        return;
    }
    int max_val = getMax(*head);
    int digits = log10(max_val) + 1;
    for (int exp = 1; exp <= digits; exp++) {
        linklist* output = nullptr;
        linklist* temp = nullptr;
        linklist* buckets[10] = { nullptr };
        while (*head != nullptr) {
            int digit = ((*head)->data / exp) % 10;
            if (buckets[digit] == nullptr) {
                buckets[digit] = *head;
            } else {
                temp = buckets[digit];
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = *head;
            }

            *head = (*head)->next;
        }
        for (int i = 0; i < 10; i++) {
            if (buckets[i] != nullptr) {
                if (output == nullptr) {
                    output = buckets[i];
                    temp = output;
                } else {
                    temp->next = buckets[i];
                    temp = temp->next;
                }

                temp->next = nullptr;
            }
        }
        *head = output;
    }
}

void radixSortWrapper(int a[], size_t n) {
    radixSort(&list.next);
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

void initListData(int* arr, linklist* head) {
    auto node = head;
    for (size_t i = 0; i < TOTAL_CAPACITY; ++i) {
        auto new_node = new linklist();
        new_node->data = arr[i];
        node->next = new_node;
        node = new_node;
    }
}

int main() {
    initArrayData(
            disorderedStorage,
            DISORDERED_SAMPLE_PATH
    );
    initListData(disorderedStorage, &list);
    profile("Quick Sort", quickSortWrapper); // ok
    profile("Heap Sort", heapSort); // ok
    profile("Merge Sort", mergeSortWrapper); // ok
    profile("Shell Sort", shellSort); // ok
    profile("Binary Insertion Sort", binaryInsertSort); // ok
    profile("Select Sort", selectSort); // ok
    profile("Insertion Sort", insertSort); //ok
    profile("Bubble Sort", bubbleSort); // ok
    profile("Radix Sort", radixSortWrapper); // error TODO
    return 0;
}