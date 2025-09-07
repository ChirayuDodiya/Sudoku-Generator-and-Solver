#include "Sudoku.h"

using namespace std;
using namespace std::chrono;

//constructor
Sudoku::Sudoku()
    {
        board.resize(9, vector<char>(9, ' '));
        solution.resize(9, vector<char>(9, ' '));
        possibility.assign(9,vector<set<char>>(9));
    }
//calls generateSudokuWithNClues function
void Sudoku::generateRandomSudoku(int n)
    {   reseedRandom();
        if(n==1) generateSudokuWithNClues(47+dist(rng));
        else if(n==2) generateSudokuWithNClues(40+dist(rng));
        else generateSudokuWithNClues(33+dist(rng));
    }
//takes input of sudoku from user
void Sudoku::inputCustomSudoku()
    {
        cout<<"Enter the Sudoku puzzle Row wise without any blank space(0 for empty cells):\n";
        string s;
        for(int i=0;i<9;i++)
        {
            cin>>s;
            if(s.length()!=9) 
            { 
                cout<<"Please enter exactly 9 digits."<<endl; 
                i--; 
                continue; 
            }
            for(int j=0;j<9;j++)
            {
                if(s[j] == '0')
                {
                    board[i][j] = ' ';
                }
                else if(s[j] >= '1' && s[j] <= '9')
                {
                    board[i][j] = s[j];
                }
                else
                {
                    cout<<"Please enter digits 0-9 only.\n";
                    i--;
                    break;
                }
            }
        }
        return;
    }
//main function for user interface
void Sudoku::interactiveSolve()
    {
        solution=findSolution(board);
        initializepossiblity();
        if(solution.size()==1 && solution[0].size()==1 && solution[0][0] == '1')
        {
            cout<<"No unique solution exists for the given Sudoku puzzle.\n";
            return;
        }
        if(solution.empty())
        {
            cout<<"No solution exists for the given Sudoku puzzle.\n";
            return;
        }
        while(!isSolved(board))
        {
            printBoard(board);
            cout<<"enter row number column number and value to fill"<<endl;
            cout<<"type 0 0 0 to exit"<<endl;
            cout<<"type -1 -1 -1 to get hint"<<endl;
            int row, col, value;
            cin>>row>>col>>value;
            char ch = value + '0';

            if(row == 0 && col == 0 && value == 0)
            {
                cout<<"Exiting...\n";
                return;
            }
            
            else if(row == -1 && col == -1 && value == -1)
            {
                if(applyPointingPair(board,1))
                {

                }
                else if(applyObviousPair(board,1))
                {

                }
                else if(applyObviousSingle(board,1))
                {

                }
                else if(applyHiddenSingle(board,1))
                {

                }
                else
                {
                    cout<<"this sudoku is not solvable by hidden single or obvious single"<<endl;   
                }
                
            }

            else if(row <= 0 || row > 9 || col <= 0 || col > 9 || value <= 0 || value > 9)
            {
                cout<<"Invalid input. Please enter valid row, column and value.\n";
            }

            else if(board[row-1][col-1]!= ' ')
            {
                cout<<"Cell already filled. Please choose an empty cell.\n";
            }
            
            
            else if(solution[row-1][col-1] != ch)
            {
                cout<<"Invalid move. Please try again.\n";
                continue;
            }

            else 
            {
                board[row-1][col-1] = ch;
            }
        }
        printBoard(board);
        cout<<"Congratulations! You have solved the Sudoku puzzle.\n";
    }
//finds solution,and if no unique solution is there or multiple solutions are there,it gives warning message   
void Sudoku::getsolution()
    {
        //validate sudoku
        solution=findSolution(board);
        if(solution.size()==1 && solution[0].size()==1 && solution[0][0] == '1')
        {
            cout<<"No unique solution exists for the given Sudoku puzzle.\n";
            return;
        }
        if(solution.empty())
        {
            cout<<"No solution exists for the given Sudoku puzzle.\n";
            return;
        }
        cout<<"solution:"<<endl;
        printBoard(solution);
        return;
    }
//to get more random values
void Sudoku::reseedRandom() 
    {
        rng.seed(steady_clock::now().time_since_epoch().count());
    }
