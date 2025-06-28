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
    //last free cell
    bool applyNakedSingles(vector<vector<char>>&tempboard);
    //last remaining cell,last possible number,obvious single
    bool applyHiddenSingles(vector<vector<char>>&tempboard);
    //obvious pairs
    bool applyNakedPairs(vector<vector<char>>&tempboard);
    //pointing pairs
    bool applyPointingPairs(vector<vector<char>>&tempboard);
    void generateSudokuWithNClues(int n);
    void printBoard(vector<vector<char>>& s);
    bool isSolved(vector<vector<char>>& b);
    vector<vector<char>> findSolution(vector<vector<char>> b);
};

#endif
