# Chess Engine

Basic chess engine written in C++, built to understand fundamental concepts of chess programming. This project is work in progress.

## Current State

Implementing basic structures for board representation and move generation.

## Features

List of fundamental concepts that have already been implemented:

- bitboard-based board representation, i.e. using 64-bit integers as occupancy grids and for move generation, which allows for efficient bit operations (https://www.chessprogramming.org/Bitboard_Board-Definition)
- precalculated attack tables that allow for fast lookup of attacked squares and pieces (https://www.chessprogramming.org/Attack_and_Defend_Maps)
- magic number generation for efficient hashing of board configurations during sliding piece attack generation
- board generation based on FEN notation (https://de.wikipedia.org/wiki/Forsyth-Edwards-Notation)

## Future Work

List of logical next steps and missing functionalities:

- board status struct containing unobservable game information, e.g. side to move, castling rights, en-passant squares
- comprehensive move generation including special cases, e.g. pinned pieces, check and double check, en-passant captures and pawn promotions
- implementation of mate and stalemate logic
- performance tests ```perft``` to validate move generation
- UCI communication interface for GUI usage
- search logic, typically alpha-beta minimax search
- heuristics for move evaluation
- ...

## References

A vast amount of resources is available on chess programming. These are the ones that I use most during this project:

- [Chess Programming Wiki](https://www.chessprogramming.org/Main_Page) &rarr; resources on every detail of chess programming, absolute rabbit hole
- [Sebastian Lague](https://www.youtube.com/watch?v=U4ogK0MIzqk) &rarr; the reason why I and many others got into chess programming
- [Bitboard CHESS ENGINE in C](https://www.youtube.com/playlist?list=PLmN0neTso3Jxh8ZIylk74JpwfiWNI76Cs) &rarr; comprehensive development of a bitboard-based chess engine in C
- [Ameye Chess Engine](https://ameye.dev/notes/chess-engine/) &rarr; insights into and reasoning behind a chess engine based on C++