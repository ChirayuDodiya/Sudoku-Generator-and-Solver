#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>
#include <utility>

class Sudoku {
public:
    Sudoku();
    void generateRandomSudoku(int difficultyLevel);
    void inputCustomSudoku();
    void interactiveSolve();
    void getsolution();

private:
    std::vector<std::vector<char>> board, solution;
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist{0, 6};
    void reseedRandom();
    bool isValidMove(std::vector<std::vector<char>>& b, int row, int col, char num);
    int countSolutions(std::vector<std::vector<char>>& b, int& count);
    bool fillGrid(int row, int col);
    void generateSudokuWithNClues(int n);
    std::vector<std::vector<char>> findSolution(std::vector<std::vector<char>> b);
    bool isSolved(std::vector<std::vector<char>>& b);
    void printBoard(std::vector<std::vector<char>>& s);
};

#endif
