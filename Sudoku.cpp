#include "Sudoku.h"

using namespace std;
using namespace std::chrono;

// Constructor
Sudoku::Sudoku()
{
    board.resize(9, vector<int>(9, 0));
    solution.clear();
    possibility.assign(9, vector<int>(9, 0));
    possibility_initialized = false;
}

// Calls generateSudokuWithNClues function
void Sudoku::generateRandomSudoku(int n)
{
    reseedRandom();
    if (n == 1) generateSudokuWithNClues(47 + dist(rng));
    else if (n == 2) generateSudokuWithNClues(40 + dist(rng));
    else generateSudokuWithNClues(33 + dist(rng));
    possibility_initialized = false;
}

// Getters and Setters
const vector<vector<int>>& Sudoku::getBoard() const
{
    return board;
}

const vector<vector<int>>& Sudoku::getSolution() const
{
    return solution;
}

void Sudoku::setBoardCell(int r, int c, int val)
{
    board[r][c] = val;
    possibility_initialized = false;
}

void Sudoku::clearSolution()
{
    solution.clear();
    possibility_initialized = false;
}

// Core Engine public interfaces
bool Sudoku::solve()
{
    if (!sudokuValidator()) return false;
    if (solution.empty())
    {
        int solutionCount = 0;
        vector<vector<int>> temp_board = board;
        countSolutions(temp_board, solutionCount);
        if (solutionCount > 1)
        {
            solution = {{1}};
            return false;
        }
        else if (solutionCount == 0)
        {
            solution = {{0}};
            return false;
        }
        else
        {
            temp_board = board;
            humansolver(temp_board);
            if (isSolved(temp_board))
            {
                solution = temp_board;
            }
            else
            {
                solution = recursivesolver(temp_board);
            }
        }
    }
    return true;
}

bool Sudoku::getHint(string& out_reason)
{
    if (!possibility_initialized)
    {
        initializepossiblity(board);
        possibility_initialized = true;
    }
    if (applyObviousSingle(board, true, out_reason)) return true;
    if (applyHiddenSingle(board, true, out_reason)) return true;
    if (applyObviousPair(board, true, out_reason)) return true;
    if (applyPointingPair(board, true, out_reason)) return true;
    return false;
}

bool Sudoku::sudokuValidator()
{
    // Check rows for duplicates
    for (int i = 0; i < 9; i++) 
    {
        int mask = 0;
        for (int j = 0; j < 9; j++) 
        {
            if (board[i][j] != 0) 
            {
                int val = board[i][j] - 1;
                if (val < 0 || val > 8) return false;
                if (mask & (1 << val)) return false;
                mask |= (1 << val);
            }
        }
    }
    
    // Check columns for duplicates
    for (int j = 0; j < 9; j++) 
    {
        int mask = 0;
        for (int i = 0; i < 9; i++) 
        {
            if (board[i][j] != 0) 
            {
                int val = board[i][j] - 1;
                if (mask & (1 << val)) return false;
                mask |= (1 << val);
            }
        }
    }
    
    // Check 3x3 boxes for duplicates
    for (int box = 0; box < 9; box++) 
    {
        int startRow = (box / 3) * 3;
        int startCol = (box % 3) * 3;
        int mask = 0;
        for (int r = startRow; r < startRow + 3; r++) 
        {
            for (int c = startCol; c < startCol + 3; c++) 
            {
                if (board[r][c] != 0) 
                {
                    int val = board[r][c] - 1;
                    if (mask & (1 << val)) return false;
                    mask |= (1 << val);
                }
            }
        }
    }
    return true;
}

// Reseed RNG
void Sudoku::reseedRandom() 
{
    rng.seed(steady_clock::now().time_since_epoch().count());
}

// Checks if it is valid to put num at board[row][col]
bool Sudoku::isValidMove(vector<vector<int>> &b, int row, int col, int num)
{
    for (int j = 0; j < 9; j++)
    {
        if (b[row][j] == num) return false;
    }
    for (int i = 0; i < 9; i++)
    {
        if (b[i][col] == num) return false;
    }
    
    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int i = startRow; i < startRow + 3; i++)
    {
        for (int j = startCol; j < startCol + 3; j++)
        {
            if (b[i][j] == num) return false;
        }
    }
    return true;
}

