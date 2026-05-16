#pragma once
#include "AppConfig.h"
#include "Engine.h"
#include "Benchmark.h"

class Controller {
private:
    AppConfig _config;

public:
    Controller(AppConfig config);
    void run();
};