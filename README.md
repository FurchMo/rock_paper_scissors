# Rock-Paper-Scissors Game

A console-based Rock-Paper-Scissors game built as a C++ practice project. Play against a computer opponent using 4 predictive AI models and `ncurses` for a polished terminal interface.

## Features

- **Interactive Gameplay**: Choose "Scissors" (1), "Rock" (2), "Paper" (3), or exit (4) via terminal menu.
- **Smart Computer**: Uses **4 predictive models** based on your past moves:
  | Model | Strategy |
  |-------|----------|
  | **0** | Predicts repeat/change patterns from last move |
  | **1** | Analyzes 3-7 move sequences |
  | **2** | Counters your **most frequent** choice |
  | **3** | Counters your **least frequent** choice |
- **Weighted Scoring**: Best-performing models get higher weight each round.
- **Full Stats**: Round-by-round history + win probability summary.

## Prerequisites

**C++11+ compiler** + **ncurses**:
| OS | Command |
|----|---------|
| **Ubuntu/Debian** | `sudo apt install libncurses5-dev libncursesw5-dev` |
| **Arch** | `sudo pacman -S ncurses` |
| **Fedora** | `sudo dnf install ncurses-devel` |
| **macOS** | `brew install ncurses` |

## 🚀 Quick Start

1. **Compile**:
   ```bash
   g++ -o output/main main.cpp src/algorithms.cpp -lncurses
   ```
2. **Run**:
    ```bash
    ./output/main
    ```
    
I know that the history can get larger than the console window, and that it will then print a bit weird. I'm just too lazy to fix it.