// Human Solvability Check
bool Sudoku::isHumansolvable()
{       
    vector<vector<int>> tempboard = board;
    return humansolver(tempboard);
}

// Unified Human Solver
bool Sudoku::humansolver(vector<vector<int>>& b)
{       
    initializepossiblity(b);
    
    bool progress = true;
    int iterations = 0;
    const int MAX_ITERATIONS = 100;
    string dummy_reason;
    
    while (progress && !isSolved(b) && iterations < MAX_ITERATIONS) {
        progress = false;
        iterations++;
        
        if (applyObviousSingle(b, false, dummy_reason)) 
        {
            progress = true;
            continue;
        }
        
        if (applyHiddenSingle(b, false, dummy_reason)) 
        {
            progress = true;
            continue;
        }
        
        if (applyObviousPair(b, false, dummy_reason)) 
        {
            progress = true;
            continue;
        }
        
        if (applyPointingPair(b, false, dummy_reason)) 
        {
            progress = true;
            continue;
        }
    }
    return isSolved(b);
}

// Add all possible possibility for empty cells in board
void Sudoku::initializepossiblity(vector<vector<int>>& b) 
{
    possibility.assign(9, vector<int>(9, 0));
    for (int row = 0; row < 9; row++) 
    {
        for (int col = 0; col < 9; col++) 
        {
            if (b[row][col] == 0) 
            {
                for (int num = 1; num <= 9; num++) 
                {
                    if (isValidMove(b, row, col, num)) 
                    {
                        possibility[row][col] |= (1 << (num - 1));
                    }
                }
            }
        }
    }
}

// Update possibility after correct move
void Sudoku::updatepossiblityAfterMove(int row, int col, int num) 
{
    possibility[row][col] = 0;
    int bit = 1 << (num - 1);
    int mask = ~bit;
    
    for (int c = 0; c < 9; c++) 
    {
        possibility[row][c] &= mask;
    }
    for (int r = 0; r < 9; r++) 
    {
        possibility[r][col] &= mask;
    }
    
    int box_row = (row / 3) * 3;
    int box_col = (col / 3) * 3;
    for (int r = box_row; r < box_row + 3; r++) 
    {
        for (int c = box_col; c < box_col + 3; c++) 
        {
            possibility[r][c] &= mask;
        }
    }
}

// If cell is empty and has only one candidate
bool Sudoku::applyObviousSingle(vector<vector<int>>& b, bool printReason, string& out_reason) 
{
    bool found = false;
    
    for (int row = 0; row < 9; row++) 
    {
        for (int col = 0; col < 9; col++) 
        {
            int mask = possibility[row][col];
            if (b[row][col] == 0 && mask != 0 && (mask & (mask - 1)) == 0) 
            {
                int lsb = 0;
                while ((mask & (1 << lsb)) == 0) {
                    lsb++;
                }
                int num = 1 + lsb;
                b[row][col] = num;
                updatepossiblityAfterMove(row, col, num);
                found = true;
                if (printReason)
                {
                    stringstream ss;
                    ss << "Hint:Obvious Single\n";
                    ss << "at index " << row + 1 << " " << col + 1 << " there is only one possible num " << num << ".";
                    out_reason = ss.str();
                    return found;
                }
            }
        }
    }
    return found;
}

