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
#include <sstream>

using namespace std;

struct Cell {
    int row;
    int col;
};

class Sudoku {
public:
    Sudoku();
    void generateRandomSudoku(int difficultyLevel);
    
    const vector<vector<int>>& getBoard() const;
    const vector<vector<int>>& getSolution() const;
    void setBoardCell(int r, int c, int val);
    void clearSolution();
    
    bool solve();
    bool getHint(string& out_reason);
    bool sudokuValidator();
    bool isSolved(vector<vector<int>>& b);

private:
    vector<vector<int>> board, solution;
    vector<vector<int>> possibility;
    bool possibility_initialized;
    mt19937 rng;
    uniform_int_distribution<int> dist{0, 6};
    void reseedRandom();
    bool isValidMove(vector<vector<int>>& b, int row, int col, int num);
    void countSolutions(vector<vector<int>>& b, int& count);
    bool fillGrid(int row, int col);
    void initializepossiblity(vector<vector<int>>& b);
    void updatepossiblityAfterMove(int row, int col, int num);
    bool isHumansolvable();
    
    // obviousSingle
    bool applyObviousSingle(vector<vector<int>>&board,bool printReason, string& out_reason);
    // HiddenSingle
    bool applyHiddenSingle(vector<vector<int>>&board,bool printReason, string& out_reason);
    // obviousPair
    bool applyObviousPair(vector<vector<int>>&board,bool printReason, string& out_reason);
    // pointingPair
    bool applyPointingPair(vector<vector<int>>&board,bool printReason, string& out_reason);
    
    void generateSudokuWithNClues(int n);
    bool humansolver(vector<vector<int>>& b);
    vector<vector<int>> recursivesolver(vector<vector<int>>& b);
};

class SudokuUI {
public:
    void runMainMenu();
private:
    Sudoku sudoku;
    void printBoard(const vector<vector<int>>& s);
    void playInteractive();
    void getSolutionOfCustomSudoku();
    void inputCustomSudoku();
};

#endif
