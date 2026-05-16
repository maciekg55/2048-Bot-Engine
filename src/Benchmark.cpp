#include "Benchmark.h"
#include <future>
#include <iostream>
#include <chrono>
#include <fstream>
#include <ctime>



BenchmarkResult Benchmark::run(std::function<std::unique_ptr<Bot>()> botFactory, int numberOfGames) {
    const int numberOfThreads = std::thread::hardware_concurrency();
    const int gamesPerThread = numberOfGames / numberOfThreads;

    std::vector<std::future<std::vector<int>>> futures;

    for (int thr = 0; thr < numberOfThreads; thr++) {
        futures.push_back(std::async(std::launch::async, [botFactory, gamesPerThread]() {
            auto bot = botFactory();
            std::vector<int> scores;

            for (int i = 0; i<gamesPerThread; i++) {
                Game game({0,0}, bot->getBotName());
                while (game.canMove()) {
                    auto move = bot->getNextMove(game);
                    game.processInput(move, false);
                }
                scores.push_back(game.getScore());
            }
            return scores;
        }));
    }

    std::vector<int> allScores;
    for (auto& ftr : futures) {
        auto scores = ftr.get();
        allScores.insert(allScores.end(), scores.begin(), scores.end());
    }

    std::sort(allScores.begin(), allScores.end());

    double average = std::accumulate(allScores.begin(), allScores.end(), 0.0) / allScores.size();

    double median = allScores[allScores.size() / 2];

    int best = allScores.back();
    int worst = allScores.front();

    return {botFactory()->getBotName(), static_cast<int>(allScores.size()), average, median, best, worst};

}

void Benchmark::print(const BenchmarkResult &result) {
    std::cout << "== " << result.botName << " ==\n";
    std::cout << "Games:   " << result.gamesPlayed << "\n";
    std::cout << "Average: " << result.average << "\n";
    std::cout << "Median:  " << result.median << "\n";
    std::cout << "Best:    " << result.best << "\n";
    std::cout << "Worst:   " << result.worst << "\n\n";
}

void Benchmark::saveToCSV(const BenchmarkResult &result, const std::string &filename, const bool& isFirst) {
    std::ofstream file(filename, std::ios::app);

    if (isFirst) {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::string timeStr = std::ctime(&t);
        timeStr.pop_back();
        file << "\nRun: " << timeStr << "\n";
        file << "Bot,Games,Average,Median,Best,Worst\n";
    }

    file << result.botName << "," << result.gamesPlayed << "," << result.average << "," << result.median << "," << result.best << "," << result.worst << "\n";
}