//checks if it is valid to put num at board[row][col]
bool Sudoku::isValidMove(vector<vector<char>> &board, int row, int col, char num)
    {
        for(int j = 0; j < 9; j++)
        {
            if(board[row][j] == num)
            {
                return false;
            }
        }
        for(int i = 0; i < 9; i++)
        {
            if(board[i][col] == num)
            {
                return false;
            }
        }
        
        int startRow = (row / 3) * 3;
        int startCol = (col / 3) * 3;
        for(int i = startRow; i < startRow + 3; i++)
        {
            for(int j = startCol; j < startCol + 3; j++)
            {
                if(board[i][j] == num)
                {
                    return false;
                }
            }
        }
        return true;
    }
//checks if generated sudoku is solvable by human(checks basic methods,advance methods will be added)
bool Sudoku::isHumansolvable(vector<vector<char>>&board)
    {       
        vector<vector<char>>tempboard=board;
        initializepossiblity();
        
        bool progress = true;
        int iterations = 0;
        const int MAX_ITERATIONS = 100;
        
        while (progress && !isSolved(tempboard) && iterations < MAX_ITERATIONS) {
            progress = false;
            iterations++;
            
            if (applyObviousSingle(tempboard,0)) 
            {
                progress = true;
                continue;
            }
            
            if (applyHiddenSingle(tempboard,0)) 
            {
                progress = true;
                continue;
            }
            
            if (applyObviousPair(tempboard,0)) 
            {
                progress = true;
                continue;
            }
            
            if (applyPointingPair(tempboard,0)) 
            {
                progress = true;
                continue;
            }
        }
        return isSolved(tempboard);
    }
//helps to fill values that are directly solvable ,so that it reduces recursion
void Sudoku::solverhelper(vector<vector<char>>&board)
    {       
        initializepossiblity();
        
        bool progress = true;
        int iterations = 0;
        const int MAX_ITERATIONS = 100;
        
        while (progress && !isSolved(board) && iterations < MAX_ITERATIONS) {
            progress = false;
            iterations++;
            
            if (applyObviousSingle(board,0)) 
            {
                progress = true;
                continue;
            }
            
            if (applyHiddenSingle(board,0)) 
            {
                progress = true;
                continue;
            }
            
            if (applyObviousPair(board,0)) 
            {
                progress = true;
                continue;
            }
            
            if (applyPointingPair(board,0)) 
            {
                progress = true;
                continue;
            }
        }
        return;
    }
// Add all possible possibility for empty cells in board
void Sudoku::initializepossiblity() 
    {
        possibility.assign(9,vector<set<char>>(9));
        for (int row = 0; row < 9; row++) 
        {
            for (int col = 0; col < 9; col++) 
            {
                if (board[row][col] == ' ') 
                {
                    for (char num = '1'; num <= '9'; num++) 
                    {
                        if (isValidMove(board,row, col,num)) 
                        {
                            possibility[row][col].insert(num);
                        }
                    }
                }
            }
        }
    }
//update possiblity after correct move
void Sudoku::updatepossiblityAfterMove(int row, int col, char num) 
    {
        possibility[row][col].clear();
        
        // Remove this number from possibility in the same row
        for (int c = 0; c < 9; c++) 
        {
            possibility[row][c].erase(num);
        }
        
        // Remove this number from possibility in the same column
        for (int r = 0; r < 9; r++) 
        {
            possibility[r][col].erase(num);
        }
        
        // Remove this number from possibility in the same box
        int box_row = (row / 3) * 3;
        int box_col = (col / 3) * 3;
        for (int r = box_row; r < box_row + 3; r++) 
        {
            for (int c = box_col; c < box_col + 3; c++) 
            {
                possibility[r][c].erase(num);
            }
        }
    }
// If cell is empty and has only one candidate
bool Sudoku::applyObviousSingle(vector<vector<char>>&tempboard,bool printReason) 
    {
        bool found = false;
        
        for (int row = 0; row < 9; row++) 
        {
            for (int col = 0; col < 9; col++) 
            {
                if (tempboard[row][col] == ' ' && possibility[row][col].size() == 1) 
                {
                    char num = *possibility[row][col].begin();
                    tempboard[row][col] = num;
                    updatepossiblityAfterMove(row, col, num);
                    found = true;
                    if(printReason)
                    {
                        cout<<"Hint:Obvious Single"<<endl;
                        cout<<"at index "<<row+1<<" "<<col+1<<" there is only one possible num "<<num<<"."<<endl;
                        return found;
                    }
                }
            }
        }
        return found;
    }
