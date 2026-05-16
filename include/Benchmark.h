#pragma once
#include "Bot.h"
#include <vector>
#include <functional>

struct BenchmarkResult {
    std::string botName;
    int gamesPlayed;
    double average;
    double median;
    int best;
    int worst;
};

class Benchmark {
public:
    static BenchmarkResult run(std::function<std::unique_ptr<Bot>()> botFactory, int numberOfGames);
    static void print(const BenchmarkResult& result);
    static void saveToCSV(const BenchmarkResult& result, const std::string& filename = "benchmark_results.csv", const bool& isFirst = false);

};