// If only one position possible, it's a hidden single
bool Sudoku::applyHiddenSingle(vector<vector<int>>& b, bool printReason, string& out_reason) 
{
    bool found = false;
    
    // Check rows for hidden singles
    for (int row = 0; row < 9; row++) 
    {
        for (int num = 1; num <= 9; num++) 
        {
            vector<int> possible_cols;
            int bit = 1 << (num - 1);
            
            for (int col = 0; col < 9; col++) 
            {
                if (b[row][col] == 0 && (possibility[row][col] & bit)) 
                {
                    possible_cols.push_back(col);
                }
            }
            if (possible_cols.size() == 1) 
            {
                int col = possible_cols[0];
                b[row][col] = num;
                updatepossiblityAfterMove(row, col, num);
                found = true;
                if (printReason)
                {
                    stringstream ss;
                    ss << "Hint:Hidden Single\n";
                    ss << "for row " << row + 1 << " there is only one possible column " << possible_cols[0] + 1 << " where you can insert " << num;
                    out_reason = ss.str();
                    return found;
                }
            }
        }
    }
    
    // Check columns for hidden singles
    for (int col = 0; col < 9; col++) 
    {
        for (int num = 1; num <= 9; num++) 
        {
            vector<int> possible_rows;
            int bit = 1 << (num - 1);
            
            for (int row = 0; row < 9; row++) 
            {
                if (b[row][col] == 0 && (possibility[row][col] & bit)) 
                {
                    possible_rows.push_back(row);
                }
            }
            if (possible_rows.size() == 1) 
            {
                int row = possible_rows[0];
                b[row][col] = num;
                updatepossiblityAfterMove(row, col, num);
                found = true;
                if (printReason)
                {
                    stringstream ss;
                    ss << "Hint:Hidden Single\n";
                    ss << "for column " << col + 1 << " there is only one possible row " << possible_rows[0] + 1 << " where you can insert " << num;
                    out_reason = ss.str();
                    return found;
                }
            }
        }
    }
    
    // Check boxes for hidden singles
    for (int box = 0; box < 9; box++) 
    {
        int box_row = (box / 3) * 3;
        int box_col = (box % 3) * 3;
        
        for (int num = 1; num <= 9; num++) 
        {
            vector<pair<int, int>> possible_cells;
            int bit = 1 << (num - 1);
            
            for (int r = box_row; r < box_row + 3; r++) 
            {
                for (int c = box_col; c < box_col + 3; c++) 
                {
                    if (b[r][c] == 0 && (possibility[r][c] & bit)) 
                    {
                        possible_cells.push_back({r, c});
                    }
                }
            }
            if (possible_cells.size() == 1) 
            {
                int row = possible_cells[0].first;
                int col = possible_cells[0].second;
                b[row][col] = num;
                updatepossiblityAfterMove(row, col, num);
                found = true;
                if (printReason)
                {
                    stringstream ss;
                    ss << "Hint:Hidden Single\n";
                    ss << "for box " << box + 1 << " there is only one possible row,column " << row + 1 << " " << col + 1 << " where you can insert " << num;
                    out_reason = ss.str();
                    return found;
                }
            }
        }
    }
    return found;
}

