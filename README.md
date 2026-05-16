# 2048 Bot Engine

A C++ implementation of 2048 with multiple AI bots, a visual engine built on SFML, and a multithreaded benchmarking system for comparing bot performance.

---

## Project Structure

```
src/
├── Board          — core game logic, tile movement, scoring, evaluation
├── Game           — thin wrapper around Board, manages game lifecycle
├── Engine         — SFML window, game loop, bot tick management
├── Controller     — top level app entry, routes between visual and benchmark mode
├── AppConfig      — configuration struct for mode, bots, weights, tick speed
├── Benchmark      — headless multithreaded runner, CSV output
└── Bots/
    ├── RandomBot
    ├── GreedyBot
    ├── SearchBot
    ├── ExpectimaxBot
    └── MCTSBot
```

---

## Bots

All bots inherit from the `Bot` base class and implement `getNextMove(const Game&)`. They all share the same `Board::evaluate()` heuristic function.

### RandomBot
The baseline. Picks one of the four directions completely at random each turn. No lookahead, no evaluation. Useful as a lower bound in benchmarks.

**Average score:** ~500

---

### GreedyBot
Evaluates all four possible moves and picks the one that results in the highest immediately evaluated board state. No lookahead — purely depth 1. Functionally equivalent to SearchBot at depth 1.

**Average score:** ~3,000

---

### SearchBot
Builds a deterministic max search tree to a fixed depth. At each node it tries all four directions, recurses, and returns the maximum score found. No tile spawns occur during search — the tree is clean and deterministic.

```
getNextMove:
    for each direction:
        apply move (simulating=true, no spawn)
        score = search(board, depth-1)
    return direction with highest score

search(board, depth):
    if depth == 0: return evaluate()
    for each direction:
        apply move
        score = search(copy, depth-1)
    return max score
```

At depth 6 this explores `4^6 = 4096` board states per move. Fast but blind to randomness — it picks the best deterministic future without accounting for where tiles will actually spawn.

**Average score:** ~8,000

---

### ExpectimaxBot
The strongest bot. Extends the search tree with **chance nodes** that model the random tile spawns probabilistically. The tree alternates between two node types:

- **Max node** (player turn) — pick the direction with the highest expected score
- **Chance node** (tile spawn) — for every empty cell, simulate spawning a 2 (90% probability) and a 4 (10% probability), recurse, and return the **probability-weighted average**

```
getNextMove:
    for each direction:
        apply move (simulating=true)
        score = expectimax(board, depth-1, Chance, probability=1.0)
    return direction with highest score

expectimax(board, depth, nodeType, probability):
    if probability < 0.0001: return evaluate()   // pruning
    if depth == 0 or cant move: return evaluate()

    if Max:
        try all 4 directions, return highest score

    if Chance:
        for each empty cell:
            place 2 → recurse with probability * 0.9 * (1/emptyCells)
            place 4 → recurse with probability * 0.1 * (1/emptyCells)
            accumulate weighted average
        return totalScore
```

The probability parameter enables **pruning** — branches whose cumulative probability drops below `0.0001` are skipped entirely. This allows deeper search on critical low-empty-cell positions (endgame) while pruning unlikely branches early on open boards.

At depth 4 with pruning, this explores thousands of weighted futures per move and picks the direction that performs best **on average across all realistic tile spawns** — not just in the best case.

**Average score:** ~20,000  
**Peak observed:** ~53,000+

---

### MCTSBot (Monte Carlo Tree Search)
A fundamentally different approach. Instead of building an exhaustive tree, MCTS runs many **random playouts** from each candidate move within a fixed time budget and picks the move with the best average playout score.

```
getNextMove:
    while time < 100ms:
        pick direction with fewest playouts so far
        apply that move
        score = simulate(board, maxDepth=40)
        update stats for that direction

    return direction with highest average score

simulate(board, maxDepth):
    while canMove and depth < maxDepth:
        apply random direction
    return evaluate()
```

Each playout plays 40 random moves from the candidate position and evaluates the result. Over many playouts the average converges toward the true expected value for that move.

MCTS excels in games with huge branching factors (Go, Chess) where exhaustive search is impossible. In 2048 however the branching factor is only 4, making exhaustive probabilistic search (Expectimax) far more effective. MCTS is included for comparison and to illustrate why the algorithm choice matters.

**Average score:** ~2,500

---

## Board Evaluation

All bots share the same `Board::evaluate()` heuristic. It combines five components:

| Component | Description | Default Weight |
|---|---|---|
| Empty cells | More open space = more future options | 500 |
| Monotonicity | Penalises disorder between adjacent tiles | 15 |
| Snake weighting | Rewards tiles arranged in a snake pattern from corner | 10 |
| Merge potential | Rewards adjacent equal tiles ready to merge | 20 |
| Corner bonus | Bonus when the highest tile is in a corner | 10 |
| Max tile | Rewards reaching high tile values | 50 |

Weights are configurable via `EvalWeights` in `AppConfig` without recompiling.

The **snake pattern** used for weighting:
```
16  15  14  13
 9  10  11  12
 8   7   6   5
 1   2   3   4
```

Tiles that follow this ordering are rewarded — keeping the highest tile in the top-left corner and building downward in a snake encourages the structural consistency needed to reach 2048 and beyond.

---

## Benchmarking

The benchmark system runs N games per bot headlessly using `std::thread` for parallelism. Each thread owns its own bot instance and RNG to avoid data races.

```cpp
// in main.cpp
config.mode = Mode::Benchmark;
config.benchmarkGames = 100;
config.addBenchmarkBot<ExpectimaxBot>();
config.addBenchmarkBot<SearchBot>();
```

Results are printed to console and appended to `benchmark_results.csv` with a timestamp per run:

```
Run: Sat May 16 14:32:01 2026
Bot,Games,Average,Median,Best,Worst
Expectimax Bot,100,20140,16644,53176,3356
Search Bot,100,8432,8000,24576,1024
```

---

## Visual Mode

```cpp
config.mode = Mode::Visual;
config.visualTickMs = 120;   // ms between bot moves, set to 0 for max speed
config.addVisualBot<ExpectimaxBot>();
config.addVisualBot<SearchBot>();
config.addVisualBot<GreedyBot>();
```

Window width scales automatically based on the number of bots. Each game displays the bot name above the board and current score below.

---

## Building

Requires SFML 3.x and a C++20 compiler.

```bash
mkdir build && cd build
cmake ..
ninja
```
