#ifndef gol_h
#define gol_h

#include <iostream>
#include <vector>
#include <string>

class Grid {
    public:
    Grid(int row, int col);
    Grid(int row, int col, int num_alive);
    Grid(std::string path);

    void printGrid();
    void set(int r, int c, char value);
    char get(int r, int c);
    int fetch_live(int r, int c);

    std::vector<std::vector<char>> data;

    private:
    int row;
    int col;
    int num_alive;
};

class GameOfLife : public Grid {
    public:
    GameOfLife(int row, int col) : Grid(row, col) {};
    GameOfLife(int row, int col, int num_alive) : Grid(row, col, num_alive) {};
    GameOfLife(std::string path) : Grid(path) {};

    void takeStep();
};

#endif