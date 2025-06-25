# Sudoku Generator and Solver (C++17)

A feature-rich terminal-based Sudoku game written in modern C++17. This project provides a complete Sudoku experience with puzzle generation, solving capabilities, and interactive gameplay.

## Features

- **Random Puzzle Generation**: Create unique Sudoku puzzles with guaranteed single solutions
- **Multiple Difficulty Levels**: Choose from Easy, Medium, or Hard difficulty settings
- **Custom Puzzle Input**: Import and solve your own Sudoku puzzles
- **Interactive Solver**: Play with real-time validation and helpful feedback
- **Solution Display**: View the complete solution when needed
- **Modern C++17**: Built using contemporary C++ standards and best practices

## Project Structure

```
sudoku_project/
├── main.cpp      # Contains main() function and user interface
├── Sudoku.h      # Sudoku class declaration and header definitions
├── Sudoku.cpp    # Sudoku class implementation and core logic
└── README.md     # This file
```

## Build Instructions

### Ubuntu/Linux
```bash
g++ -std=c++17 main.cpp Sudoku.cpp -o sudoku
./sudoku
```

### Windows
```cmd
g++ -std=c++17 main.cpp Sudoku.cpp -o sudoku.exe
sudoku.exe
```

### Requirements
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Standard C++ library

## How to Play

1. **Launch the game**: Run the compiled executable
2. **Choose an option**:
   - Generate a new random puzzle (select difficulty)
   - Input your own custom puzzle to solve
   - Input your own custom puzzle to get solution
3. **Solve interactively**: Enter numbers in the format `row col value`
4. **Win**: Complete the puzzle with all valid numbers!

## Contributing

Feel free to fork this project and submit pull requests for improvements such as:
- Additional difficulty levels
- Save/load functionality
- GUI interface
- Performance optimizations
- Additional puzzle variants

## License

This project is open source. Feel free to use, modify, and distribute as needed.

---

*Enjoy solving Sudoku puzzles!* 🧩
