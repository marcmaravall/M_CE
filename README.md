# M_CE

M_CE is my own chess engine (Marc's Chess Engine), written in C++ with no external libraries.  
It uses a minimax algorithm with alpha-beta pruning, move ordering, and transposition tables for search, along with a simple evaluation function.

# Discord:
Link: https://discord.gg/GjPHXuyzmD

# Features:
- Bitboards for move generation and board representation.
- Minimax with alpha-beta pruning.
- Move ordering.
- Transposition tables.
- Simple evaluation.
- Very poor search speed (takes more than 2 seconds at depth 5). I'm working to improve this.
- Polyglot opening books.
- Basic UCI support (only very basic commands implemented).

# ELO:
The engine has played some games against me and other low-ELO bots.  
(My own games were quite bad because they were just for testing; I’m not actually that weak.)

A rough estimate of its ELO is between **800 and 1200** at depth 5.  
I want this engine to eventually reach **2700 ELO or more**.

# HOW TO PLAY:
To play against the engine, you can install a GUI and connect the engine using the UCI protocol.  
Note: Only basic UCI commands are supported, so make sure to set a fixed search depth instead of using `go wtime ...`, which is not implemented yet.
