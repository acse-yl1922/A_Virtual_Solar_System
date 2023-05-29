#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>
#include "gol.h"

void print_generations(GameOfLife gol, int generations) {
    gol.printGrid();
    for (int i = 0; i < generations; i++) {
        std::this_thread::sleep_for (std::chrono::seconds(2));
        std::cout << "Generation " << i + 1 << "\n";
        gol.takeStep();
        gol.printGrid();
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3 || (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))) {
        std::cerr << "Usage: " << argv[0] << std::endl;
        std::cout << "Please choose between the following two options:\n"
                  << "1. Specify a text file input:\n"
                  << "[path] [generations]\n"
                  << "2. Start with random cell contents for the initial conditions:\n"
                  << "[row] [col] [generations] or\n"
                  << "[row] [col] [num_alive] [generations]" <<std::endl;
        return 1;
    }

    if (argc == 3) {
        std::string path = argv[1];
        int generations = std::stoi(argv[2]);
        print_generations(GameOfLife(path), generations);
    }

    if (argc == 4) {
        int row = std::stoi(argv[1]);
        int col = std::stoi(argv[2]);
        int generations = std::stoi(argv[3]);
        print_generations(GameOfLife(row, col), generations);
    }

    if (argc == 5) {
        int row = std::stoi(argv[1]);
        int col = std::stoi(argv[2]);
        int num_alive = std::stoi(argv[3]);
        int generations = std::stoi(argv[4]);
        print_generations(GameOfLife(row, col, num_alive), generations);
    }
}