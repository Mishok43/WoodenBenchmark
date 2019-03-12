#pragma once
#include <chrono>
#include <iostream>

#define BENCHMARK(benchmarkLabel, func, ...){\
std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::high_resolution_clock::now();\
func(##__VA_ARGS__);\
std::chrono::time_point<std::chrono::steady_clock> finish = std::chrono::high_resolution_clock::now();\
std::chrono::duration<double, std::milli> elapsed = finish - start;\
std::cout << "Benchmark \"" << benchmarkLabel.c_str() << "\" <" << #func << ">. Elapsed: " << elapsed.count() << "ms" << std::endl;}

#define BENCHMARK_MEAN(benchmarkLabel, func, numbr_repeatings, ...)  { \
double mean = 0.0; \
for (std::size_t i = 0; i < numbr_repeatings; ++i) {\
std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::high_resolution_clock::now(); \
func(##__VA_ARGS__); \
std::chrono::time_point<std::chrono::steady_clock> finish = std::chrono::high_resolution_clock::now(); \
std::chrono::duration<double, std::milli> elapsed = finish - start; \
mean += elapsed.count()/numbr_repeatings;}\
std::cout << "Benchmark \"" << benchmarkLabel.c_str() << "\" <" << #func  << ">. Repeated: " << numbr_repeatings << " Elapsed: " << mean << "ms" << std::endl; }

