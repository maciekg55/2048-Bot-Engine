#include "../include/Controller.h"

Controller::Controller(AppConfig config) : _config(std::move(config)) {}

void Controller::run() {
    if (_config.mode == Mode::Visual) {
        Engine engine(std::move(_config.visualBots), std::move(_config.visualTickMs));
        engine.run();
    }else {
        bool isFirst = true;
        for (auto& factory : _config.benchmarkBots) {
            auto result = Benchmark::run(factory, _config.benchmarkGames);
            Benchmark::print(result);
            Benchmark::saveToCSV(result, "benchmark_results.csv", isFirst);
            isFirst = false;
        }
    }
}
