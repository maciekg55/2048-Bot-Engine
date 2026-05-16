#pragma once
#include <vector>
#include <memory>
#include "Bot.h"

enum class Mode {
    Visual,
    Benchmark
};

struct AppConfig {
    Mode mode = Mode::Visual;
    EvaluationWeights weights;
    int benchmarkGames = 100;
    int visualTickMs = 120;
    std::vector<std::function<std::unique_ptr<Bot>()>> benchmarkBots;
    std::vector<std::unique_ptr<Bot>> visualBots;

    template <typename BotType>
    void addVisualBot(){
        visualBots.push_back(std::make_unique<BotType>());
    }

    template<typename BotType>
    void addBenchmarkBot() {
        benchmarkBots.push_back([]() { return std::make_unique<BotType>(); });
    }
};