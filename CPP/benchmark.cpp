#include <iostream>
#include <chrono>
#include <set>
#include "ScapeGoatTree.hpp"

void benchmark_sequential_ops() {
    constexpr int N = 50000;  // ✅ Size that works

    // Sequential Insert
    auto start = std::chrono::high_resolution_clock::now();
    ScapeGoatTree<int> sgt;
    for (int i = 0; i < N; ++i) sgt.insert(i);
    auto end = std::chrono::high_resolution_clock::now();
    auto sgt_insert = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Sequential Search
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) sgt.search(i);
    end = std::chrono::high_resolution_clock::now();
    auto sgt_search = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Range Query (unique to your tree!)
    start = std::chrono::high_resolution_clock::now();
    int sum = sgt.sumInRange(0, N);
    end = std::chrono::high_resolution_clock::now();
    auto sgt_range = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // std::set comparison
    start = std::chrono::high_resolution_clock::now();
    std::set<int> stdset;
    for (int i = 0; i < N; ++i) stdset.insert(i);
    end = std::chrono::high_resolution_clock::now();
    auto set_insert = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) stdset.find(i);
    end = std::chrono::high_resolution_clock::now();
    auto set_search = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "=== Performance Benchmark (50K operations) ===\n\n";
    std::cout << "Insert:\n";
    std::cout << "  ScapeGoatTree: " << sgt_insert.count() << " ms\n";
    std::cout << "  std::set:      " << set_insert.count() << " ms\n\n";

    std::cout << "Search:\n";
    std::cout << "  ScapeGoatTree: " << sgt_search.count() << " ms\n";
    std::cout << "  std::set:      " << set_search.count() << " ms\n\n";

    std::cout << "Range Sum (ScapeGoat unique feature):\n";
    std::cout << "  Result: " << sum << " in " << sgt_range.count() << " μs\n";
}

int main() {
    benchmark_sequential_ops();
    return 0;
}