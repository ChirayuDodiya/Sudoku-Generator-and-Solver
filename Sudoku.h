#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>
#include <utility>
#include <set>

using namespace std;
class Sudoku {
public:
    Sudoku();
    void generateRandomSudoku(int difficultyLevel);
    void inputCustomSudoku();
    void interactiveSolve();
    void getsolution();
    
    private:
    vector<vector<char>> board, solution;
    vector<vector<int>> possibility;
    mt19937 rng;
    uniform_int_distribution<int> dist{0, 6};
    void reseedRandom();
    bool sudokuValidator(vector<vector<char>>&board);
    bool isValidMove(vector<vector<char>>& b, int row, int col, char num);
    void countSolutions(vector<vector<char>>& b, int& count);
    bool fillGrid(int row, int col);
    void initializepossiblity();
    void updatepossiblityAfterMove(int row, int col, char num);
    bool isHumansolvable(vector<vector<char>> &board);
    //obviousSingle
    bool applyObviousSingle(vector<vector<char>>&board,bool printReason);
    //HiddenSingle
    bool applyHiddenSingle(vector<vector<char>>&board,bool printReason);
    //obviousPair
    bool applyObviousPair(vector<vector<char>>&board,bool printReason);
    //pointingPair
    bool applyPointingPair(vector<vector<char>>&board,bool printReason);
    void generateSudokuWithNClues(int n);
    void printBoard(vector<vector<char>>& s);
    bool isSolved(vector<vector<char>>& b);
    void humansolver(vector<vector<char>>&board);
    vector<vector<char>> recursivesolver(vector<vector<char>> b);
};

#endif
