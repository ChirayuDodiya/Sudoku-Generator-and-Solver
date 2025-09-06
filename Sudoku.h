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
    vector<vector<set<char>>> possibility;
    mt19937 rng;
    uniform_int_distribution<int> dist{0, 6};
    void reseedRandom();
    bool isValidMove(vector<vector<char>>& b, int row, int col, char num);
    int countSolutions(vector<vector<char>>& b, int& count);
    bool fillGrid(int row, int col);
    void initializepossiblity();
    void updatepossiblityAfterMove(int row, int col, char num);
    bool isHumansolvable(vector<vector<char>> &board);
    //obviousSingle
    bool applyObviousSingle(vector<vector<char>>&tempboard,bool printReason);
    //HiddenSingle
    bool applyHiddenSingle(vector<vector<char>>&tempboard,bool printReason);
    //obviousPair
    bool applyObviousPair(vector<vector<char>>&tempboard,bool printReason);
    //pointingPair
    bool applyPointingPair(vector<vector<char>>&tempboard,bool printReason);
    void generateSudokuWithNClues(int n);
    void printBoard(vector<vector<char>>& s);
    bool isSolved(vector<vector<char>>& b);
    void solverhelper(vector<vector<char>>&board);
    vector<vector<char>> findSolution(vector<vector<char>> b);
};

#endif