// Obvious Pair
bool Sudoku::applyObviousPair(vector<vector<int>>& b, bool printReason, string& out_reason)
{
    bool found = false;
    
    auto countCandidates = [](int mask) {
        int count = 0;
        while (mask) {
            count += (mask & 1);
            mask >>= 1;
        }
        return count;
    };
    
    // Check rows
    for (int row = 0; row < 9; row++) 
    {
        for (int col1 = 0; col1 < 8; col1++) 
        {
            for (int col2 = col1 + 1; col2 < 9; col2++) 
            {
                if (b[row][col1] == 0 && b[row][col2] == 0 &&
                    countCandidates(possibility[row][col1]) == 2 &&
                    possibility[row][col1] == possibility[row][col2]) 
                {
                    int mask = possibility[row][col1];
                    bool row_changed = false;
                    for (int c = 0; c < 9; c++) 
                    {
                        if (c != col1 && c != col2 && b[row][c] == 0)
                        {
                            for (int i = 0; i < 9; i++)
                            {
                                int bit = 1 << i;
                                if ((mask & bit) && (possibility[row][c] & bit))
                                {
                                    possibility[row][c] &= ~bit;
                                    row_changed = true;
                                    found = true;
                                }
                            }
                        }
                    }
                    if (row_changed && printReason)
                    {   
                        int first = -1, second = -1;
                        for (int i = 0; i < 9; i++) {
                            if (mask & (1 << i)) {
                                if (first == -1) first = i;
                                else second = i;
                            }
                        }
                        int first_val = 1 + first;
                        int second_val = 1 + second;
                        stringstream ss;
                        ss << "Hint:Obvious Pair\n";
                        ss << "for row " << row + 1 << " there is only two columns " << col1 + 1 << " " << col2 + 1 << " where you can insert " << first_val << " and " << second_val;
                        out_reason = ss.str();
                        return found;
                    }
                }
            }
        }
    }
    
    // Check columns
    for (int col = 0; col < 9; col++) 
    {
        for (int row1 = 0; row1 < 8; row1++) 
        {
            for (int row2 = row1 + 1; row2 < 9; row2++) 
            {
                if (b[row1][col] == 0 && b[row2][col] == 0 &&
                    countCandidates(possibility[row1][col]) == 2 &&
                    possibility[row1][col] == possibility[row2][col]) 
                {
                    int mask = possibility[row1][col];
                    bool col_changed = false;
                    for (int r = 0; r < 9; r++) 
                    {
                        if (r != row1 && r != row2 && b[r][col] == 0) 
                        {
                            for (int i = 0; i < 9; i++) 
                            {
                                int bit = 1 << i;
                                if ((mask & bit) && (possibility[r][col] & bit)) 
                                {
                                    possibility[r][col] &= ~bit;
                                    col_changed = true;
                                    found = true;
                                }
                            }
                        }
                    }
                    if (col_changed && printReason)
                    {   
                        int first = -1, second = -1;
                        for (int i = 0; i < 9; i++) {
                            if (mask & (1 << i)) {
                                if (first == -1) first = i;
                                else second = i;
                            }
                        }
                        int first_val = 1 + first;
                        int second_val = 1 + second;
                        stringstream ss;
                        ss << "Hint:Obvious Pair\n";
                        ss << "for column " << col + 1 << " there is only two rows " << row1 + 1 << " " << row2 + 1 << " where you can insert " << first_val << " and " << second_val;
                        out_reason = ss.str();
                        return found;
                    }
                }
            }
        }
    }
    
    // Check boxes
    for (int box = 0; box < 9; box++) 
    {
        int box_row = (box / 3) * 3;
        int box_col = (box % 3) * 3;
        
        vector<pair<int, int>> empty_cells;
        for (int r = box_row; r < box_row + 3; r++) 
        {
            for (int c = box_col; c < box_col + 3; c++) 
            {
                if (b[r][c] == 0) empty_cells.push_back({r, c});
            }
        }
        
        for (size_t i = 0; i < empty_cells.size() - 1; i++) 
        {
            for (size_t j = i + 1; j < empty_cells.size(); j++) 
            {
                int r1 = empty_cells[i].first, c1 = empty_cells[i].second;
                int r2 = empty_cells[j].first, c2 = empty_cells[j].second;
                
                if (countCandidates(possibility[r1][c1]) == 2 &&
                    possibility[r1][c1] == possibility[r2][c2]) 
                {
                    int mask = possibility[r1][c1];
                    bool box_changed = false;
                    for (auto& cell : empty_cells) 
                    {
                        int r = cell.first, c = cell.second;
                        if ((r != r1 || c != c1) && (r != r2 || c != c2)) 
                        {
                            for (int i = 0; i < 9; i++) 
                            {
                                int bit = 1 << i;
                                if ((mask & bit) && (possibility[r][c] & bit)) 
                                {
                                    possibility[r][c] &= ~bit;
                                    box_changed = true;
                                    found = true;
                                }
                            }
                        }
                    }
                    if (box_changed && printReason)
                    {   
                        int first = -1, second = -1;
                        for (int i = 0; i < 9; i++) {
                            if (mask & (1 << i)) {
                                if (first == -1) first = i;
                                else second = i;
                            }
                        }
                        int first_val = 1 + first;
                        int second_val = 1 + second;
                        stringstream ss;
                        ss << "Hint:Obvious Pair\n";
                        ss << "for box " << box + 1 << " there is only two sub-box (" << r1 + 1 << "," << c1 + 1 << ") , (" << r2 + 1 << "," << c2 + 1 << ") where you can insert " << first_val << " and " << second_val;
                        out_reason = ss.str();
                        return found;
                    }
                }
            }
        }
    }
    return found;
}

