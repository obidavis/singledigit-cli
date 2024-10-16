## singledigit

A CLI for solving and generating sudoku puzzles. For a web interface to this tool (and more) check out [singledigit.org](https://singledigit.org).

### Usage
```
singledigit [OPTIONS] SUBCOMMAND

Options:
  -h,--help                   

Subcommands:
generate
  Generate sudoku puzzles
  Options:
    -c,--count                  Number of puzzles to generate
    -d,--difficulty [min, max (optional)]
                                Difficulty range
    -s,--seed                   Random seed
    -t,--threads                Number of threads
    -f,--format [plain, json]   Output format
    -i,--indent                 Indentation level for JSON output (-1 for unformatted)
    -a,--allowed-strategies [basic, naked_singles, hidden_singles, naked_pairs, naked_triples, hidden_pairs, hidden_triples, naked_quads, hidden_quads, pointing_pairs_triples, box_line_reduction, x_wing, swordfish]
                                Allowed strategies
    -r,--required-strategies [Same as allowed strategies]
                                Required strategies

solve
  Calculate a detailed solution path for a given board
  Positionals:
    board                       The board to solve
  Options:
    -S,--single-step            Solve one step at a time
    -f,--format [plain, json]   Output format
    -i,--indent                 Indentation level for JSON output (-1 for unformatted)
```

### Generation
The generation algorithm begins by creating a solved board, and then iteratively removes clues first symmetrically, then individually. The presence of a unique solution is checked at each step. This process continues until no further clues can be removed, resulting in a minimal puzzle. 

If the generation is constrained, either by difficulty or by a set of allowed/required strategies, the solver and grader will be used to ensure the puzzle meets the constraints.

### Board format
Simple boards (no candidates/pencil marks) are represented as a string of 81 characters, where each character is a digit from 1 to 9, or a zero (0) to represent an empty cell. 
 
Board with pencil marks are represented as a string of 162 characters, with each pair of characters being a base32 string representation of the cell's state. In this 10bit number, the least significant bit represents whether a cell is a clue or not, and the other 9 bits represent the presence of candidates 1-9. This has the consequence of unsolved Naked Singles being unrepresentable in this format. 

This format comes from [sudokuwiki.org](https://www.sudokuwiki.org/Sudoku_String_Definitions)


### Difficulty

The difficulty of a puzzle is calculated from the strategies required to solve the puzzle, and the frequency of 'opportunities' to eliminate candidates at any step. 

This approach is inspired again by Andrew Stuart of sudokuwiki.org and his paper [here](https://www.sudokuwiki.org/Sudoku_Creation_and_Grading.pdf).



### Strategies
The current supported strategies are:
- Naked Singles/Doubles/Triples/Quads
- Hidden Singles/Doubles/Triples/Quads
- Intersection Removal (Pointing Pairs/Triples and Box-Line Reduction)
- X-Wing, Swordfish, Jellyfish

This list is clearly non-exhaustive, but appropriate for making puzzles up to newspaper 'hard' difficulty at least! More strategies will be added in the future.

### TODO
- More strategies
- More difficulty metrics (SudokuExplainer, sudoku.coach etc.)
- Support for more board formats
- Quality heuristics for puzzle generation (symmetry, bottlenecks etc.)
