# Chess Engine

Basic chess engine written in C++, built to understand fundamental concepts of chess programming. This project is work in progress.

## Current State
Implementing basic structures for board representation and move generation.

## Features
List of fundamental concepts that have already been implemented
- bitboard-based board representation, i.e. using 64-bit integers as occupancy grids and for move generation, which allows for efficient bit operations (https://www.chessprogramming.org/Bitboard_Board-Definition)
- precalculated attack tables that allow for fast lookup of attacked squares and pieces (https://www.chessprogramming.org/Attack_and_Defend_Maps)
- board generation based on FEN notation (https://de.wikipedia.org/wiki/Forsyth-Edwards-Notation)
- board status struct containing unobservable game information, e.g. side to move, castling rights, en-passant squares (work in progress)

## Future Work
List of logical next steps and missing functionalities:
- comprehensive move generation including special cases, e.g. pinned pieces, check and double check, en-passant captures and pawn promotions
- implementation of mate and stalemate logic
- performance tests to validate move generation
- UCI communication interface for GUI usage
- search logic, typically alpha-beta minimax search
- heuristics for move evaluation
- ...