// Pointing Pair
bool Sudoku::applyPointingPair(vector<vector<int>>& b, bool printReason, string& out_reason)
{
    bool found = false;
    
    for (int box = 0; box < 9; box++) 
    {
        int box_row = (box / 3) * 3;
        int box_col = (box % 3) * 3;
        
        for (int num = 1; num <= 9; num++) 
        {
            int row_mask = 0;
            int col_mask = 0;
            int bit = 1 << (num - 1);
            
            for (int r = box_row; r < box_row + 3; r++) 
            {
                for (int c = box_col; c < box_col + 3; c++) 
                {
                    if (b[r][c] == 0 && (possibility[r][c] & bit)) 
                    {
                        row_mask |= (1 << r);
                        col_mask |= (1 << c);
                    }
                }
            }
            
            if (row_mask != 0 && (row_mask & (row_mask - 1)) == 0) 
            {
                int row = 0;
                while ((row_mask & (1 << row)) == 0) {
                    row++;
                }
                for (int c = 0; c < 9; c++) 
                {
                    if ((c < box_col || c >= box_col + 3) && 
                        (b[row][c] == 0) && 
                        (possibility[row][c] & bit)) 
                    {
                        possibility[row][c] &= ~bit;
                        found = true;
                    }
                }
                if (found && printReason)
                {   
                    stringstream ss;
                    ss << "Hint:Pointing Pair\n";
                    ss << "for box " << box + 1 << " there is only one row " << row + 1 << " where you can insert " << num;
                    out_reason = ss.str();
                    return found;
                }
            }
            
            if (col_mask != 0 && (col_mask & (col_mask - 1)) == 0) 
            {
                int col = 0;
                while ((col_mask & (1 << col)) == 0) {
                    col++;
                }
                for (int r = 0; r < 9; r++) 
                {
                    if ((r < box_row || r >= box_row + 3) && 
                        (b[r][col] == 0) && 
                        (possibility[r][col] & bit)) 
                    {
                        possibility[r][col] &= ~bit;
                        found = true;
                    }
                }
                if (found && printReason)
                {   
                    stringstream ss;
                    ss << "Hint:Pointing Pair\n";
                    ss << "for box " << box + 1 << " there is only one column " << col + 1 << " where you can insert " << num;
                    out_reason = ss.str();
                    return found;
                }
            }
        }
    }
    return found;
}

// Solutions Count
void Sudoku::countSolutions(vector<vector<int>> &b, int& count)
{
    if (count > 1) return;
    
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (b[i][j] == 0)
            {
                for (int num = 1; num <= 9; num++)
                {
                    if (isValidMove(b, i, j, num))
                    {
                        b[i][j] = num;
                        countSolutions(b, count);
                        b[i][j] = 0;
                        if (count > 1) return;
                    }
                }
                return;
            }
        }
    }
    count++;
}

// Fill Grid
bool Sudoku::fillGrid(int row, int col) 
{
    if (row == 9) return true;
    if (col == 9) return fillGrid(row + 1, 0);
    
    vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    shuffle(nums.begin(), nums.end(), rng);
    for (int num : nums) 
    {
        if (isValidMove(solution, row, col, num)) 
        {
            solution[row][col] = num;
            if (fillGrid(row, col + 1)) return true;
            solution[row][col] = 0;
        }
    }
    return false;
}

