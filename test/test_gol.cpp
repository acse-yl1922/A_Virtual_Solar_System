#include "catch.hpp"
#include "gol.h"
#include <stdexcept>

TEST_CASE("Check instantiation of class Grid", "[GOL]") {
    REQUIRE_NOTHROW(Grid(3, 4));
    
    REQUIRE_THROWS(Grid(0, 4));
    REQUIRE_THROWS(Grid(3, -2));
    REQUIRE_THROWS(Grid(-1, -5));

    REQUIRE(Grid(4, 5).data.size() == 4);
    REQUIRE(Grid(4, 5).data[0].size() == 5);

    REQUIRE_NOTHROW(Grid(3, 4, 5));
    REQUIRE_NOTHROW(Grid(3, 4, 0));
    REQUIRE_NOTHROW(Grid(3, 4, 12));
    
    REQUIRE_THROWS(Grid(0, 4, 2));
    REQUIRE_THROWS(Grid(3, -2, 2));
    REQUIRE_THROWS(Grid(-1, -5, 2));
    REQUIRE_THROWS(Grid(-1, -5, 2));
    REQUIRE_THROWS(Grid(3, 4, 13));

    int count = 0;
    Grid gol = Grid(4, 5, 6);
    REQUIRE(gol.data.size() == 4);
    REQUIRE(gol.data[0].size() == 5);
    for (auto &innerVec: gol.data){
        for (char &c: innerVec) {
            if (c == 'o') count++;
        }
    }
    REQUIRE(count == 6);
    
    REQUIRE_NOTHROW(Grid("/workspaces/PHAS0100Assignment1/test/data/glider.txt"));
    REQUIRE_THROWS(Grid("/workspaces/PHAS0100Assignment1/test/data/input.txt"));
    REQUIRE_THROWS(Grid("Error"));

    Grid glider = Grid("/workspaces/PHAS0100Assignment1/test/data/glider.txt");
    int num1 = glider.fetch_live(0, 0);
    int num2 = glider.fetch_live(2, 2);
    REQUIRE(num1 == 0);
    REQUIRE(num2 == 3);

    REQUIRE(glider.get(5, 5) == '-');
    glider.set(5, 5, 'o');
    REQUIRE(glider.get(5, 5) == 'o');
}

TEST_CASE("Check instantiation of class GameOfLife", "[GOL]") {
    char arr1[10][10] = {
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', 'o', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', 'o', 'o', '-', '-', '-', '-', '-', '-'},
        {'-', 'o', 'o', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'}
    };

    std::vector<std::vector<char>> glider_next;
    for (int i = 0; i < 10; i++) {
        std::vector<char> row(arr1[i], arr1[i] + 10);
        glider_next.push_back(row);
    }

    GameOfLife glider_ = GameOfLife("/workspaces/PHAS0100Assignment1/test/data/glider.txt");
    glider_.takeStep();
    bool isEqual1 = std::equal(glider_.data.begin(), glider_.data.end(), glider_next.begin(), glider_next.end());
    REQUIRE(isEqual1 == true);

    char arr2[10][10] = {
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', 'o', 'o', '-', '-', '-', '-', '-', '-', '-'},
        {'o', '-', '-', 'o', '-', '-', 'o', 'o', '-', '-'},
        {'-', 'o', 'o', '-', '-', '-', 'o', '-', 'o', '-'},
        {'-', '-', '-', '-', '-', '-', '-', 'o', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', '-', '-', '-', '-', '-', '-', '-', '-', '-'},
        {'-', 'o', '-', '-', '-', '-', 'o', 'o', '-', '-'},
        {'o', '-', 'o', '-', '-', '-', 'o', 'o', '-', '-'},
        {'-', 'o', '-', '-', '-', '-', '-', '-', '-', '-'}
    };

    std::vector<std::vector<char>> still_lifes_next;
    for (int i = 0; i < 10; i++) {
        std::vector<char> row(arr2[i], arr2[i] + 10);
        still_lifes_next.push_back(row);
    }

    GameOfLife still_lifes_ = GameOfLife("/workspaces/PHAS0100Assignment1/test/data/still_lifes.txt");
    still_lifes_.takeStep();
    bool isEqual2 = std::equal(still_lifes_.data.begin(), still_lifes_.data.end(), still_lifes_next.begin(), still_lifes_next.end());
    REQUIRE(isEqual2 == true);
}