//
// Created by robcholz on 1/3/24.
//
#ifndef JXNU23_DATASTRUCTURE_MURPHY_HPP
#define JXNU23_DATASTRUCTURE_MURPHY_HPP

#include <chrono>
#include <iostream>


void mur_profiler_output_lambda(const char* name, const std::function<bool()> &func) {
    std::string result;
    const auto start = std::chrono::high_resolution_clock::now();
    if (func())
        result = "Success";
    else
        result = "Unsuccess";
    const auto end = std::chrono::high_resolution_clock::now();

    const auto s = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    long long size;
    std::string unit;
    if (s.count() > 0) {
        size = s.count();
        unit = "s";
    } else {
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if (ms.count() > 0) {
            size = ms.count();
            unit = "ms";
        } else {
            const auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            if (us.count() > 0) {
                size = us.count();
                unit = "us";
            } else {
                const auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
                size = ns.count();
                unit = "ns";
            }
        }
    }
    std::cout << "[" << result << "] [" << name << "] Spent: " << size << unit << std::endl;
}

void mur_profiler_output(const char* name, bool (* func)()) {
    mur_profiler_output_lambda(name, func);
}


#endif //JXNU23_DATASTRUCTURE_MURPHY_HPP