// Generate N Clues Puzzle
void Sudoku::generateSudokuWithNClues(int n) 
{
    solution.assign(9, vector<int>(9, 0));
    fillGrid(0, 0);
    board = solution;
    vector<pair<int, int>> cells;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            cells.emplace_back(i, j);
        }
    }
    
    shuffle(cells.begin(), cells.end(), rng);

    int totalCells = 81;
    for (size_t i = 0; i < cells.size(); ++i) 
    {
        int row = cells[i].first;
        int col = cells[i].second;
        if (totalCells <= n) break;
        int backup = board[row][col];
        board[row][col] = 0;
        int count = 0;
        countSolutions(board, count);
        if ((count != 1) || (!isHumansolvable()))
        {
            board[row][col] = backup;
        }
        else
        {
            totalCells--;
        }
    }
}

// Recursive solver backtracking
vector<vector<int>> Sudoku::recursivesolver(vector<vector<int>>& b)
{
    initializepossiblity(b);
    
    int min_candidates = 10;
    Cell target_cell = {-1, -1};
    
    auto countCandidates = [](int mask) {
        int count = 0;
        while (mask) {
            count += (mask & 1);
            mask >>= 1;
        }
        return count;
    };
    
    for (int r = 0; r < 9; r++) 
    {
        for (int c = 0; c < 9; c++) 
        {
            if (b[r][c] == 0) 
            {
                int cnt = countCandidates(possibility[r][c]);
                if (cnt == 0) return {};
                if (cnt < min_candidates) 
                {
                    min_candidates = cnt;
                    target_cell = {r, c};
                }
            }
        }
    }
    
    if (target_cell.row == -1) return b;
    
    int mask = possibility[target_cell.row][target_cell.col];
    for (int i = 0; i < 9; i++) 
    {
        if (mask & (1 << i)) 
        {
            int num = 1 + i;
            vector<vector<int>> backup = b;
            b[target_cell.row][target_cell.col] = num;
            vector<vector<int>> result = recursivesolver(b);
            if (!result.empty() && result.size() == 9) return result;
            b = backup;
        }
    }
    return {};
}

// Checks if board is solved
bool Sudoku::isSolved(vector<vector<int>>& b)
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (b[i][j] == 0) return false;
        }
    }
    return true;
}

// ==========================================
//           SUDOKUUI IMPLEMENTATION
// ==========================================

void SudokuUI::runMainMenu()
{
    int choice;
    do {
        cout << "welcome to sudoku solver" << endl;
        cout << "type 1 to solve random sudoku" << endl;
        cout << "type 2 to solve custom sudoku" << endl;
        cout << "type 3 to get solution of custom sudoku" << endl;
        cout << "type 4 to exit" << endl;
        cin >> choice;
        switch (choice)
        {
        case 1:
            {
                int difficulty;
                cout << "Select difficulty:" << endl;
                cout << "1. Easy (47-53 clues)" << endl;
                cout << "2. Medium (40-46 clues)" << endl;
                cout << "3. Hard (33-39 clues)" << endl;
                cout << "Enter choice: ";
                cin >> difficulty;
                while (difficulty < 1 || difficulty > 3)
                {
                    cout << "Invalid choice. Please enter 1, 2, or 3: ";
                    cin >> difficulty;
                }
                sudoku.generateRandomSudoku(difficulty);
                playInteractive();
                break;
            }
        case 2:
            inputCustomSudoku();
            playInteractive();
            break;
        case 3:
            inputCustomSudoku();
            getSolutionOfCustomSudoku();
            break;
        case 4:
            cout << "thanks for using Sudoku Generator and Solver" << endl;
            break;
        default:
            cout << "invalid choice,please try again" << endl;
            break;
        }
    } while (choice != 4);
}

