#include "gol.h"
#include <random>
#include <fstream>
#include <stdexcept>

Grid::Grid(int row, int col): row(row), col(col), data(std::vector<std::vector<char>> (row, std::vector<char>(col))) {
    if (row < 1 || col < 1) {
        throw std::domain_error("Row and col should be positive integers.");
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            data[i][j] = dis(gen) == 0 ? '-' : 'o';
        }
    }
};

Grid::Grid(int row, int col, int num_alive): row(row), col(col), num_alive(num_alive), data(std::vector<std::vector<char>> (row, std::vector<char>(col, '-'))) {
    if (row < 1 || col < 1 || num_alive < 0) {
        throw std::domain_error("Row ,col and num_alive should be positive integers.");
    }

    if (num_alive > row * col) {
        throw std::range_error("num_alive should not be larger than the size of 2d-grid.");
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, row * col - 1);
    int count = num_alive;
    while (count > 0) {
        int idx = dis(gen);
        int i = int(idx / col);
        int j = idx % col;
        if (data[i][j] != 'o'){
            data[i][j] = 'o';
            count--;
        }
    }
};

Grid::Grid(std::string path) {
    std::fstream infile(path);
    std::string line;
    std::vector<std::vector<char>> mat;
    int rows = 0;

    if (!infile.is_open()) {
        throw std::invalid_argument("Unable to open the txt file: " + path);
    }

    if (infile.peek() == std::ifstream::traits_type::eof()) {
    throw std::runtime_error("File is empty: " + path);
    }

    while (std::getline(infile, line)) {
        std::vector<char> row;
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == 'o' || line[i] == '-') row.push_back(line[i]);
        }
        mat.push_back(row);
        rows++;
    }
    infile.close();
    int cols = mat[0].size();
    row = rows;
    col = cols;
    data = mat;
}

void Grid::printGrid(){
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            std::cout << data[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n==========================\n" << std::endl;
}

char Grid::get(int r, int c) {
    return data[r][c];
}

void Grid::set(int r, int c, char value) {
    data[r][c] = value;
}

int Grid::fetch_live(int r, int c) {
    std::vector<std::vector<char>> surround_mat = std::vector<std::vector<char>> (row + 2, std::vector<char>(col + 2, '-'));
    for (int i = 1; i < row + 1; i++) {
        for (int j = 1; j < col + 1; j++) {
            surround_mat[i][j] = data[i - 1][j - 1];
        }
    }

    int count = 0;
    
    if (surround_mat[r][c] == 'o') count++;
    if (surround_mat[r][c + 1] == 'o') count++;
    if (surround_mat[r][c + 2] == 'o') count++;
    if (surround_mat[r + 1][c] == 'o') count++;
    if (surround_mat[r + 1][c + 2] == 'o') count++;
    if (surround_mat[r + 2][c] == 'o') count++;
    if (surround_mat[r + 2][c + 1] == 'o') count++;
    if (surround_mat[r + 2][c + 2] == 'o') count++;

    return count;
}

void GameOfLife::takeStep() {
    std::vector<std::vector<char>> next = data;
    for (int i = 0; i < next.size(); i++) {
        for (int j = 0; j < next[0].size(); j++) {
            if (next[i][j] == '-' && fetch_live(i, j) == 3) next[i][j] = 'o';
            if (next[i][j] == 'o' && (fetch_live(i, j) == 2 || fetch_live(i, j) == 3)) next[i][j] = 'o';
            if (next[i][j] == 'o' && (fetch_live(i, j) < 2 || fetch_live(i, j) > 3)) next[i][j] = '-';
        }
    }
    data = next;
}