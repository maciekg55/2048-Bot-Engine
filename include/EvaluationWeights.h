#pragma once

struct EvaluationWeights {
    double emptyCells = 500;
    double monotonicity = 15;
    double snake = 10;
    double mergePotential = 20;
    double cornerBonus = 10;
    double maxTile = 50;
};