void SudokuUI::printBoard(const vector<vector<int>>& s)
{
    auto displayChar = [](int val) {
        return (val == 0) ? ' ' : static_cast<char>(val + '0');
    };
    
    cout << "     1   2   3    4   5   6    7   8   9" << endl;
    cout << "    --- --- ---  --- --- ---  --- --- ---" << endl;
    cout << "1  | " << displayChar(s[0][0]) << " | " << displayChar(s[0][1]) << " | " << displayChar(s[0][2]) << " || " << displayChar(s[0][3]) << " | " << displayChar(s[0][4]) << " | " << displayChar(s[0][5]) << " || " << displayChar(s[0][6]) << " | " << displayChar(s[0][7]) << " | " << displayChar(s[0][8]) << " |" << endl;
    cout << "   |---|---|---||---|---|---||---|---|---|" << endl;
    cout << "2  | " << displayChar(s[1][0]) << " | " << displayChar(s[1][1]) << " | " << displayChar(s[1][2]) << " || " << displayChar(s[1][3]) << " | " << displayChar(s[1][4]) << " | " << displayChar(s[1][5]) << " || " << displayChar(s[1][6]) << " | " << displayChar(s[1][7]) << " | " << displayChar(s[1][8]) << " |" << endl;
    cout << "   |---|---|---||---|---|---||---|---|---|" << endl;
    cout << "3  | " << displayChar(s[2][0]) << " | " << displayChar(s[2][1]) << " | " << displayChar(s[2][2]) << " || " << displayChar(s[2][3]) << " | " << displayChar(s[2][4]) << " | " << displayChar(s[2][5]) << " || " << displayChar(s[2][6]) << " | " << displayChar(s[2][7]) << " | " << displayChar(s[2][8]) << " |" << endl;
    cout << "   |===|===|===  ===|===|===  ===|===|===|" << endl;
    cout << "4  | " << displayChar(s[3][0]) << " | " << displayChar(s[3][1]) << " | " << displayChar(s[3][2]) << " || " << displayChar(s[3][3]) << " | " << displayChar(s[3][4]) << " | " << displayChar(s[3][5]) << " || " << displayChar(s[3][6]) << " | " << displayChar(s[3][7]) << " | " << displayChar(s[3][8]) << " |" << endl;
    cout << "   |---|---|---||---|---|---||---|---|---|" << endl;
    cout << "5  | " << displayChar(s[4][0]) << " | " << displayChar(s[4][1]) << " | " << displayChar(s[4][2]) << " || " << displayChar(s[4][3]) << " | " << displayChar(s[4][4]) << " | " << displayChar(s[4][5]) << " || " << displayChar(s[4][6]) << " | " << displayChar(s[4][7]) << " | " << displayChar(s[4][8]) << " |" << endl;
    cout << "   |---|---|---||---|---|---||---|---|---|" << endl;
    cout << "6  | " << displayChar(s[5][0]) << " | " << displayChar(s[5][1]) << " | " << displayChar(s[5][2]) << " || " << displayChar(s[5][3]) << " | " << displayChar(s[5][4]) << " | " << displayChar(s[5][5]) << " || " << displayChar(s[5][6]) << " | " << displayChar(s[5][7]) << " | " << displayChar(s[5][8]) << " |" << endl;
    cout << "   |===|===|===  ===|===|===  ===|===|===|" << endl;
    cout << "7  | " << displayChar(s[6][0]) << " | " << displayChar(s[6][1]) << " | " << displayChar(s[6][2]) << " || " << displayChar(s[6][3]) << " | " << displayChar(s[6][4]) << " | " << displayChar(s[6][5]) << " || " << displayChar(s[6][6]) << " | " << displayChar(s[6][7]) << " | " << displayChar(s[6][8]) << " |" << endl;
    cout << "   |---|---|---||---|---|---||---|---|---|" << endl;
    cout << "8  | " << displayChar(s[7][0]) << " | " << displayChar(s[7][1]) << " | " << displayChar(s[7][2]) << " || " << displayChar(s[7][3]) << " | " << displayChar(s[7][4]) << " | " << displayChar(s[7][5]) << " || " << displayChar(s[7][6]) << " | " << displayChar(s[7][7]) << " | " << displayChar(s[7][8]) << " |" << endl;
    cout << "   |---|---|---||---|---|---||---|---|---|" << endl;
    cout << "9  | " << displayChar(s[8][0]) << " | " << displayChar(s[8][1]) << " | " << displayChar(s[8][2]) << " || " << displayChar(s[8][3]) << " | " << displayChar(s[8][4]) << " | " << displayChar(s[8][5]) << " || " << displayChar(s[8][6]) << " | " << displayChar(s[8][7]) << " | " << displayChar(s[8][8]) << " |" << endl;
    cout << "   |---|---|---||---|---|---||---|---|---|" << endl;
}

