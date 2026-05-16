#include "Controller.h"
#include "ExpectimaxBot.h"
#include "SearchBot.h"
#include "GreedyBot.h"
#include "RandomBot.h"
#include "MCTSBot.h"

int main() {
    AppConfig config;

    // -- Evaluation Weight Tuning --
    //config.weights.snake = 20;

    //-- Visual mode --
    config.mode = Mode::Visual;
    config.addVisualBot<ExpectimaxBot>();
    config.addVisualBot<ExpectimaxBot>();

    // -- Visual mode - ms Visual Tick Time Tuning --
    config.visualTickMs = 50;

    // -- Benchmark mode --
    // config.mode = Mode::Benchmark;
    // config.benchmarkGames = 100;
    // config.addBenchmarkBot<ExpectimaxBot>();





    Controller controller(std::move(config));
    controller.run();
}


