# M_CE

M_CE is my own chess engine (Marc's Chess Engine), written in C++ with no external libraries.  
It uses a minimax algorithm with alpha-beta pruning, move ordering, and transposition tables for search, along with a simple evaluation function.

# Discord:
Link: https://discord.gg/GjPHXuyzmD

# Features:
- Bitboards for move generation and board representation.
- Minimax with alpha-beta pruning.
- MVL_LVA Move ordering.
- Transposition tables.
- Evaluation with pawn structs and different game phases interpolation.
- Decent search speed (800K - 1.2M opening (depth 7), more than 2M in some endgames (depth 12 - 25)).
- Polyglot opening books.
- UCI support.
- Decent table based time managment.

# ELO:
The engine has played some games against me and other low-ELO bots.  

A rough estimate of its ELO is between **1500 and 2100**.  
I want this engine to eventually reach **2700 ELO or more**.

# HOW TO PLAY:
To play against the engine, you can install a GUI and connect the engine using the UCI protocol.  