void SudokuUI::playInteractive()
{
    // Validate custom sudoku if user inputs custom sudoku
    if (!sudoku.solve())
    {
        const auto& sol = sudoku.getSolution();
        if (sol.size() == 1 && sol[0].size() == 1 && sol[0][0] == 1)
        {
            cout << "more than 1 solution,No unique solution exists for the given Sudoku puzzle." << endl;
            return;
        }
        else if (sol.size() == 1 && sol[0].size() == 1 && sol[0][0] == 0)
        {
            cout << "zero solutions,No solution exists for given sudoku puzzle." << endl;
            return;
        }
        else
        {
            // validation error
            cout << "Invalid Sudoku: Board contains duplicates in a row, column, or box." << endl;
            return;
        }
    }
    
    while (!sudoku.isSolved(const_cast<vector<vector<int>>&>(sudoku.getBoard())))
    {
        printBoard(sudoku.getBoard());
        cout << "enter row number column number and value to fill" << endl;
        cout << "type 0 0 0 to exit" << endl;
        cout << "type -1 -1 -1 to get hint" << endl;
        int row, col, value;
        cin >> row >> col >> value;

        if (row == 0 && col == 0 && value == 0)
        {
            cout << "Exiting...\n";
            return;
        }
        else if (row == -1 && col == -1 && value == -1)
        {
            string hint_reason;
            if (sudoku.getHint(hint_reason))
            {
                cout << hint_reason << endl;
            }
            else
            {
                cout << "this sudoku is not solvable by hidden single, obvious single, obvious pair and pointing pair." << endl;   
            }
        }
        else if (row <= 0 || row > 9 || col <= 0 || col > 9 || value <= 0 || value > 9)
        {
            cout << "Invalid input. Please enter valid row, column and value.\n";
        }
        else if (sudoku.getBoard()[row - 1][col - 1] != 0)
        {
            cout << "Cell already filled. Please choose an empty cell.\n";
        }
        else if (sudoku.getSolution()[row - 1][col - 1] != value)
        {
            cout << "Invalid move. Please try again.\n";
            continue;
        }
        else 
        {
            sudoku.setBoardCell(row - 1, col - 1, value);
        }
    }
    printBoard(sudoku.getBoard());
    cout << "Congratulations! You have solved the Sudoku puzzle.\n";
}

void SudokuUI::getSolutionOfCustomSudoku()
{
    if (sudoku.solve())
    {
        cout << "solution:" << endl;
        printBoard(sudoku.getSolution());
    }
    else
    {
        const auto& sol = sudoku.getSolution();
        if (sol.size() == 1 && sol[0].size() == 1 && sol[0][0] == 1)
        {
            cout << "No unique solution exists for the given Sudoku puzzle.\n";
        }
        else if (sol.size() == 1 && sol[0].size() == 1 && sol[0][0] == 0)
        {
            cout << "No solution exists for the given Sudoku puzzle.\n";
        }
        else
        {
            cout << "Invalid Sudoku: Board contains duplicates in a row, column, or box.\n";
        }
    }
}

void SudokuUI::inputCustomSudoku()
{
    sudoku.clearSolution();
    cout << "Enter the Sudoku puzzle Row wise without any blank space(0 for empty cells):\n";
    string s;
    for (int i = 0; i < 9; i++)
    {
        cin >> s;
        if (s.length() != 9) 
        { 
            cout << "Please enter exactly 9 digits." << endl; 
            i--; 
            continue; 
        }
        for (int j = 0; j < 9; j++)
        {
            if (s[j] == '0')
            {
                sudoku.setBoardCell(i, j, 0);
            }
            else if (s[j] >= '1' && s[j] <= '9')
            {
                sudoku.setBoardCell(i, j, s[j] - '0');
            }
            else
            {
                cout << "Please enter digits 0-9 only.\n";
                i--;
                break;
            }
        }
    }
}