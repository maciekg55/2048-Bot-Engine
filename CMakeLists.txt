cmake_minimum_required(VERSION 3.30)
project(2048DecisionEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3.0.2 REQUIRED COMPONENTS Graphics Window System)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/Board.cpp
        src/Game.cpp
        include/Game.h
        src/Engine.cpp
        include/Engine.h
        src/Bot.cpp
        include/Bot.h
        include/RandomBot.h
        src/GreedyBot.cpp
        include/GreedyBot.h
        src/RandomBot.cpp
        src/SearchBot.cpp
        include/SearchBot.h
        src/ExpectimaxBot.cpp
        include/ExpectimaxBot.h
        src/Benchmark.cpp
        include/Benchmark.h
        src/MCTSBot.cpp
        include/MCTSBot.h
        include/AppConfig.h
        src/Controller.cpp
        include/Controller.h
        include/EvaluationWeights.h
)

file(COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME}
        SFML::Graphics
        SFML::Window
        SFML::System
)