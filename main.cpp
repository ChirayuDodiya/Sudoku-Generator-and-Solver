#include "Sudoku.h"
using namespace std;
int main()
{
    Sudoku sudoku;
    int choice;
    
    do{
        cout<<"welcome to sudoku solver"<<endl;
        cout<<"type 1 to solve random sudoku"<<endl;
        cout<<"type 2 to solve custom sudoku"<<endl;
        cout<<"type 3 to get solution of custom sudoku"<<endl;
        cout<<"type 4 to exit"<<endl;
        cin>>choice;
        switch (choice)
        {
        case 1:
            {
                int difficulty;
                cout<<"Select difficulty:"<<endl;
                cout<<"1. Easy (47-53 clues)"<<endl;
                cout<<"2. Medium (40-46 clues)"<<endl;
                cout<<"3. Hard (33-39 clues)"<<endl;
                cout<<"Enter choice: ";
                cin >> difficulty;
                while(difficulty<1||difficulty>3)
                {
                    cout<<"Invalid choice. Please enter 1, 2, or 3: ";
                    cin >> difficulty;
                }
                sudoku.generateRandomSudoku(difficulty);
                sudoku.interactiveSolve();
                break;
            }
            
        case 2:
            sudoku.inputCustomSudoku();
            sudoku.interactiveSolve();
            break;
            
        case 3:
            sudoku.inputCustomSudoku();
            sudoku.getsolution();
            break;

        case 4:
            cout<<"thanks for using Sudoku Generator and Solver"<<endl;
            return 0;

        default:
            cout<<"invalid choice,please try again"<<endl;
            break;
        }
    }
    while(choice!=4);
    return 0;
}
