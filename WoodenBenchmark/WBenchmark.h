#pragma once
#include <chrono>
#include <iostream>

#define BENCHMARK(func, first_arg)   {\
std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::high_resolution_clock::now();\
func(first_arg);\
std::chrono::time_point<std::chrono::steady_clock> finish = std::chrono::high_resolution_clock::now();\
std::chrono::duration<double, std::milli> elapsed = finish - start;\
std::cout << "Benchmark <" << #func << ">. Elapsed: " << elapsed.count() << "ms" << std::endl;}


#define BENCHMARK_MEAN(func, numbr_repeatings, first_arg)  { \
double mean = 0.0; \
for (std::size_t i = 0; i < numbr_repeatings; ++i) {\
std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::high_resolution_clock::now(); \
func(first_arg); \
std::chrono::time_point<std::chrono::steady_clock> finish = std::chrono::high_resolution_clock::now(); \
std::chrono::duration<double, std::milli> elapsed = finish - start; \
mean += elapsed.count()/numbr_repeatings;}\
std::cout << "Benchmark <" << #func << ">. Repeated: " << numbr_repeatings << " Elapsed: " << mean << "ms" << std::endl; }