// If only one position possible, it's a hidden single
bool Sudoku::applyHiddenSingle(vector<vector<char>>&tempboard,bool printReason) 
    {
        bool found = false;
        
        // Check rows for hidden singles
        for (int row = 0; row < 9; row++) 
        {
            for (char num = '1'; num <= '9'; num++) 
            {
                vector<int> possible_cols;
                
                // Find all columns in this row where 'num' can go
                for (int col = 0; col < 9; col++) 
                {
                    if (tempboard[row][col] == ' ' && possibility[row][col].count(num)) 
                    {
                        possible_cols.push_back(col);
                    }
                }
                
                // If only one position possible, it's a hidden single
                if (possible_cols.size() == 1) 
                {
                    int col = possible_cols[0];
                    tempboard[row][col] = num;
                    updatepossiblityAfterMove(row, col, num);
                    found = true;
                    if(printReason)
                    {
                        cout<<"Hint:Hidden Single"<<endl;
                        cout<<"for row "<<row+1<<" there is only one possible column "<<possible_cols[0]+1<<" where you can insert "<<num<<endl;
                        return found;
                    }
                }
            }
        }
        
        // Check columns for hidden singles
        for (int col = 0; col < 9; col++) 
        {
            for (char num = '1'; num <= '9'; num++) 
            {
                vector<int> possible_rows;
                
                for (int row = 0; row < 9; row++) 
                {
                    if (tempboard[row][col] == ' ' && possibility[row][col].count(num)) 
                    {
                        possible_rows.push_back(row);
                    }
                }
                
                if (possible_rows.size() == 1) 
                {
                    int row = possible_rows[0];
                    tempboard[row][col] = num;
                    updatepossiblityAfterMove(row, col, num);
                    found = true;
                    if(printReason)
                    {
                        cout<<"Hint:Hidden Single"<<endl;
                        cout<<"for column "<<col+1<<" there is only one possible row "<<possible_rows[0]+1<<" where you can insert "<<num<<endl;
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
            
            for (char num = '1'; num <= '9'; num++) 
            {
                vector<pair<int, int>> possible_cells;
                
                for (int r = box_row; r < box_row + 3; r++) 
                {
                    for (int c = box_col; c < box_col + 3; c++) 
                    {
                        if (tempboard[r][c] == ' ' && possibility[r][c].count(num)) 
                        {
                            possible_cells.push_back({r, c});
                        }
                    }
                }
                
                if (possible_cells.size() == 1) 
                {
                    int row = possible_cells[0].first;
                    int col = possible_cells[0].second;
                    tempboard[row][col] = num;
                    updatepossiblityAfterMove(row, col, num);
                    found = true;
                    if(printReason)
                    {
                        cout<<"Hint:Hidden Single"<<endl;
                        cout<<"for box "<<box+1<<" there is only one possible row,column "<<row+1<<" "<<col+1<<" where you can insert "<<num<<endl;
                        return found;
                    }
                }
            }
        }
        return found;
    }
// if 2 cells are empty, have exactly 2 possibility, and are identical,so remove that from all other respective
bool Sudoku::applyObviousPair(vector<vector<char>>&tempboard,bool printReason)
    {
        bool found = false;
        
        // Check rows for obvious pairs
        for (int row = 0; row < 9; row++) 
        {
            for (int col1 = 0; col1 < 8; col1++) 
            {
                for (int col2 = col1 + 1; col2 < 9; col2++) 
                {
                    // Check if both cells are empty, have exactly 2 possibility, and are identical
                    if (tempboard[row][col1] == ' ' && tempboard[row][col2] == ' ' &&
                        possibility[row][col1].size() == 2 &&
                        possibility[row][col1] == possibility[row][col2]) 
                        {
                            // Remove these possibility from other cells in the row
                            for (int c = 0; c < 9; c++) 
                            {
                                if (c != col1 && c != col2 && tempboard[row][c] == ' ')
                                {
                                    for (char num : possibility[row][col1])
                                    {
                                        if (possibility[row][c].erase(num))
                                        {
                                            found = true;
                                        }
                                    }
                                    if(found && printReason)
                                    {   
                                        auto it=possibility[row][col1].begin();
                                        cout<<"Hint:Obvious Pair"<<endl;
                                        cout<<"for row "<<row+1<<" there is only two columns "<<col1+1<<" "<<col2+1<<" where you can insert "<<*it<<" ";
                                        it++;
                                        cout<<"and "<<*it<<endl;
                                        return found;
                                    }
                                }
                            }
                        }
                }
            }
        }
        
        // Check columns for obvious pairs
        for (int col = 0; col < 9; col++) 
        {
            for (int row1 = 0; row1 < 8; row1++) 
            {
                for (int row2 = row1 + 1; row2 < 9; row2++) 
                {
                    if (tempboard[row1][col] == ' ' && tempboard[row2][col] == ' ' &&
                        possibility[row1][col].size() == 2 &&
                        possibility[row1][col] == possibility[row2][col]) 
                        {
                            for (int r = 0; r < 9; r++) 
                            {
                                if (r != row1 && r != row2 && tempboard[r][col] == ' ') 
                                {
                                    for (char num : possibility[row1][col]) 
                                    {
                                        if (possibility[r][col].erase(num)) 
                                        {
                                            found = true;
                                            if(found && printReason)
                                            {   
                                                auto it=possibility[row1][col].begin();
                                                cout<<"Hint:Obvious Pair"<<endl;
                                                cout<<"for column "<<col+1<<" there is only two rows "<<row1+1<<" "<<row2+1<<" where you can insert "<<*it<<" ";
                                                it++;
                                                cout<<"and "<<*it<<endl;
                                                return found;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                }
            }
        }
        
        // Check boxes for obvious pairs
        for (int box = 0; box < 9; box++) 
        {
            int box_row = (box / 3) * 3;
            int box_col = (box % 3) * 3;
            
            vector<pair<int, int>> empty_cells;
            for (int r = box_row; r < box_row + 3; r++) 
            {
                for (int c = box_col; c < box_col + 3; c++) 
                {
                    if (tempboard[r][c] == ' ') 
                    {
                        empty_cells.push_back({r, c});
                    }
                }
            }
            
            for (size_t i = 0; i < empty_cells.size() - 1; i++) 
            {
                for (size_t j = i + 1; j < empty_cells.size(); j++) 
                {
                    int r1 = empty_cells[i].first, c1 = empty_cells[i].second;
                    int r2 = empty_cells[j].first, c2 = empty_cells[j].second;
                    
                    if (possibility[r1][c1].size() == 2 &&
                        possibility[r1][c1] == possibility[r2][c2]) 
                        {
                        
                            for (auto& cell : empty_cells) 
                            {
                                int r = cell.first, c = cell.second;
                                if ((r != r1 || c != c1) && (r != r2 || c != c2)) 
                                {
                                    for (char num : possibility[r1][c1]) 
                                    {
                                        if (possibility[r][c].erase(num)) 
                                        {
                                            found = true;
                                            if(printReason)
                                            {   
                                                auto it=possibility[r1][c1].begin();
                                                cout<<"Hint:Obvious Pair"<<endl;
                                                cout<<"for box "<<box+1<<" there is only two sub-box "<<r1+1<<","<<c1+1<<" "<<r2+1<<","<<c2+1<<" where you can insert "<<*it<<" ";
                                                it++;
                                                cout<<"and "<<*it<<endl;
                                                return found;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                }
            }
        }
        return found;
    }
//if number appears only in one row/one column of the box,then remove if from that row/column
bool Sudoku::applyPointingPair(vector<vector<char>>&tempboard,bool printReason)
    {
        bool found = false;
        
        // For each box, check if a number appears only in one row or column
        for (int box = 0; box < 9; box++) 
        {
            int box_row = (box / 3) * 3;
            int box_col = (box % 3) * 3;
            
            for (char num = '1'; num <= '9'; num++) 
            {
                // Check if number appears only in one row of the box
                set<int> rows_with_num;
                for (int r = box_row; r < box_row + 3; r++) 
                {
                    for (int c = box_col; c < box_col + 3; c++) 
                    {
                        if (tempboard[r][c] == ' ' && possibility[r][c].count(num)) 
                        {
                            rows_with_num.insert(r);
                        }
                    }
                }
                
                // If number only appears in one row of the box
                if (rows_with_num.size() == 1) 
                {
                    int row = *rows_with_num.begin();
                    // Remove this number from other cells in the same row outside the box
                    for (int c = 0; c < 9; c++) 
                    {
                        if ((c < box_col || c >= box_col + 3) && 
                            (tempboard[row][c] == ' ') && 
                            (possibility[row][c].erase(num))) 
                            {
                                found = true;
                                if(printReason)
                                {   
                                    cout<<"Hint:Pointing Pair"<<endl;
                                    cout<<"for box "<<box+1<<" there is only one row "<<row+1<<" where you can insert "<<num<<endl;
                                }
                                return found;
                            }
                    }
                }
                
                // Check if number appears only in one column of the box
                set<int> cols_with_num;
                for (int c = box_col; c < box_col + 3; c++) 
                {
                    for (int r = box_row; r < box_row + 3; r++) 
                    {
                        if (tempboard[r][c] == ' ' && possibility[r][c].count(num)) 
                        {
                            cols_with_num.insert(c);
                        }
                    }
                }
                
                // If number only appears in one column of the box
                if (cols_with_num.size() == 1) 
                {
                    int col = *cols_with_num.begin();
                    // Remove this number from other cells in the same column outside the box
                    for (int r = 0; r < 9; r++) 
                    {
                        if ((r < box_row || r >= box_row + 3) && 
                            (tempboard[r][col] == ' ') && 
                            (possibility[r][col].erase(num))) 
                            {
                                found = true;
                                if(printReason)
                                {   
                                    cout<<"Hint:Pointing Pair"<<endl;
                                    cout<<"for box "<<box+1<<" there is only one column "<<col+1<<" where you can insert "<<num<<endl;
                                }
                                return found;
                            }
                    }
                }
            }
        }
        return found;
    }
//count num of solution,if solution>2 then returns 2
void Sudoku::countSolutions(vector<vector<char>> &board, int& count)
    {
        if(count > 1) return;
        
        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                if(board[i][j] == ' ')
                {
                    for(char num = '1'; num <= '9'; num++)
                    {
                        if(isValidMove(board, i, j, num))
                        {
                            board[i][j] = num;
                            countSolutions(board, count);
                            board[i][j] = ' ';
                            if(count > 1) return;
                        }
                    }
                    return;
                }
            }
        }
        count++;
        return;
    }
//fills grid which could be a possible sudoku answer
bool Sudoku::fillGrid(int row, int col) 
    {
        if (row == 9) return true;
        if (col == 9) return fillGrid(row + 1, 0);
        vector<char> nums = {'1','2','3','4','5','6','7','8','9'};
        shuffle(nums.begin(), nums.end(),rng);
        for (char num : nums) 
        {
            if (isValidMove(solution,row, col, num)) 
            {
                solution[row][col] = num;
                if (fillGrid(row, col + 1)) return true;
                solution[row][col] = ' ';
            }
        }
        return false;
    }
//generates sudoku question
void Sudoku::generateSudokuWithNClues(int n) 
    {
        solution.assign(9, vector<char>(9, ' '));
        fillGrid(0, 0);
        board=solution;
        vector<pair<int, int>> cells;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                cells.emplace_back(i, j);
            }
        }
        
        shuffle(cells.begin(), cells.end(),rng);

        int totalCells = 81;
        for (size_t i = 0; i < cells.size(); ++i) 
        {
            int row = cells[i].first;
            int col = cells[i].second;
            if (totalCells <= n) break;
            char backup = board[row][col];
            board[row][col] = ' ';
            int count = 0;
            countSolutions(board,count);
            if((count != 1)||(!isHumansolvable(board)))
            {
                board[row][col] = backup;
            }
            else
            {
                totalCells--;
            }
        }
    }
//finds solution of sudoku
vector<vector<char>> Sudoku::findSolution(vector<vector<char>>board)
    {
        int solutionCount = 0;
        countSolutions(board, solutionCount);
        
        if(solutionCount > 1)
        {
            // Multiple solutions exist, return {{1}}
            return {{'1'}};
        }
        solverhelper(board);
        //get box with min possiblity
        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                if(board[i][j] == ' ')
                {
                    for(char num = '1'; num <= '9'; num++)
                    {
                        if(isValidMove(board, i, j, num))
                        {
                            board[i][j] = num;
                            vector<vector<char>> result = findSolution(board);
                            if(!result.empty() && result.size() == 9)
                                return result;
                            board[i][j] = ' ';
                        }
                    }
                    return {};
                }
            }
        }
        return board;
    }
//checks if sudoku is solved or not
bool Sudoku::isSolved(vector<vector<char>>&board)
    {
        for(int i=0;i<9;i++)
        {
            for(int j=0;j<9;j++)
            {
                if(board[i][j]==' ')return false;
            }
        }
        return true;
    }
//prints sudoku with proper formate
void Sudoku::printBoard(vector<vector<char>>&s)
    {
        cout<<"     1   2   3    4   5   6    7   8   9"<<endl;
        cout<<"    --- --- ---  --- --- ---  --- --- ---"<<endl;
        cout<<"1  | "<<s[0][0]<<" | "<<s[0][1]<<" | "<<s[0][2]<<" || "<<s[0][3]<<" | "<<s[0][4]<<" | "<<s[0][5]<<" || "<<s[0][6]<<" | "<<s[0][7]<<" | "<<s[0][8]<<" |"<<endl;
        cout<<"   |---|---|---||---|---|---||---|---|---|"<<endl;
        cout<<"2  | "<<s[1][0]<<" | "<<s[1][1]<<" | "<<s[1][2]<<" || "<<s[1][3]<<" | "<<s[1][4]<<" | "<<s[1][5]<<" || "<<s[1][6]<<" | "<<s[1][7]<<" | "<<s[1][8]<<" |"<<endl;
        cout<<"   |---|---|---||---|---|---||---|---|---|"<<endl;
        cout<<"3  | "<<s[2][0]<<" | "<<s[2][1]<<" | "<<s[2][2]<<" || "<<s[2][3]<<" | "<<s[2][4]<<" | "<<s[2][5]<<" || "<<s[2][6]<<" | "<<s[2][7]<<" | "<<s[2][8]<<" |"<<endl;
        cout<<"   |===|===|===  ===|===|===  ===|===|===|"<<endl;
        cout<<"4  | "<<s[3][0]<<" | "<<s[3][1]<<" | "<<s[3][2]<<" || "<<s[3][3]<<" | "<<s[3][4]<<" | "<<s[3][5]<<" || "<<s[3][6]<<" | "<<s[3][7]<<" | "<<s[3][8]<<" |"<<endl;
        cout<<"   |---|---|---||---|---|---||---|---|---|"<<endl;
        cout<<"5  | "<<s[4][0]<<" | "<<s[4][1]<<" | "<<s[4][2]<<" || "<<s[4][3]<<" | "<<s[4][4]<<" | "<<s[4][5]<<" || "<<s[4][6]<<" | "<<s[4][7]<<" | "<<s[4][8]<<" |"<<endl;
        cout<<"   |---|---|---||---|---|---||---|---|---|"<<endl;
        cout<<"6  | "<<s[5][0]<<" | "<<s[5][1]<<" | "<<s[5][2]<<" || "<<s[5][3]<<" | "<<s[5][4]<<" | "<<s[5][5]<<" || "<<s[5][6]<<" | "<<s[5][7]<<" | "<<s[5][8]<<" |"<<endl;
        cout<<"   |===|===|===  ===|===|===  ===|===|===|"<<endl;
        cout<<"7  | "<<s[6][0]<<" | "<<s[6][1]<<" | "<<s[6][2]<<" || "<<s[6][3]<<" | "<<s[6][4]<<" | "<<s[6][5]<<" || "<<s[6][6]<<" | "<<s[6][7]<<" | "<<s[6][8]<<" |"<<endl;
        cout<<"   |---|---|---||---|---|---||---|---|---|"<<endl;
        cout<<"8  | "<<s[7][0]<<" | "<<s[7][1]<<" | "<<s[7][2]<<" || "<<s[7][3]<<" | "<<s[7][4]<<" | "<<s[7][5]<<" || "<<s[7][6]<<" | "<<s[7][7]<<" | "<<s[7][8]<<" |"<<endl;
        cout<<"   |---|---|---||---|---|---||---|---|---|"<<endl;
        cout<<"9  | "<<s[8][0]<<" | "<<s[8][1]<<" | "<<s[8][2]<<" || "<<s[8][3]<<" | "<<s[8][4]<<" | "<<s[8][5]<<" || "<<s[8][6]<<" | "<<s[8][7]<<" | "<<s[8][8]<<" |"<<endl;
        cout<<"   |---|---|---||---|---|---||---|---|---|"<<endl;
    }