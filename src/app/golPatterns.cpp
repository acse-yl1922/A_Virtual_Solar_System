#include <iostream>
#include <cstring>
#include "gol.h"

int main(int argc, char* argv[]) {
    if (argc != 5 || (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))) {
        std::cerr << "Usage: " << argv[0] << std::endl;
        std::cout << "Please type in the following parameters in order to find stationary patterns:\n"
                  << "[number of gird rows] "
                  << "[number of grid columns] "
                  << "[number of alive cells] "
                  << "[number of iterations]"<< std::endl;
        return 1;
    }

    if (argc == 5) {
        int row = std::stoi(argv[1]);
        int col = std::stoi(argv[2]);
        int num_alive = std::stoi(argv[3]);
        int generations = std::stoi(argv[4]);
        
        bool foundEqual = false;
        for (int i = 0; i < 10; i++) {
            GameOfLife gol = GameOfLife(row, col, num_alive);
            for (int j = 0; j < generations; j++) {
                GameOfLife last = gol;
                gol.takeStep();
                bool isEqual = std::equal(last.data.begin(), last.data.end(), gol.data.begin(), gol.data.end());
                bool allDead = true;
                for (auto &innerVec: gol.data){
                    for (char &c: innerVec) {
                        if (c == 'o') allDead = false;
                    }
                }
                if (isEqual == true && allDead == false) {
                    std::cout << "Find stationary pattern:\n";
                    gol.printGrid();
                    foundEqual = true;
                    break;
                }
            }
        }
        if (foundEqual == false) {
            std::cout << "No found stationary pattern in 10 tries." << std::endl;
        }

    }

}